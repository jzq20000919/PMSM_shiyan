#include <stddef.h>
#include <stdio.h>
#include "Algorithm.h"

void rt_OneStep(void);
void rt_OneStep(void)
{
    static boolean_T OverrunFlag = false;
    if (OverrunFlag) {
        rtmSetErrorStatus(Algorithm_M, "Overrun");
        return;
    }

    OverrunFlag = true;
    Algorithm_step();
    OverrunFlag = false;
}

int_T main(int_T argc, const char *argv[])
{
    (void)(argc);
    (void)(argv);
    Algorithm_initialize();
    printf("Warning: The simulation will run forever. "
           "Generated ERT main won't simulate model step behavior. "
           "To change this behavior select the 'MAT-file logging' option.\n");
    fflush((NULL));
    while (rtmGetErrorStatus(Algorithm_M) == (NULL)) {
    }

    Algorithm_terminate();
    return 0;
}
