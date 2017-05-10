#include "stdafx.h"

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved) {

	if (ul_reason_for_call == DLL_PROCESS_ATTACH) {
		OutputDebugString(L"DLLInjection");
		MessageBoxA(0, "DLLInjection", "Successfully injected.", MB_ICONEXCLAMATION | MB_OK);
	}
	return TRUE;
}