#include "headers.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define APP_NAME "My_Code.exe"

int main(int argc, char *argv[]) {

    if (argc >= 2) {
        Process prog = {
            .appName = APP_NAME,
            .progID = atoi(argv[1])
        };

        getProcessHandle(&prog);
        getProcessModule(&prog);
        printStruct(&prog);

        uint8_t buf;
        int valid = readAtAddr(&prog, (LPCVOID) 0x000000000000, &buf);
        printf("\n%02X\n", buf);
    }

    getchar();
    return 0;
}
