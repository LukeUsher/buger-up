param(
    [Parameter(Mandatory=$true)]
    [string]$dllPath
)

if (!(Test-Path $dllPath)) {
    Write-Error "DLL path '$dllPath' does not exist."
    exit 1
}

$tempFile = [System.IO.Path]::GetTempFileName()
dumpbin /exports "$dllPath" > "$tempFile"

$excludeNames = @("functions","names","DllCanUnloadNow","DllGetClassObject","ordinal")
$exports = @()
Get-Content $tempFile | ForEach-Object {
    if ($_ -match '^\s+\d+\s+\S+\s+\S+\s+(\S+)$') {
        $name = $matches[1]
        if ($name -ne '' -and -not ($excludeNames -contains $name)) {
            $exports += $name
        }
    }
}

Remove-Item $tempFile

if ($exports.Count -eq 0) {
    Write-Error "No named exports found in $dllPath"
    exit 1
}

$dllName = [System.IO.Path]::GetFileName($dllPath)
$dllBase = [System.IO.Path]::GetFileNameWithoutExtension($dllPath)
$setupFuncName = "${dllBase}_InitProxies"

$outputDir = "proxy"
if (!(Test-Path $outputDir)) {
    New-Item -ItemType Directory -Path $outputDir | Out-Null
}

$outputFile = Join-Path $outputDir "${dllBase}.h"

if ([string]::IsNullOrEmpty($outputFile)) {
    Write-Error "Output file path is null or empty!"
    exit 1
}

Write-Output "Generating proxy C++ file at: $outputFile"

$prefix = "${dllBase}_"

$lines = @(
"// Auto-generated proxy for $dllName",
"",
"static HMODULE ${prefix}hOriginal = NULL;",
"static FARPROC* ${prefix}JumpTable = NULL;",
"static const char** ${prefix}FunctionNames = NULL;",
"static int ${prefix}ExportCount = 0;",
"",
"void $setupFuncName() {",
"    if (${prefix}hOriginal) return;",
"",
"    char path[MAX_PATH];",
"    GetSystemDirectoryA(path, MAX_PATH);",
"    strcat_s(path, `"\\$dllName`");",
"    ${prefix}hOriginal = LoadLibraryA(path);",
"",
"    ${prefix}ExportCount = $($exports.Count);",
"    ${prefix}FunctionNames = new const char*[${prefix}ExportCount];",
"    ${prefix}JumpTable = new FARPROC[${prefix}ExportCount];"
)

for ($i=0; $i -lt $exports.Count; $i++) {
    $func = $exports[$i]
    $lines += "    ${prefix}FunctionNames[$i] = `"$func`";"
}

$lines += @(
"",
"    for (int i = 0; i < ${prefix}ExportCount; i++) {",
"        ${prefix}JumpTable[i] = GetProcAddress(${prefix}hOriginal, ${prefix}FunctionNames[i]);",
"    }",
"}",
"",
'extern "C" {'
)

$lines | Set-Content -Path $outputFile -Encoding ASCII

for ($i=0; $i -lt $exports.Count; $i++) {
    $func = $exports[$i]
    $proxyName = "${dllBase}_proxy_${func}"
    $stub = @"
__declspec(naked) void $proxyName() {
    __asm {
        mov eax, ${prefix}JumpTable
        mov ecx, $i
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
"@
    Add-Content -Path $outputFile -Value $stub -Encoding ASCII
}

Add-Content -Path $outputFile -Value "}" -Encoding ASCII

for ($i=0; $i -lt $exports.Count; $i++) {
    $func = $exports[$i]
    $proxyName = "${dllBase}_proxy_${func}"

    $pragma = "#pragma comment(linker,`"/export:$func=_$proxyName`")"

    $stub = @"
$pragma
"@
    Add-Content -Path $outputFile -Value $stub -Encoding ASCII
}


Write-Output "Proxy generation complete: $outputFile"
