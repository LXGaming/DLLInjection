#include "stdafx.h"
#include "Windows.h"
#include "TlHelp32.h"
#include "string"
#include "iostream"
#include "ConsoleOutput.h"
using namespace std;

bool InjectDynamicLibrary(DWORD pid, char* path) {
	ConsoleOutput::Info("Attempting to Inject DLL '" + string(path) + "' Into Process '" + to_string(pid) + "'.");

	HANDLE targetProcess = OpenProcess(PROCESS_ALL_ACCESS, false, pid);
	if (!targetProcess) {
		ConsoleOutput::Error("Specified process could not be found!");
		return false;
	}

	LPVOID loadLibAddr = (LPVOID)GetProcAddress(GetModuleHandleA("kernel32.dll"), "LoadLibraryA");
	if (!loadLibAddr) {
		ConsoleOutput::Error("LoadLibraryA function was not found inside kernel32.dll library!");
		return false;
	}

	LPVOID loadPath = VirtualAllocEx(targetProcess, NULL, strlen(path), MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
	//VirtualAllocEx(targetProcess, 0, strlen(path) + 1, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
	if (!loadPath) {
		ConsoleOutput::Error("Memory could not be allocated inside the chosen process!");
		return false;
	}

	BOOL loadMemory = WriteProcessMemory(targetProcess, loadPath, path, strlen(path), NULL);
	//BOOL LoadMemory = WriteProcessMemory(targetProcess, LoadPath, path, strlen(path) + 1, 0);
	if (!loadMemory) {
		ConsoleOutput::Error("Memory could not be written!");
		return false;
	}

	HANDLE remoteThread = CreateRemoteThread(targetProcess, NULL, NULL, (LPTHREAD_START_ROUTINE)loadLibAddr, loadPath, NULL, NULL);
	//HANDLE remoteThread = CreateRemoteThread(targetProcess, 0, 0, (LPTHREAD_START_ROUTINE)LoadLibAddr, LoadPath, 0, 0);
	if (!remoteThread) {
		ConsoleOutput::Error("Remote thread could not be created!");
		return false;
	}

	WaitForSingleObject(remoteThread, INFINITE);

	BOOL freeMemory = VirtualFreeEx(targetProcess, loadPath, strlen(path), MEM_RELEASE);
	//VirtualFreeEx(targetProcess, LoadPath, strlen(dllPath) + 1, MEM_RELEASE);

	CloseHandle(remoteThread);
	CloseHandle(targetProcess);
	return true;
}

DWORD FindProcessId(wstring processName) {
	ConsoleOutput::Info("Looking for process '" + string(processName.begin(), processName.end()) + "'.");

	PROCESSENTRY32 processInfo;
	processInfo.dwSize = sizeof(processInfo);

	HANDLE processSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
	if (processSnapshot == INVALID_HANDLE_VALUE) {
		return NULL;
	}

	Process32First(processSnapshot, &processInfo);
	if (!processName.compare(processInfo.szExeFile)) {
		CloseHandle(processSnapshot);
		return processInfo.th32ProcessID;
	}

	while (Process32Next(processSnapshot, &processInfo)) {
		if (!processName.compare(processInfo.szExeFile)) {
			CloseHandle(processSnapshot);
			return processInfo.th32ProcessID;
		}
	}

	CloseHandle(processSnapshot);
	return NULL;
}

int main() {
	ConsoleOutput::Info("DLLInjection.");
	ConsoleOutput::Info("Created by LX_Gaming.");

	int processId = FindProcessId(L"notepad.exe");
	char* dllPath = "TargetDll.dll";

	if (processId == 0) {
		ConsoleOutput::Error("Failed to find process!");
		return 0;
	}

	ConsoleOutput::Info("Succesfully found process.");
	InjectDynamicLibrary(processId, dllPath);
	return 0;
}