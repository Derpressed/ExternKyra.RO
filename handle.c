#include <stdio.h>
#include <windows.h>
#include <tlhelp32.h>
#include <stdint.h>

// Get the HANDLE to process (by going through every process in PC)
int targetProcessHandle(char targetProcess[], HANDLE* procHandle, DWORD* pid) {
    int valid = 0;
    
    HANDLE snap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    PROCESSENTRY32 pe;

    memset(pe.szExeFile, 0, sizeof(pe.szExeFile));

    pe.dwSize = sizeof(PROCESSENTRY32);

    if (Process32First(snap, &pe)) {
        do {
            if (strcmp(targetProcess, pe.szExeFile) == 0) {
                *procHandle = OpenProcess(PROCESS_VM_READ, FALSE, pe.th32ProcessID);
                printf("\nFound process: %s\nPID: %lu", pe.szExeFile, pe.th32ProcessID);
                *pid = pe.th32ProcessID;
                CloseHandle(snap);
                return 1;
            }
        } while (Process32Next(snap, &pe));
    }


    CloseHandle(snap);
    return 0;
}

int GetModuleBaseAddress(char application[], DWORD pid, BYTE** baseAddr) {
    int valid = 0;
    HANDLE snap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE32 | TH32CS_SNAPMODULE, pid);
    MODULEENTRY32 me;

    me.dwSize = sizeof(MODULEENTRY32);
    if (Module32First(snap, &me)) {
        do {
            if (strcmp(application, me.szModule) == 0) {
                printf("\nProcess: %s\nBase Address: %p", me.szModule, me.modBaseAddr);
                *baseAddr = me.modBaseAddr;
            }
        } while (Module32Next(snap, &me));
    }

    CloseHandle(snap);
    return valid;
}



int main() {

    char program[] = "RobloxPlayerBeta.exe";


    DWORD pid = -1;
    HANDLE procHandle = NULL;
    BYTE* baseAddr = NULL;
    int handleValid = targetProcessHandle(program, &procHandle, &pid);
    int baseAddrValid = GetModuleBaseAddress(program, pid, &baseAddr);

    int value = 0;

    while (1) {
        if (ReadProcessMemory(procHandle, baseAddr + 0xec, &value, sizeof(value), NULL)) {
            printf("\nValue: %d", value);
        } else {
            printf("\nDidn't work");
        }

        Sleep(1000);
    }




    CloseHandle(procHandle);
    return 0;
}