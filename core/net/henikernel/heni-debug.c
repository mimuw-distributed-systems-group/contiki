
#include <ctype.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <strformat.h>
#include <string.h>
#include <strings.h>

void heniFatalCrash()
{
    uint *p = 0;
    printf("Terminating due to HENI's fatal error...\n\n");
    printf("%d\n", *p);
    while(1) {}
}



void heniFatalReportErrorID(unsigned id)
{
    printf("HENI FATAL ERROR CODE: %u\n", id);
}



void heniFatalReportErrorMsgStart()
{
    printf("HENI FATAL ERROR MSG: ");
}



void heniFatalReportErrorMsgContinue(
        char const * errMsg, ...
)
{
    va_list   args;
    va_start(args, errMsg);
    printf(errMsg, args);
    va_end(args);
}



void heniFatalReportErrorMsgFinish()
{
    printf("\n");
}