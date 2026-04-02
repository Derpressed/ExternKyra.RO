#include "headers.h"
#include <tlhelp32.h>
#include <windows.h>
#include <stdio.h>

int getProcessHandle(Process *program) {
    // false by default
    int valid = 0;
    
    // check if our struct has a pid (then use that instead of name)
    if (program->progID != 0) {
        program->progHandle = OpenProcess(DEFAULT_PROCESS_PERMS, FALSE, program->progID);
        if (program->progHandle && program->progHandle != NULL) {
            return 1;
        } else {
            return 0;
        }
    }
    
    // CreatToolhelp32Snapshot takes all processes
    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (snapshot == INVALID_HANDLE_VALUE) {
        return 0;
    }

    // PROCESSENTRY struct to store snapshot values
    PROCESSENTRY32 pe;
    pe.dwSize = sizeof(PROCESSENTRY32);

    // if first processExist then iterate through every process
    if (Process32First(snapshot, &pe)) {
        do {
            if (strcmp(program -> appName, pe.szExeFile) == 0) {
                program -> progID = pe.th32ProcessID;
                printf("%lu", pe.th32ProcessID);
                // Use WindowsAPI OpenProcess to get processHandle
                program -> progHandle = OpenProcess (
                    PROCESS_VM_READ,
                    FALSE,
                    program -> progID
                );
                // make it so that processHandle returns valid
                valid = 1;
            }
        } while (Process32Next(snapshot, &pe));
    }
    CloseHandle(snapshot);
    return valid;
}

// Get the base address of our program (parameter is struct)
int getProcessModule(Process *program) {
    int valid = 0;
    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, program -> progID);
    if (snapshot == INVALID_HANDLE_VALUE) {
        return 0;
    }
    MODULEENTRY32 me;
    me.dwSize = sizeof(MODULEENTRY32);

    if (Module32First(snapshot, &me)) {
        do {
            // if we find that our program with our PID equates to what we iterated and found
            if (program -> progID == me.th32ProcessID) {
                program -> baseAddr = me.modBaseAddr;
            }
            valid = 1;
        } while (Module32Next(snapshot, &me));
    }

    CloseHandle(snapshot);
    return valid;

}

// Print values within struct
void printStruct(Process *program) {
    printf("\nApplication: %s\nPID: %lu\nHandle: %lu\nBase Address: %p", program->appName, program->progID, program->progHandle, program->baseAddr);
}
