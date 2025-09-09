// Auto-generated proxy for ws2_32.dll

static HMODULE ws2_32_hOriginal = NULL;
static FARPROC* ws2_32_JumpTable = NULL;
static const char** ws2_32_FunctionNames = NULL;
static int ws2_32_ExportCount = 0;

void ws2_32_InitProxies() {
    if (ws2_32_hOriginal) return;

    char path[MAX_PATH];
    GetSystemDirectoryA(path, MAX_PATH);
    strcat_s(path, "\\ws2_32.dll");
    ws2_32_hOriginal = LoadLibraryA(path);

    ws2_32_ExportCount = 180;
    ws2_32_FunctionNames = new const char*[ws2_32_ExportCount];
    ws2_32_JumpTable = new FARPROC[ws2_32_ExportCount];
    ws2_32_FunctionNames[0] = "FreeAddrInfoEx";
    ws2_32_FunctionNames[1] = "FreeAddrInfoExW";
    ws2_32_FunctionNames[2] = "FreeAddrInfoW";
    ws2_32_FunctionNames[3] = "GetAddrInfoExA";
    ws2_32_FunctionNames[4] = "GetAddrInfoExCancel";
    ws2_32_FunctionNames[5] = "GetAddrInfoExOverlappedResult";
    ws2_32_FunctionNames[6] = "GetAddrInfoExW";
    ws2_32_FunctionNames[7] = "GetAddrInfoW";
    ws2_32_FunctionNames[8] = "GetHostNameW";
    ws2_32_FunctionNames[9] = "GetNameInfoW";
    ws2_32_FunctionNames[10] = "InetNtopW";
    ws2_32_FunctionNames[11] = "InetPtonW";
    ws2_32_FunctionNames[12] = "SetAddrInfoExA";
    ws2_32_FunctionNames[13] = "SetAddrInfoExW";
    ws2_32_FunctionNames[14] = "WEP";
    ws2_32_FunctionNames[15] = "WPUCompleteOverlappedRequest";
    ws2_32_FunctionNames[16] = "WPUGetProviderPathEx";
    ws2_32_FunctionNames[17] = "WSAAccept";
    ws2_32_FunctionNames[18] = "WSAAddressToStringA";
    ws2_32_FunctionNames[19] = "WSAAddressToStringW";
    ws2_32_FunctionNames[20] = "WSAAdvertiseProvider";
    ws2_32_FunctionNames[21] = "WSAAsyncGetHostByAddr";
    ws2_32_FunctionNames[22] = "WSAAsyncGetHostByName";
    ws2_32_FunctionNames[23] = "WSAAsyncGetProtoByName";
    ws2_32_FunctionNames[24] = "WSAAsyncGetProtoByNumber";
    ws2_32_FunctionNames[25] = "WSAAsyncGetServByName";
    ws2_32_FunctionNames[26] = "WSAAsyncGetServByPort";
    ws2_32_FunctionNames[27] = "WSAAsyncSelect";
    ws2_32_FunctionNames[28] = "WSACancelAsyncRequest";
    ws2_32_FunctionNames[29] = "WSACancelBlockingCall";
    ws2_32_FunctionNames[30] = "WSACleanup";
    ws2_32_FunctionNames[31] = "WSACloseEvent";
    ws2_32_FunctionNames[32] = "WSAConnect";
    ws2_32_FunctionNames[33] = "WSAConnectByList";
    ws2_32_FunctionNames[34] = "WSAConnectByNameA";
    ws2_32_FunctionNames[35] = "WSAConnectByNameW";
    ws2_32_FunctionNames[36] = "WSACreateEvent";
    ws2_32_FunctionNames[37] = "WSADuplicateSocketA";
    ws2_32_FunctionNames[38] = "WSADuplicateSocketW";
    ws2_32_FunctionNames[39] = "WSAEnumNameSpaceProvidersA";
    ws2_32_FunctionNames[40] = "WSAEnumNameSpaceProvidersExA";
    ws2_32_FunctionNames[41] = "WSAEnumNameSpaceProvidersExW";
    ws2_32_FunctionNames[42] = "WSAEnumNameSpaceProvidersW";
    ws2_32_FunctionNames[43] = "WSAEnumNetworkEvents";
    ws2_32_FunctionNames[44] = "WSAEnumProtocolsA";
    ws2_32_FunctionNames[45] = "WSAEnumProtocolsW";
    ws2_32_FunctionNames[46] = "WSAEventSelect";
    ws2_32_FunctionNames[47] = "WSAGetLastError";
    ws2_32_FunctionNames[48] = "WSAGetOverlappedResult";
    ws2_32_FunctionNames[49] = "WSAGetQOSByName";
    ws2_32_FunctionNames[50] = "WSAGetServiceClassInfoA";
    ws2_32_FunctionNames[51] = "WSAGetServiceClassInfoW";
    ws2_32_FunctionNames[52] = "WSAGetServiceClassNameByClassIdA";
    ws2_32_FunctionNames[53] = "WSAGetServiceClassNameByClassIdW";
    ws2_32_FunctionNames[54] = "WSAHtonl";
    ws2_32_FunctionNames[55] = "WSAHtons";
    ws2_32_FunctionNames[56] = "WSAInstallServiceClassA";
    ws2_32_FunctionNames[57] = "WSAInstallServiceClassW";
    ws2_32_FunctionNames[58] = "WSAIoctl";
    ws2_32_FunctionNames[59] = "WSAIsBlocking";
    ws2_32_FunctionNames[60] = "WSAJoinLeaf";
    ws2_32_FunctionNames[61] = "WSALookupServiceBeginA";
    ws2_32_FunctionNames[62] = "WSALookupServiceBeginW";
    ws2_32_FunctionNames[63] = "WSALookupServiceEnd";
    ws2_32_FunctionNames[64] = "WSALookupServiceNextA";
    ws2_32_FunctionNames[65] = "WSALookupServiceNextW";
    ws2_32_FunctionNames[66] = "WSANSPIoctl";
    ws2_32_FunctionNames[67] = "WSANtohl";
    ws2_32_FunctionNames[68] = "WSANtohs";
    ws2_32_FunctionNames[69] = "WSAPoll";
    ws2_32_FunctionNames[70] = "WSAProviderCompleteAsyncCall";
    ws2_32_FunctionNames[71] = "WSAProviderConfigChange";
    ws2_32_FunctionNames[72] = "WSARecv";
    ws2_32_FunctionNames[73] = "WSARecvDisconnect";
    ws2_32_FunctionNames[74] = "WSARecvFrom";
    ws2_32_FunctionNames[75] = "WSARemoveServiceClass";
    ws2_32_FunctionNames[76] = "WSAResetEvent";
    ws2_32_FunctionNames[77] = "WSASend";
    ws2_32_FunctionNames[78] = "WSASendDisconnect";
    ws2_32_FunctionNames[79] = "WSASendMsg";
    ws2_32_FunctionNames[80] = "WSASendTo";
    ws2_32_FunctionNames[81] = "WSASetBlockingHook";
    ws2_32_FunctionNames[82] = "WSASetEvent";
    ws2_32_FunctionNames[83] = "WSASetLastError";
    ws2_32_FunctionNames[84] = "WSASetServiceA";
    ws2_32_FunctionNames[85] = "WSASetServiceW";
    ws2_32_FunctionNames[86] = "WSASocketA";
    ws2_32_FunctionNames[87] = "WSASocketW";
    ws2_32_FunctionNames[88] = "WSAStartup";
    ws2_32_FunctionNames[89] = "WSAStringToAddressA";
    ws2_32_FunctionNames[90] = "WSAStringToAddressW";
    ws2_32_FunctionNames[91] = "WSAUnadvertiseProvider";
    ws2_32_FunctionNames[92] = "WSAUnhookBlockingHook";
    ws2_32_FunctionNames[93] = "WSAWaitForMultipleEvents";
    ws2_32_FunctionNames[94] = "WSApSetPostRoutine";
    ws2_32_FunctionNames[95] = "WSCDeinstallProvider";
    ws2_32_FunctionNames[96] = "WSCDeinstallProviderEx";
    ws2_32_FunctionNames[97] = "WSCEnableNSProvider";
    ws2_32_FunctionNames[98] = "WSCEnumProtocols";
    ws2_32_FunctionNames[99] = "WSCEnumProtocolsEx";
    ws2_32_FunctionNames[100] = "WSCGetApplicationCategory";
    ws2_32_FunctionNames[101] = "WSCGetApplicationCategoryEx";
    ws2_32_FunctionNames[102] = "WSCGetProviderInfo";
    ws2_32_FunctionNames[103] = "WSCGetProviderPath";
    ws2_32_FunctionNames[104] = "WSCInstallNameSpace";
    ws2_32_FunctionNames[105] = "WSCInstallNameSpaceEx";
    ws2_32_FunctionNames[106] = "WSCInstallNameSpaceEx2";
    ws2_32_FunctionNames[107] = "WSCInstallProvider";
    ws2_32_FunctionNames[108] = "WSCInstallProviderAndChains";
    ws2_32_FunctionNames[109] = "WSCInstallProviderEx";
    ws2_32_FunctionNames[110] = "WSCSetApplicationCategory";
    ws2_32_FunctionNames[111] = "WSCSetApplicationCategoryEx";
    ws2_32_FunctionNames[112] = "WSCSetProviderInfo";
    ws2_32_FunctionNames[113] = "WSCUnInstallNameSpace";
    ws2_32_FunctionNames[114] = "WSCUnInstallNameSpaceEx2";
    ws2_32_FunctionNames[115] = "WSCUpdateProvider";
    ws2_32_FunctionNames[116] = "WSCUpdateProviderEx";
    ws2_32_FunctionNames[117] = "WSCWriteNameSpaceOrder";
    ws2_32_FunctionNames[118] = "WSCWriteProviderOrder";
    ws2_32_FunctionNames[119] = "WSCWriteProviderOrderEx";
    ws2_32_FunctionNames[120] = "WahCloseApcHelper";
    ws2_32_FunctionNames[121] = "WahCloseHandleHelper";
    ws2_32_FunctionNames[122] = "WahCloseNotificationHandleHelper";
    ws2_32_FunctionNames[123] = "WahCloseSocketHandle";
    ws2_32_FunctionNames[124] = "WahCloseThread";
    ws2_32_FunctionNames[125] = "WahCompleteRequest";
    ws2_32_FunctionNames[126] = "WahCreateHandleContextTable";
    ws2_32_FunctionNames[127] = "WahCreateNotificationHandle";
    ws2_32_FunctionNames[128] = "WahCreateSocketHandle";
    ws2_32_FunctionNames[129] = "WahDestroyHandleContextTable";
    ws2_32_FunctionNames[130] = "WahDisableNonIFSHandleSupport";
    ws2_32_FunctionNames[131] = "WahEnableNonIFSHandleSupport";
    ws2_32_FunctionNames[132] = "WahEnumerateHandleContexts";
    ws2_32_FunctionNames[133] = "WahInsertHandleContext";
    ws2_32_FunctionNames[134] = "WahNotifyAllProcesses";
    ws2_32_FunctionNames[135] = "WahOpenApcHelper";
    ws2_32_FunctionNames[136] = "WahOpenCurrentThread";
    ws2_32_FunctionNames[137] = "WahOpenHandleHelper";
    ws2_32_FunctionNames[138] = "WahOpenNotificationHandleHelper";
    ws2_32_FunctionNames[139] = "WahQueueUserApc";
    ws2_32_FunctionNames[140] = "WahReferenceContextByHandle";
    ws2_32_FunctionNames[141] = "WahRemoveHandleContext";
    ws2_32_FunctionNames[142] = "WahWaitForNotification";
    ws2_32_FunctionNames[143] = "WahWriteLSPEvent";
    ws2_32_FunctionNames[144] = "__WSAFDIsSet";
    ws2_32_FunctionNames[145] = "accept";
    ws2_32_FunctionNames[146] = "bind";
    ws2_32_FunctionNames[147] = "closesocket";
    ws2_32_FunctionNames[148] = "connect";
    ws2_32_FunctionNames[149] = "freeaddrinfo";
    ws2_32_FunctionNames[150] = "getaddrinfo";
    ws2_32_FunctionNames[151] = "gethostbyaddr";
    ws2_32_FunctionNames[152] = "gethostbyname";
    ws2_32_FunctionNames[153] = "gethostname";
    ws2_32_FunctionNames[154] = "getnameinfo";
    ws2_32_FunctionNames[155] = "getpeername";
    ws2_32_FunctionNames[156] = "getprotobyname";
    ws2_32_FunctionNames[157] = "getprotobynumber";
    ws2_32_FunctionNames[158] = "getservbyname";
    ws2_32_FunctionNames[159] = "getservbyport";
    ws2_32_FunctionNames[160] = "getsockname";
    ws2_32_FunctionNames[161] = "getsockopt";
    ws2_32_FunctionNames[162] = "htonl";
    ws2_32_FunctionNames[163] = "htons";
    ws2_32_FunctionNames[164] = "inet_addr";
    ws2_32_FunctionNames[165] = "inet_ntoa";
    ws2_32_FunctionNames[166] = "inet_ntop";
    ws2_32_FunctionNames[167] = "inet_pton";
    ws2_32_FunctionNames[168] = "ioctlsocket";
    ws2_32_FunctionNames[169] = "listen";
    ws2_32_FunctionNames[170] = "ntohl";
    ws2_32_FunctionNames[171] = "ntohs";
    ws2_32_FunctionNames[172] = "recv";
    ws2_32_FunctionNames[173] = "recvfrom";
    ws2_32_FunctionNames[174] = "select";
    ws2_32_FunctionNames[175] = "send";
    ws2_32_FunctionNames[176] = "sendto";
    ws2_32_FunctionNames[177] = "setsockopt";
    ws2_32_FunctionNames[178] = "shutdown";
    ws2_32_FunctionNames[179] = "socket";

    for (int i = 0; i < ws2_32_ExportCount; i++) {
        ws2_32_JumpTable[i] = GetProcAddress(ws2_32_hOriginal, ws2_32_FunctionNames[i]);
    }
}

extern "C" {
__declspec(naked) void ws2_32_proxy_FreeAddrInfoEx() {
    __asm {
        mov eax, ws2_32_JumpTable
        mov ecx, 0
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void ws2_32_proxy_FreeAddrInfoExW() {
    __asm {
        mov eax, ws2_32_JumpTable
        mov ecx, 1
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void ws2_32_proxy_FreeAddrInfoW() {
    __asm {
        mov eax, ws2_32_JumpTable
        mov ecx, 2
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void ws2_32_proxy_GetAddrInfoExA() {
    __asm {
        mov eax, ws2_32_JumpTable
        mov ecx, 3
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void ws2_32_proxy_GetAddrInfoExCancel() {
    __asm {
        mov eax, ws2_32_JumpTable
        mov ecx, 4
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void ws2_32_proxy_GetAddrInfoExOverlappedResult() {
    __asm {
        mov eax, ws2_32_JumpTable
        mov ecx, 5
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void ws2_32_proxy_GetAddrInfoExW() {
    __asm {
        mov eax, ws2_32_JumpTable
        mov ecx, 6
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void ws2_32_proxy_GetAddrInfoW() {
    __asm {
        mov eax, ws2_32_JumpTable
        mov ecx, 7
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void ws2_32_proxy_GetHostNameW() {
    __asm {
        mov eax, ws2_32_JumpTable
        mov ecx, 8
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void ws2_32_proxy_GetNameInfoW() {
    __asm {
        mov eax, ws2_32_JumpTable
        mov ecx, 9
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void ws2_32_proxy_InetNtopW() {
    __asm {
        mov eax, ws2_32_JumpTable
        mov ecx, 10
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void ws2_32_proxy_InetPtonW() {
    __asm {
        mov eax, ws2_32_JumpTable
        mov ecx, 11
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void ws2_32_proxy_SetAddrInfoExA() {
    __asm {
        mov eax, ws2_32_JumpTable
        mov ecx, 12
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void ws2_32_proxy_SetAddrInfoExW() {
    __asm {
        mov eax, ws2_32_JumpTable
        mov ecx, 13
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void ws2_32_proxy_WEP() {
    __asm {
        mov eax, ws2_32_JumpTable
        mov ecx, 14
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void ws2_32_proxy_WPUCompleteOverlappedRequest() {
    __asm {
        mov eax, ws2_32_JumpTable
        mov ecx, 15
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void ws2_32_proxy_WPUGetProviderPathEx() {
    __asm {
        mov eax, ws2_32_JumpTable
        mov ecx, 16
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void ws2_32_proxy_WSAAccept() {
    __asm {
        mov eax, ws2_32_JumpTable
        mov ecx, 17
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void ws2_32_proxy_WSAAddressToStringA() {
    __asm {
        mov eax, ws2_32_JumpTable
        mov ecx, 18
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void ws2_32_proxy_WSAAddressToStringW() {
    __asm {
        mov eax, ws2_32_JumpTable
        mov ecx, 19
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void ws2_32_proxy_WSAAdvertiseProvider() {
    __asm {
        mov eax, ws2_32_JumpTable
        mov ecx, 20
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void ws2_32_proxy_WSAAsyncGetHostByAddr() {
    __asm {
        mov eax, ws2_32_JumpTable
        mov ecx, 21
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void ws2_32_proxy_WSAAsyncGetHostByName() {
    __asm {
        mov eax, ws2_32_JumpTable
        mov ecx, 22
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void ws2_32_proxy_WSAAsyncGetProtoByName() {
    __asm {
        mov eax, ws2_32_JumpTable
        mov ecx, 23
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void ws2_32_proxy_WSAAsyncGetProtoByNumber() {
    __asm {
        mov eax, ws2_32_JumpTable
        mov ecx, 24
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void ws2_32_proxy_WSAAsyncGetServByName() {
    __asm {
        mov eax, ws2_32_JumpTable
        mov ecx, 25
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void ws2_32_proxy_WSAAsyncGetServByPort() {
    __asm {
        mov eax, ws2_32_JumpTable
        mov ecx, 26
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void ws2_32_proxy_WSAAsyncSelect() {
    __asm {
        mov eax, ws2_32_JumpTable
        mov ecx, 27
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void ws2_32_proxy_WSACancelAsyncRequest() {
    __asm {
        mov eax, ws2_32_JumpTable
        mov ecx, 28
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void ws2_32_proxy_WSACancelBlockingCall() {
    __asm {
        mov eax, ws2_32_JumpTable
        mov ecx, 29
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void ws2_32_proxy_WSACleanup() {
    __asm {
        mov eax, ws2_32_JumpTable
        mov ecx, 30
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void ws2_32_proxy_WSACloseEvent() {
    __asm {
        mov eax, ws2_32_JumpTable
        mov ecx, 31
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void ws2_32_proxy_WSAConnect() {
    __asm {
        mov eax, ws2_32_JumpTable
        mov ecx, 32
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void ws2_32_proxy_WSAConnectByList() {
    __asm {
        mov eax, ws2_32_JumpTable
        mov ecx, 33
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void ws2_32_proxy_WSAConnectByNameA() {
    __asm {
        mov eax, ws2_32_JumpTable
        mov ecx, 34
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void ws2_32_proxy_WSAConnectByNameW() {
    __asm {
        mov eax, ws2_32_JumpTable
        mov ecx, 35
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void ws2_32_proxy_WSACreateEvent() {
    __asm {
        mov eax, ws2_32_JumpTable
        mov ecx, 36
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void ws2_32_proxy_WSADuplicateSocketA() {
    __asm {
        mov eax, ws2_32_JumpTable
        mov ecx, 37
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void ws2_32_proxy_WSADuplicateSocketW() {
    __asm {
        mov eax, ws2_32_JumpTable
        mov ecx, 38
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void ws2_32_proxy_WSAEnumNameSpaceProvidersA() {
    __asm {
        mov eax, ws2_32_JumpTable
        mov ecx, 39
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void ws2_32_proxy_WSAEnumNameSpaceProvidersExA() {
    __asm {
        mov eax, ws2_32_JumpTable
        mov ecx, 40
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void ws2_32_proxy_WSAEnumNameSpaceProvidersExW() {
    __asm {
        mov eax, ws2_32_JumpTable
        mov ecx, 41
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void ws2_32_proxy_WSAEnumNameSpaceProvidersW() {
    __asm {
        mov eax, ws2_32_JumpTable
        mov ecx, 42
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void ws2_32_proxy_WSAEnumNetworkEvents() {
    __asm {
        mov eax, ws2_32_JumpTable
        mov ecx, 43
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void ws2_32_proxy_WSAEnumProtocolsA() {
    __asm {
        mov eax, ws2_32_JumpTable
        mov ecx, 44
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void ws2_32_proxy_WSAEnumProtocolsW() {
    __asm {
        mov eax, ws2_32_JumpTable
        mov ecx, 45
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void ws2_32_proxy_WSAEventSelect() {
    __asm {
        mov eax, ws2_32_JumpTable
        mov ecx, 46
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void ws2_32_proxy_WSAGetLastError() {
    __asm {
        mov eax, ws2_32_JumpTable
        mov ecx, 47
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void ws2_32_proxy_WSAGetOverlappedResult() {
    __asm {
        mov eax, ws2_32_JumpTable
        mov ecx, 48
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void ws2_32_proxy_WSAGetQOSByName() {
    __asm {
        mov eax, ws2_32_JumpTable
        mov ecx, 49
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void ws2_32_proxy_WSAGetServiceClassInfoA() {
    __asm {
        mov eax, ws2_32_JumpTable
        mov ecx, 50
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void ws2_32_proxy_WSAGetServiceClassInfoW() {
    __asm {
        mov eax, ws2_32_JumpTable
        mov ecx, 51
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void ws2_32_proxy_WSAGetServiceClassNameByClassIdA() {
    __asm {
        mov eax, ws2_32_JumpTable
        mov ecx, 52
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void ws2_32_proxy_WSAGetServiceClassNameByClassIdW() {
    __asm {
        mov eax, ws2_32_JumpTable
        mov ecx, 53
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void ws2_32_proxy_WSAHtonl() {
    __asm {
        mov eax, ws2_32_JumpTable
        mov ecx, 54
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void ws2_32_proxy_WSAHtons() {
    __asm {
        mov eax, ws2_32_JumpTable
        mov ecx, 55
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void ws2_32_proxy_WSAInstallServiceClassA() {
    __asm {
        mov eax, ws2_32_JumpTable
        mov ecx, 56
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void ws2_32_proxy_WSAInstallServiceClassW() {
    __asm {
        mov eax, ws2_32_JumpTable
        mov ecx, 57
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void ws2_32_proxy_WSAIoctl() {
    __asm {
        mov eax, ws2_32_JumpTable
        mov ecx, 58
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void ws2_32_proxy_WSAIsBlocking() {
    __asm {
        mov eax, ws2_32_JumpTable
        mov ecx, 59
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void ws2_32_proxy_WSAJoinLeaf() {
    __asm {
        mov eax, ws2_32_JumpTable
        mov ecx, 60
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void ws2_32_proxy_WSALookupServiceBeginA() {
    __asm {
        mov eax, ws2_32_JumpTable
        mov ecx, 61
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void ws2_32_proxy_WSALookupServiceBeginW() {
    __asm {
        mov eax, ws2_32_JumpTable
        mov ecx, 62
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void ws2_32_proxy_WSALookupServiceEnd() {
    __asm {
        mov eax, ws2_32_JumpTable
        mov ecx, 63
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void ws2_32_proxy_WSALookupServiceNextA() {
    __asm {
        mov eax, ws2_32_JumpTable
        mov ecx, 64
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void ws2_32_proxy_WSALookupServiceNextW() {
    __asm {
        mov eax, ws2_32_JumpTable
        mov ecx, 65
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void ws2_32_proxy_WSANSPIoctl() {
    __asm {
        mov eax, ws2_32_JumpTable
        mov ecx, 66
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void ws2_32_proxy_WSANtohl() {
    __asm {
        mov eax, ws2_32_JumpTable
        mov ecx, 67
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void ws2_32_proxy_WSANtohs() {
    __asm {
        mov eax, ws2_32_JumpTable
        mov ecx, 68
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void ws2_32_proxy_WSAPoll() {
    __asm {
        mov eax, ws2_32_JumpTable
        mov ecx, 69
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void ws2_32_proxy_WSAProviderCompleteAsyncCall() {
    __asm {
        mov eax, ws2_32_JumpTable
        mov ecx, 70
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void ws2_32_proxy_WSAProviderConfigChange() {
    __asm {
        mov eax, ws2_32_JumpTable
        mov ecx, 71
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void ws2_32_proxy_WSARecv() {
    __asm {
        mov eax, ws2_32_JumpTable
        mov ecx, 72
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void ws2_32_proxy_WSARecvDisconnect() {
    __asm {
        mov eax, ws2_32_JumpTable
        mov ecx, 73
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void ws2_32_proxy_WSARecvFrom() {
    __asm {
        mov eax, ws2_32_JumpTable
        mov ecx, 74
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void ws2_32_proxy_WSARemoveServiceClass() {
    __asm {
        mov eax, ws2_32_JumpTable
        mov ecx, 75
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void ws2_32_proxy_WSAResetEvent() {
    __asm {
        mov eax, ws2_32_JumpTable
        mov ecx, 76
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void ws2_32_proxy_WSASend() {
    __asm {
        mov eax, ws2_32_JumpTable
        mov ecx, 77
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void ws2_32_proxy_WSASendDisconnect() {
    __asm {
        mov eax, ws2_32_JumpTable
        mov ecx, 78
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void ws2_32_proxy_WSASendMsg() {
    __asm {
        mov eax, ws2_32_JumpTable
        mov ecx, 79
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void ws2_32_proxy_WSASendTo() {
    __asm {
        mov eax, ws2_32_JumpTable
        mov ecx, 80
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void ws2_32_proxy_WSASetBlockingHook() {
    __asm {
        mov eax, ws2_32_JumpTable
        mov ecx, 81
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void ws2_32_proxy_WSASetEvent() {
    __asm {
        mov eax, ws2_32_JumpTable
        mov ecx, 82
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void ws2_32_proxy_WSASetLastError() {
    __asm {
        mov eax, ws2_32_JumpTable
        mov ecx, 83
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void ws2_32_proxy_WSASetServiceA() {
    __asm {
        mov eax, ws2_32_JumpTable
        mov ecx, 84
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void ws2_32_proxy_WSASetServiceW() {
    __asm {
        mov eax, ws2_32_JumpTable
        mov ecx, 85
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void ws2_32_proxy_WSASocketA() {
    __asm {
        mov eax, ws2_32_JumpTable
        mov ecx, 86
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void ws2_32_proxy_WSASocketW() {
    __asm {
        mov eax, ws2_32_JumpTable
        mov ecx, 87
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void ws2_32_proxy_WSAStartup() {
    __asm {
        mov eax, ws2_32_JumpTable
        mov ecx, 88
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void ws2_32_proxy_WSAStringToAddressA() {
    __asm {
        mov eax, ws2_32_JumpTable
        mov ecx, 89
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void ws2_32_proxy_WSAStringToAddressW() {
    __asm {
        mov eax, ws2_32_JumpTable
        mov ecx, 90
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void ws2_32_proxy_WSAUnadvertiseProvider() {
    __asm {
        mov eax, ws2_32_JumpTable
        mov ecx, 91
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void ws2_32_proxy_WSAUnhookBlockingHook() {
    __asm {
        mov eax, ws2_32_JumpTable
        mov ecx, 92
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void ws2_32_proxy_WSAWaitForMultipleEvents() {
    __asm {
        mov eax, ws2_32_JumpTable
        mov ecx, 93
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void ws2_32_proxy_WSApSetPostRoutine() {
    __asm {
        mov eax, ws2_32_JumpTable
        mov ecx, 94
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void ws2_32_proxy_WSCDeinstallProvider() {
    __asm {
        mov eax, ws2_32_JumpTable
        mov ecx, 95
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void ws2_32_proxy_WSCDeinstallProviderEx() {
    __asm {
        mov eax, ws2_32_JumpTable
        mov ecx, 96
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void ws2_32_proxy_WSCEnableNSProvider() {
    __asm {
        mov eax, ws2_32_JumpTable
        mov ecx, 97
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void ws2_32_proxy_WSCEnumProtocols() {
    __asm {
        mov eax, ws2_32_JumpTable
        mov ecx, 98
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void ws2_32_proxy_WSCEnumProtocolsEx() {
    __asm {
        mov eax, ws2_32_JumpTable
        mov ecx, 99
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void ws2_32_proxy_WSCGetApplicationCategory() {
    __asm {
        mov eax, ws2_32_JumpTable
        mov ecx, 100
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void ws2_32_proxy_WSCGetApplicationCategoryEx() {
    __asm {
        mov eax, ws2_32_JumpTable
        mov ecx, 101
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void ws2_32_proxy_WSCGetProviderInfo() {
    __asm {
        mov eax, ws2_32_JumpTable
        mov ecx, 102
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void ws2_32_proxy_WSCGetProviderPath() {
    __asm {
        mov eax, ws2_32_JumpTable
        mov ecx, 103
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void ws2_32_proxy_WSCInstallNameSpace() {
    __asm {
        mov eax, ws2_32_JumpTable
        mov ecx, 104
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void ws2_32_proxy_WSCInstallNameSpaceEx() {
    __asm {
        mov eax, ws2_32_JumpTable
        mov ecx, 105
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void ws2_32_proxy_WSCInstallNameSpaceEx2() {
    __asm {
        mov eax, ws2_32_JumpTable
        mov ecx, 106
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void ws2_32_proxy_WSCInstallProvider() {
    __asm {
        mov eax, ws2_32_JumpTable
        mov ecx, 107
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void ws2_32_proxy_WSCInstallProviderAndChains() {
    __asm {
        mov eax, ws2_32_JumpTable
        mov ecx, 108
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void ws2_32_proxy_WSCInstallProviderEx() {
    __asm {
        mov eax, ws2_32_JumpTable
        mov ecx, 109
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void ws2_32_proxy_WSCSetApplicationCategory() {
    __asm {
        mov eax, ws2_32_JumpTable
        mov ecx, 110
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void ws2_32_proxy_WSCSetApplicationCategoryEx() {
    __asm {
        mov eax, ws2_32_JumpTable
        mov ecx, 111
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void ws2_32_proxy_WSCSetProviderInfo() {
    __asm {
        mov eax, ws2_32_JumpTable
        mov ecx, 112
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void ws2_32_proxy_WSCUnInstallNameSpace() {
    __asm {
        mov eax, ws2_32_JumpTable
        mov ecx, 113
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void ws2_32_proxy_WSCUnInstallNameSpaceEx2() {
    __asm {
        mov eax, ws2_32_JumpTable
        mov ecx, 114
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void ws2_32_proxy_WSCUpdateProvider() {
    __asm {
        mov eax, ws2_32_JumpTable
        mov ecx, 115
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void ws2_32_proxy_WSCUpdateProviderEx() {
    __asm {
        mov eax, ws2_32_JumpTable
        mov ecx, 116
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void ws2_32_proxy_WSCWriteNameSpaceOrder() {
    __asm {
        mov eax, ws2_32_JumpTable
        mov ecx, 117
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void ws2_32_proxy_WSCWriteProviderOrder() {
    __asm {
        mov eax, ws2_32_JumpTable
        mov ecx, 118
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void ws2_32_proxy_WSCWriteProviderOrderEx() {
    __asm {
        mov eax, ws2_32_JumpTable
        mov ecx, 119
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void ws2_32_proxy_WahCloseApcHelper() {
    __asm {
        mov eax, ws2_32_JumpTable
        mov ecx, 120
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void ws2_32_proxy_WahCloseHandleHelper() {
    __asm {
        mov eax, ws2_32_JumpTable
        mov ecx, 121
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void ws2_32_proxy_WahCloseNotificationHandleHelper() {
    __asm {
        mov eax, ws2_32_JumpTable
        mov ecx, 122
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void ws2_32_proxy_WahCloseSocketHandle() {
    __asm {
        mov eax, ws2_32_JumpTable
        mov ecx, 123
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void ws2_32_proxy_WahCloseThread() {
    __asm {
        mov eax, ws2_32_JumpTable
        mov ecx, 124
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void ws2_32_proxy_WahCompleteRequest() {
    __asm {
        mov eax, ws2_32_JumpTable
        mov ecx, 125
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void ws2_32_proxy_WahCreateHandleContextTable() {
    __asm {
        mov eax, ws2_32_JumpTable
        mov ecx, 126
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void ws2_32_proxy_WahCreateNotificationHandle() {
    __asm {
        mov eax, ws2_32_JumpTable
        mov ecx, 127
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void ws2_32_proxy_WahCreateSocketHandle() {
    __asm {
        mov eax, ws2_32_JumpTable
        mov ecx, 128
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void ws2_32_proxy_WahDestroyHandleContextTable() {
    __asm {
        mov eax, ws2_32_JumpTable
        mov ecx, 129
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void ws2_32_proxy_WahDisableNonIFSHandleSupport() {
    __asm {
        mov eax, ws2_32_JumpTable
        mov ecx, 130
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void ws2_32_proxy_WahEnableNonIFSHandleSupport() {
    __asm {
        mov eax, ws2_32_JumpTable
        mov ecx, 131
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void ws2_32_proxy_WahEnumerateHandleContexts() {
    __asm {
        mov eax, ws2_32_JumpTable
        mov ecx, 132
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void ws2_32_proxy_WahInsertHandleContext() {
    __asm {
        mov eax, ws2_32_JumpTable
        mov ecx, 133
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void ws2_32_proxy_WahNotifyAllProcesses() {
    __asm {
        mov eax, ws2_32_JumpTable
        mov ecx, 134
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void ws2_32_proxy_WahOpenApcHelper() {
    __asm {
        mov eax, ws2_32_JumpTable
        mov ecx, 135
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void ws2_32_proxy_WahOpenCurrentThread() {
    __asm {
        mov eax, ws2_32_JumpTable
        mov ecx, 136
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void ws2_32_proxy_WahOpenHandleHelper() {
    __asm {
        mov eax, ws2_32_JumpTable
        mov ecx, 137
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void ws2_32_proxy_WahOpenNotificationHandleHelper() {
    __asm {
        mov eax, ws2_32_JumpTable
        mov ecx, 138
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void ws2_32_proxy_WahQueueUserApc() {
    __asm {
        mov eax, ws2_32_JumpTable
        mov ecx, 139
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void ws2_32_proxy_WahReferenceContextByHandle() {
    __asm {
        mov eax, ws2_32_JumpTable
        mov ecx, 140
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void ws2_32_proxy_WahRemoveHandleContext() {
    __asm {
        mov eax, ws2_32_JumpTable
        mov ecx, 141
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void ws2_32_proxy_WahWaitForNotification() {
    __asm {
        mov eax, ws2_32_JumpTable
        mov ecx, 142
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void ws2_32_proxy_WahWriteLSPEvent() {
    __asm {
        mov eax, ws2_32_JumpTable
        mov ecx, 143
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void ws2_32_proxy___WSAFDIsSet() {
    __asm {
        mov eax, ws2_32_JumpTable
        mov ecx, 144
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void ws2_32_proxy_accept() {
    __asm {
        mov eax, ws2_32_JumpTable
        mov ecx, 145
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void ws2_32_proxy_bind() {
    __asm {
        mov eax, ws2_32_JumpTable
        mov ecx, 146
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void ws2_32_proxy_closesocket() {
    __asm {
        mov eax, ws2_32_JumpTable
        mov ecx, 147
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void ws2_32_proxy_connect() {
    __asm {
        mov eax, ws2_32_JumpTable
        mov ecx, 148
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void ws2_32_proxy_freeaddrinfo() {
    __asm {
        mov eax, ws2_32_JumpTable
        mov ecx, 149
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void ws2_32_proxy_getaddrinfo() {
    __asm {
        mov eax, ws2_32_JumpTable
        mov ecx, 150
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void ws2_32_proxy_gethostbyaddr() {
    __asm {
        mov eax, ws2_32_JumpTable
        mov ecx, 151
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void ws2_32_proxy_gethostbyname() {
    __asm {
        mov eax, ws2_32_JumpTable
        mov ecx, 152
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void ws2_32_proxy_gethostname() {
    __asm {
        mov eax, ws2_32_JumpTable
        mov ecx, 153
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void ws2_32_proxy_getnameinfo() {
    __asm {
        mov eax, ws2_32_JumpTable
        mov ecx, 154
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void ws2_32_proxy_getpeername() {
    __asm {
        mov eax, ws2_32_JumpTable
        mov ecx, 155
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void ws2_32_proxy_getprotobyname() {
    __asm {
        mov eax, ws2_32_JumpTable
        mov ecx, 156
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void ws2_32_proxy_getprotobynumber() {
    __asm {
        mov eax, ws2_32_JumpTable
        mov ecx, 157
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void ws2_32_proxy_getservbyname() {
    __asm {
        mov eax, ws2_32_JumpTable
        mov ecx, 158
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void ws2_32_proxy_getservbyport() {
    __asm {
        mov eax, ws2_32_JumpTable
        mov ecx, 159
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void ws2_32_proxy_getsockname() {
    __asm {
        mov eax, ws2_32_JumpTable
        mov ecx, 160
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void ws2_32_proxy_getsockopt() {
    __asm {
        mov eax, ws2_32_JumpTable
        mov ecx, 161
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void ws2_32_proxy_htonl() {
    __asm {
        mov eax, ws2_32_JumpTable
        mov ecx, 162
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void ws2_32_proxy_htons() {
    __asm {
        mov eax, ws2_32_JumpTable
        mov ecx, 163
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void ws2_32_proxy_inet_addr() {
    __asm {
        mov eax, ws2_32_JumpTable
        mov ecx, 164
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void ws2_32_proxy_inet_ntoa() {
    __asm {
        mov eax, ws2_32_JumpTable
        mov ecx, 165
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void ws2_32_proxy_inet_ntop() {
    __asm {
        mov eax, ws2_32_JumpTable
        mov ecx, 166
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void ws2_32_proxy_inet_pton() {
    __asm {
        mov eax, ws2_32_JumpTable
        mov ecx, 167
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void ws2_32_proxy_ioctlsocket() {
    __asm {
        mov eax, ws2_32_JumpTable
        mov ecx, 168
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void ws2_32_proxy_listen() {
    __asm {
        mov eax, ws2_32_JumpTable
        mov ecx, 169
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void ws2_32_proxy_ntohl() {
    __asm {
        mov eax, ws2_32_JumpTable
        mov ecx, 170
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void ws2_32_proxy_ntohs() {
    __asm {
        mov eax, ws2_32_JumpTable
        mov ecx, 171
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void ws2_32_proxy_recv() {
    __asm {
        mov eax, ws2_32_JumpTable
        mov ecx, 172
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void ws2_32_proxy_recvfrom() {
    __asm {
        mov eax, ws2_32_JumpTable
        mov ecx, 173
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void ws2_32_proxy_select() {
    __asm {
        mov eax, ws2_32_JumpTable
        mov ecx, 174
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void ws2_32_proxy_send() {
    __asm {
        mov eax, ws2_32_JumpTable
        mov ecx, 175
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void ws2_32_proxy_sendto() {
    __asm {
        mov eax, ws2_32_JumpTable
        mov ecx, 176
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void ws2_32_proxy_setsockopt() {
    __asm {
        mov eax, ws2_32_JumpTable
        mov ecx, 177
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void ws2_32_proxy_shutdown() {
    __asm {
        mov eax, ws2_32_JumpTable
        mov ecx, 178
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void ws2_32_proxy_socket() {
    __asm {
        mov eax, ws2_32_JumpTable
        mov ecx, 179
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
}
#pragma comment(linker,"/export:FreeAddrInfoEx=_ws2_32_proxy_FreeAddrInfoEx")
#pragma comment(linker,"/export:FreeAddrInfoExW=_ws2_32_proxy_FreeAddrInfoExW")
#pragma comment(linker,"/export:FreeAddrInfoW=_ws2_32_proxy_FreeAddrInfoW")
#pragma comment(linker,"/export:GetAddrInfoExA=_ws2_32_proxy_GetAddrInfoExA")
#pragma comment(linker,"/export:GetAddrInfoExCancel=_ws2_32_proxy_GetAddrInfoExCancel")
#pragma comment(linker,"/export:GetAddrInfoExOverlappedResult=_ws2_32_proxy_GetAddrInfoExOverlappedResult")
#pragma comment(linker,"/export:GetAddrInfoExW=_ws2_32_proxy_GetAddrInfoExW")
#pragma comment(linker,"/export:GetAddrInfoW=_ws2_32_proxy_GetAddrInfoW")
#pragma comment(linker,"/export:GetHostNameW=_ws2_32_proxy_GetHostNameW")
#pragma comment(linker,"/export:GetNameInfoW=_ws2_32_proxy_GetNameInfoW")
#pragma comment(linker,"/export:InetNtopW=_ws2_32_proxy_InetNtopW")
#pragma comment(linker,"/export:InetPtonW=_ws2_32_proxy_InetPtonW")
#pragma comment(linker,"/export:SetAddrInfoExA=_ws2_32_proxy_SetAddrInfoExA")
#pragma comment(linker,"/export:SetAddrInfoExW=_ws2_32_proxy_SetAddrInfoExW")
#pragma comment(linker,"/export:WEP=_ws2_32_proxy_WEP")
#pragma comment(linker,"/export:WPUCompleteOverlappedRequest=_ws2_32_proxy_WPUCompleteOverlappedRequest")
#pragma comment(linker,"/export:WPUGetProviderPathEx=_ws2_32_proxy_WPUGetProviderPathEx")
#pragma comment(linker,"/export:WSAAccept=_ws2_32_proxy_WSAAccept")
#pragma comment(linker,"/export:WSAAddressToStringA=_ws2_32_proxy_WSAAddressToStringA")
#pragma comment(linker,"/export:WSAAddressToStringW=_ws2_32_proxy_WSAAddressToStringW")
#pragma comment(linker,"/export:WSAAdvertiseProvider=_ws2_32_proxy_WSAAdvertiseProvider")
#pragma comment(linker,"/export:WSAAsyncGetHostByAddr=_ws2_32_proxy_WSAAsyncGetHostByAddr")
#pragma comment(linker,"/export:WSAAsyncGetHostByName=_ws2_32_proxy_WSAAsyncGetHostByName")
#pragma comment(linker,"/export:WSAAsyncGetProtoByName=_ws2_32_proxy_WSAAsyncGetProtoByName")
#pragma comment(linker,"/export:WSAAsyncGetProtoByNumber=_ws2_32_proxy_WSAAsyncGetProtoByNumber")
#pragma comment(linker,"/export:WSAAsyncGetServByName=_ws2_32_proxy_WSAAsyncGetServByName")
#pragma comment(linker,"/export:WSAAsyncGetServByPort=_ws2_32_proxy_WSAAsyncGetServByPort")
#pragma comment(linker,"/export:WSAAsyncSelect=_ws2_32_proxy_WSAAsyncSelect")
#pragma comment(linker,"/export:WSACancelAsyncRequest=_ws2_32_proxy_WSACancelAsyncRequest")
#pragma comment(linker,"/export:WSACancelBlockingCall=_ws2_32_proxy_WSACancelBlockingCall")
#pragma comment(linker,"/export:WSACleanup=_ws2_32_proxy_WSACleanup")
#pragma comment(linker,"/export:WSACloseEvent=_ws2_32_proxy_WSACloseEvent")
#pragma comment(linker,"/export:WSAConnect=_ws2_32_proxy_WSAConnect")
#pragma comment(linker,"/export:WSAConnectByList=_ws2_32_proxy_WSAConnectByList")
#pragma comment(linker,"/export:WSAConnectByNameA=_ws2_32_proxy_WSAConnectByNameA")
#pragma comment(linker,"/export:WSAConnectByNameW=_ws2_32_proxy_WSAConnectByNameW")
#pragma comment(linker,"/export:WSACreateEvent=_ws2_32_proxy_WSACreateEvent")
#pragma comment(linker,"/export:WSADuplicateSocketA=_ws2_32_proxy_WSADuplicateSocketA")
#pragma comment(linker,"/export:WSADuplicateSocketW=_ws2_32_proxy_WSADuplicateSocketW")
#pragma comment(linker,"/export:WSAEnumNameSpaceProvidersA=_ws2_32_proxy_WSAEnumNameSpaceProvidersA")
#pragma comment(linker,"/export:WSAEnumNameSpaceProvidersExA=_ws2_32_proxy_WSAEnumNameSpaceProvidersExA")
#pragma comment(linker,"/export:WSAEnumNameSpaceProvidersExW=_ws2_32_proxy_WSAEnumNameSpaceProvidersExW")
#pragma comment(linker,"/export:WSAEnumNameSpaceProvidersW=_ws2_32_proxy_WSAEnumNameSpaceProvidersW")
#pragma comment(linker,"/export:WSAEnumNetworkEvents=_ws2_32_proxy_WSAEnumNetworkEvents")
#pragma comment(linker,"/export:WSAEnumProtocolsA=_ws2_32_proxy_WSAEnumProtocolsA")
#pragma comment(linker,"/export:WSAEnumProtocolsW=_ws2_32_proxy_WSAEnumProtocolsW")
#pragma comment(linker,"/export:WSAEventSelect=_ws2_32_proxy_WSAEventSelect")
#pragma comment(linker,"/export:WSAGetLastError=_ws2_32_proxy_WSAGetLastError")
#pragma comment(linker,"/export:WSAGetOverlappedResult=_ws2_32_proxy_WSAGetOverlappedResult")
#pragma comment(linker,"/export:WSAGetQOSByName=_ws2_32_proxy_WSAGetQOSByName")
#pragma comment(linker,"/export:WSAGetServiceClassInfoA=_ws2_32_proxy_WSAGetServiceClassInfoA")
#pragma comment(linker,"/export:WSAGetServiceClassInfoW=_ws2_32_proxy_WSAGetServiceClassInfoW")
#pragma comment(linker,"/export:WSAGetServiceClassNameByClassIdA=_ws2_32_proxy_WSAGetServiceClassNameByClassIdA")
#pragma comment(linker,"/export:WSAGetServiceClassNameByClassIdW=_ws2_32_proxy_WSAGetServiceClassNameByClassIdW")
#pragma comment(linker,"/export:WSAHtonl=_ws2_32_proxy_WSAHtonl")
#pragma comment(linker,"/export:WSAHtons=_ws2_32_proxy_WSAHtons")
#pragma comment(linker,"/export:WSAInstallServiceClassA=_ws2_32_proxy_WSAInstallServiceClassA")
#pragma comment(linker,"/export:WSAInstallServiceClassW=_ws2_32_proxy_WSAInstallServiceClassW")
#pragma comment(linker,"/export:WSAIoctl=_ws2_32_proxy_WSAIoctl")
#pragma comment(linker,"/export:WSAIsBlocking=_ws2_32_proxy_WSAIsBlocking")
#pragma comment(linker,"/export:WSAJoinLeaf=_ws2_32_proxy_WSAJoinLeaf")
#pragma comment(linker,"/export:WSALookupServiceBeginA=_ws2_32_proxy_WSALookupServiceBeginA")
#pragma comment(linker,"/export:WSALookupServiceBeginW=_ws2_32_proxy_WSALookupServiceBeginW")
#pragma comment(linker,"/export:WSALookupServiceEnd=_ws2_32_proxy_WSALookupServiceEnd")
#pragma comment(linker,"/export:WSALookupServiceNextA=_ws2_32_proxy_WSALookupServiceNextA")
#pragma comment(linker,"/export:WSALookupServiceNextW=_ws2_32_proxy_WSALookupServiceNextW")
#pragma comment(linker,"/export:WSANSPIoctl=_ws2_32_proxy_WSANSPIoctl")
#pragma comment(linker,"/export:WSANtohl=_ws2_32_proxy_WSANtohl")
#pragma comment(linker,"/export:WSANtohs=_ws2_32_proxy_WSANtohs")
#pragma comment(linker,"/export:WSAPoll=_ws2_32_proxy_WSAPoll")
#pragma comment(linker,"/export:WSAProviderCompleteAsyncCall=_ws2_32_proxy_WSAProviderCompleteAsyncCall")
#pragma comment(linker,"/export:WSAProviderConfigChange=_ws2_32_proxy_WSAProviderConfigChange")
#pragma comment(linker,"/export:WSARecv=_ws2_32_proxy_WSARecv")
#pragma comment(linker,"/export:WSARecvDisconnect=_ws2_32_proxy_WSARecvDisconnect")
#pragma comment(linker,"/export:WSARecvFrom=_ws2_32_proxy_WSARecvFrom")
#pragma comment(linker,"/export:WSARemoveServiceClass=_ws2_32_proxy_WSARemoveServiceClass")
#pragma comment(linker,"/export:WSAResetEvent=_ws2_32_proxy_WSAResetEvent")
#pragma comment(linker,"/export:WSASend=_ws2_32_proxy_WSASend")
#pragma comment(linker,"/export:WSASendDisconnect=_ws2_32_proxy_WSASendDisconnect")
#pragma comment(linker,"/export:WSASendMsg=_ws2_32_proxy_WSASendMsg")
#pragma comment(linker,"/export:WSASendTo=_ws2_32_proxy_WSASendTo")
#pragma comment(linker,"/export:WSASetBlockingHook=_ws2_32_proxy_WSASetBlockingHook")
#pragma comment(linker,"/export:WSASetEvent=_ws2_32_proxy_WSASetEvent")
#pragma comment(linker,"/export:WSASetLastError=_ws2_32_proxy_WSASetLastError")
#pragma comment(linker,"/export:WSASetServiceA=_ws2_32_proxy_WSASetServiceA")
#pragma comment(linker,"/export:WSASetServiceW=_ws2_32_proxy_WSASetServiceW")
#pragma comment(linker,"/export:WSASocketA=_ws2_32_proxy_WSASocketA")
#pragma comment(linker,"/export:WSASocketW=_ws2_32_proxy_WSASocketW")
#pragma comment(linker,"/export:WSAStartup=_ws2_32_proxy_WSAStartup")
#pragma comment(linker,"/export:WSAStringToAddressA=_ws2_32_proxy_WSAStringToAddressA")
#pragma comment(linker,"/export:WSAStringToAddressW=_ws2_32_proxy_WSAStringToAddressW")
#pragma comment(linker,"/export:WSAUnadvertiseProvider=_ws2_32_proxy_WSAUnadvertiseProvider")
#pragma comment(linker,"/export:WSAUnhookBlockingHook=_ws2_32_proxy_WSAUnhookBlockingHook")
#pragma comment(linker,"/export:WSAWaitForMultipleEvents=_ws2_32_proxy_WSAWaitForMultipleEvents")
#pragma comment(linker,"/export:WSApSetPostRoutine=_ws2_32_proxy_WSApSetPostRoutine")
#pragma comment(linker,"/export:WSCDeinstallProvider=_ws2_32_proxy_WSCDeinstallProvider")
#pragma comment(linker,"/export:WSCDeinstallProviderEx=_ws2_32_proxy_WSCDeinstallProviderEx")
#pragma comment(linker,"/export:WSCEnableNSProvider=_ws2_32_proxy_WSCEnableNSProvider")
#pragma comment(linker,"/export:WSCEnumProtocols=_ws2_32_proxy_WSCEnumProtocols")
#pragma comment(linker,"/export:WSCEnumProtocolsEx=_ws2_32_proxy_WSCEnumProtocolsEx")
#pragma comment(linker,"/export:WSCGetApplicationCategory=_ws2_32_proxy_WSCGetApplicationCategory")
#pragma comment(linker,"/export:WSCGetApplicationCategoryEx=_ws2_32_proxy_WSCGetApplicationCategoryEx")
#pragma comment(linker,"/export:WSCGetProviderInfo=_ws2_32_proxy_WSCGetProviderInfo")
#pragma comment(linker,"/export:WSCGetProviderPath=_ws2_32_proxy_WSCGetProviderPath")
#pragma comment(linker,"/export:WSCInstallNameSpace=_ws2_32_proxy_WSCInstallNameSpace")
#pragma comment(linker,"/export:WSCInstallNameSpaceEx=_ws2_32_proxy_WSCInstallNameSpaceEx")
#pragma comment(linker,"/export:WSCInstallNameSpaceEx2=_ws2_32_proxy_WSCInstallNameSpaceEx2")
#pragma comment(linker,"/export:WSCInstallProvider=_ws2_32_proxy_WSCInstallProvider")
#pragma comment(linker,"/export:WSCInstallProviderAndChains=_ws2_32_proxy_WSCInstallProviderAndChains")
#pragma comment(linker,"/export:WSCInstallProviderEx=_ws2_32_proxy_WSCInstallProviderEx")
#pragma comment(linker,"/export:WSCSetApplicationCategory=_ws2_32_proxy_WSCSetApplicationCategory")
#pragma comment(linker,"/export:WSCSetApplicationCategoryEx=_ws2_32_proxy_WSCSetApplicationCategoryEx")
#pragma comment(linker,"/export:WSCSetProviderInfo=_ws2_32_proxy_WSCSetProviderInfo")
#pragma comment(linker,"/export:WSCUnInstallNameSpace=_ws2_32_proxy_WSCUnInstallNameSpace")
#pragma comment(linker,"/export:WSCUnInstallNameSpaceEx2=_ws2_32_proxy_WSCUnInstallNameSpaceEx2")
#pragma comment(linker,"/export:WSCUpdateProvider=_ws2_32_proxy_WSCUpdateProvider")
#pragma comment(linker,"/export:WSCUpdateProviderEx=_ws2_32_proxy_WSCUpdateProviderEx")
#pragma comment(linker,"/export:WSCWriteNameSpaceOrder=_ws2_32_proxy_WSCWriteNameSpaceOrder")
#pragma comment(linker,"/export:WSCWriteProviderOrder=_ws2_32_proxy_WSCWriteProviderOrder")
#pragma comment(linker,"/export:WSCWriteProviderOrderEx=_ws2_32_proxy_WSCWriteProviderOrderEx")
#pragma comment(linker,"/export:WahCloseApcHelper=_ws2_32_proxy_WahCloseApcHelper")
#pragma comment(linker,"/export:WahCloseHandleHelper=_ws2_32_proxy_WahCloseHandleHelper")
#pragma comment(linker,"/export:WahCloseNotificationHandleHelper=_ws2_32_proxy_WahCloseNotificationHandleHelper")
#pragma comment(linker,"/export:WahCloseSocketHandle=_ws2_32_proxy_WahCloseSocketHandle")
#pragma comment(linker,"/export:WahCloseThread=_ws2_32_proxy_WahCloseThread")
#pragma comment(linker,"/export:WahCompleteRequest=_ws2_32_proxy_WahCompleteRequest")
#pragma comment(linker,"/export:WahCreateHandleContextTable=_ws2_32_proxy_WahCreateHandleContextTable")
#pragma comment(linker,"/export:WahCreateNotificationHandle=_ws2_32_proxy_WahCreateNotificationHandle")
#pragma comment(linker,"/export:WahCreateSocketHandle=_ws2_32_proxy_WahCreateSocketHandle")
#pragma comment(linker,"/export:WahDestroyHandleContextTable=_ws2_32_proxy_WahDestroyHandleContextTable")
#pragma comment(linker,"/export:WahDisableNonIFSHandleSupport=_ws2_32_proxy_WahDisableNonIFSHandleSupport")
#pragma comment(linker,"/export:WahEnableNonIFSHandleSupport=_ws2_32_proxy_WahEnableNonIFSHandleSupport")
#pragma comment(linker,"/export:WahEnumerateHandleContexts=_ws2_32_proxy_WahEnumerateHandleContexts")
#pragma comment(linker,"/export:WahInsertHandleContext=_ws2_32_proxy_WahInsertHandleContext")
#pragma comment(linker,"/export:WahNotifyAllProcesses=_ws2_32_proxy_WahNotifyAllProcesses")
#pragma comment(linker,"/export:WahOpenApcHelper=_ws2_32_proxy_WahOpenApcHelper")
#pragma comment(linker,"/export:WahOpenCurrentThread=_ws2_32_proxy_WahOpenCurrentThread")
#pragma comment(linker,"/export:WahOpenHandleHelper=_ws2_32_proxy_WahOpenHandleHelper")
#pragma comment(linker,"/export:WahOpenNotificationHandleHelper=_ws2_32_proxy_WahOpenNotificationHandleHelper")
#pragma comment(linker,"/export:WahQueueUserApc=_ws2_32_proxy_WahQueueUserApc")
#pragma comment(linker,"/export:WahReferenceContextByHandle=_ws2_32_proxy_WahReferenceContextByHandle")
#pragma comment(linker,"/export:WahRemoveHandleContext=_ws2_32_proxy_WahRemoveHandleContext")
#pragma comment(linker,"/export:WahWaitForNotification=_ws2_32_proxy_WahWaitForNotification")
#pragma comment(linker,"/export:WahWriteLSPEvent=_ws2_32_proxy_WahWriteLSPEvent")
#pragma comment(linker,"/export:__WSAFDIsSet=_ws2_32_proxy___WSAFDIsSet")
#pragma comment(linker,"/export:accept=_ws2_32_proxy_accept")
#pragma comment(linker,"/export:bind=_ws2_32_proxy_bind")
#pragma comment(linker,"/export:closesocket=_ws2_32_proxy_closesocket")
#pragma comment(linker,"/export:connect=_ws2_32_proxy_connect")
#pragma comment(linker,"/export:freeaddrinfo=_ws2_32_proxy_freeaddrinfo")
#pragma comment(linker,"/export:getaddrinfo=_ws2_32_proxy_getaddrinfo")
#pragma comment(linker,"/export:gethostbyaddr=_ws2_32_proxy_gethostbyaddr")
#pragma comment(linker,"/export:gethostbyname=_ws2_32_proxy_gethostbyname")
#pragma comment(linker,"/export:gethostname=_ws2_32_proxy_gethostname")
#pragma comment(linker,"/export:getnameinfo=_ws2_32_proxy_getnameinfo")
#pragma comment(linker,"/export:getpeername=_ws2_32_proxy_getpeername")
#pragma comment(linker,"/export:getprotobyname=_ws2_32_proxy_getprotobyname")
#pragma comment(linker,"/export:getprotobynumber=_ws2_32_proxy_getprotobynumber")
#pragma comment(linker,"/export:getservbyname=_ws2_32_proxy_getservbyname")
#pragma comment(linker,"/export:getservbyport=_ws2_32_proxy_getservbyport")
#pragma comment(linker,"/export:getsockname=_ws2_32_proxy_getsockname")
#pragma comment(linker,"/export:getsockopt=_ws2_32_proxy_getsockopt")
#pragma comment(linker,"/export:htonl=_ws2_32_proxy_htonl")
#pragma comment(linker,"/export:htons=_ws2_32_proxy_htons")
#pragma comment(linker,"/export:inet_addr=_ws2_32_proxy_inet_addr")
#pragma comment(linker,"/export:inet_ntoa=_ws2_32_proxy_inet_ntoa")
#pragma comment(linker,"/export:inet_ntop=_ws2_32_proxy_inet_ntop")
#pragma comment(linker,"/export:inet_pton=_ws2_32_proxy_inet_pton")
#pragma comment(linker,"/export:ioctlsocket=_ws2_32_proxy_ioctlsocket")
#pragma comment(linker,"/export:listen=_ws2_32_proxy_listen")
#pragma comment(linker,"/export:ntohl=_ws2_32_proxy_ntohl")
#pragma comment(linker,"/export:ntohs=_ws2_32_proxy_ntohs")
#pragma comment(linker,"/export:recv=_ws2_32_proxy_recv")
#pragma comment(linker,"/export:recvfrom=_ws2_32_proxy_recvfrom")
#pragma comment(linker,"/export:select=_ws2_32_proxy_select")
#pragma comment(linker,"/export:send=_ws2_32_proxy_send")
#pragma comment(linker,"/export:sendto=_ws2_32_proxy_sendto")
#pragma comment(linker,"/export:setsockopt=_ws2_32_proxy_setsockopt")
#pragma comment(linker,"/export:shutdown=_ws2_32_proxy_shutdown")
#pragma comment(linker,"/export:socket=_ws2_32_proxy_socket")
