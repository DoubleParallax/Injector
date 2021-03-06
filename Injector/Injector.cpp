// JNIDemoInjector.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <Windows.h>
#include <iostream>

int main()
{
	char dll[MAX_PATH];
	DWORD pid;

	std::cout << "Enter the PID: ";
	std::cin >> pid;
	std::cout << "Enter the path to the DLL: ";
	std::cin >> dll;

	HANDLE hProcess = OpenProcess(PROCESS_VM_READ | PROCESS_VM_WRITE | PROCESS_VM_OPERATION | PROCESS_CREATE_THREAD, false, pid);
	LPVOID loadLibraryA = (LPVOID)GetProcAddress(GetModuleHandleA("kernel32.dll"), "LoadLibraryA");
	LPVOID loadPath = VirtualAllocEx(hProcess, NULL, strlen(dll), MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
	WriteProcessMemory(hProcess, loadPath, dll, strlen(dll), NULL);
	HANDLE thread = CreateRemoteThread(hProcess, NULL, NULL, (LPTHREAD_START_ROUTINE)loadLibraryA, loadPath, 0, NULL);
	WaitForSingleObject(thread, INFINITE);
	VirtualFreeEx(hProcess, loadPath, 0, MEM_RELEASE);
	CloseHandle(thread);
	CloseHandle(hProcess);

	system("pause");
	return 0;
}