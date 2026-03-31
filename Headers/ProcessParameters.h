#ifndef PROCESS_H
#define PROCESS_H

#include <windows.h>

// Struct for our program of type Process
typedef struct {
    char appName[256];
    DWORD progID;
    HANDLE progHandle;
    BYTE* baseAddr;
} Process;

// Error struct for our program
typedef struct {
    
} Error;

// String of application; Struct of program
// changes struct progID, progHandle
// return 0 or 1 (false or true)
int getProcessHandle(Process *program);

// String of application; Struct of program
// changes struct baseAddr
// return 0 or 1 (false or true)
int getProcessModule(Process *program);

// Print out every value of our Process struct
void printStruct(Process *program);

#endif
