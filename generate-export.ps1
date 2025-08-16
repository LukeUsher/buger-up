param(
    [Parameter(Mandatory = $true)]
    [string]$dllPath
)

# Check if the file exists
if (!(Test-Path $dllPath)) {
    Write-Error "DLL path '$dllPath' does not exist."
    exit 1
}

# Use dumpbin to extract exports
$tempFile = "exports_temp.txt"
dumpbin /exports $dllPath > $tempFile

# Regex pattern to match: ordinal hint RVA name
$pattern = '^\s+(\d+)\s+\S+\s+\S+\s+(\S+)$'

# Extract DLL base name (no path, no extension)
$dllName = [System.IO.Path]::GetFileNameWithoutExtension($dllPath)

# Escape for pragma path
$escapedDllPath = $dllPath -replace '\\', '\\\\'

# Create output dir
$outputDir = "pragma"
if (!(Test-Path $outputDir)) {
    New-Item -ItemType Directory -Path $outputDir | Out-Null
}

# Set output file
$outputFile = Join-Path $outputDir "$dllName.h"

# Extract and generate pragmas
Get-Content $tempFile | ForEach-Object {
    if ($_ -match $pattern) {
        $ordinal = $matches[1]
        $func = $matches[2]
        '#pragma comment(linker,"/export:' + $func + '=' + $escapedDllPath + '.' + $func + ',@' + $ordinal + '")'
    }
} | Set-Content $outputFile

Remove-Item $tempFile
Write-Output "Generated pragmas written to $outputFile"
