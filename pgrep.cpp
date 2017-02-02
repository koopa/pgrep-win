#include <windows.h>
#include <stdio.h>
#include <string>
#include <tchar.h>
#include <psapi.h>
#include <atlstr.h>

// To ensure correct resolution of symbols, add Psapi.lib to TARGETLIBS
// and compile with -DPSAPI_VERSION=1

void PrintIDIfProcContainsTargetName(DWORD processID, TCHAR *target){
	TCHAR szProcessName[MAX_PATH] = TEXT("<unknown>"); //default if no access to process name
	HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, processID);
	if (NULL != hProcess){
		HMODULE hMod;
		DWORD cbNeeded;
		if (EnumProcessModules(hProcess, &hMod, sizeof(hMod), &cbNeeded))
			GetModuleBaseName(hProcess, hMod, szProcessName, sizeof(szProcessName) / sizeof(TCHAR));
	}
//	_tprintf(TEXT("checking %s -> %d\n"), szProcessName, _tcsstr(szProcessName, target));
	if (_tcsstr(szProcessName, target) != NULL) {
		_tprintf(TEXT("%u\n"), processID);
	}

	CloseHandle(hProcess);
}

int main(int argc, char *argv[]){
	if (argc < 2)
		return 2;

	TCHAR target[4096];
	USES_CONVERSION; // -> A2T
	_tcscpy_s(target, A2T(argv[1]));
	DWORD aProcesses[1024], cbNeeded, cProcesses;
	unsigned int i;

	if (!EnumProcesses(aProcesses, sizeof(aProcesses), &cbNeeded))
		return 1;

	cProcesses = cbNeeded / sizeof(DWORD);
	for (i = 0; i < cProcesses; i++) {
		if (aProcesses[i] != 0) {
			PrintIDIfProcContainsTargetName(aProcesses[i], target);
		}
	}
	return 0;
}

