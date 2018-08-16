/*
 * HENI: Hierarchical Embedded Network Infrastructure
 *
 * Copyright (c) 2016, University of Warsaw
 * All rights reserved.
 *
 */
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include "HENIUnitTest.h"


/**
 * Prints the stack trace.
 */
HENI_PRV_FUNCT_DEC_PREFIX void heniUnitTestStackTrace() HENI_PRV_FUNCT_DEC_SUFFIX;



HENI_EXT_FUNCT_DEC_PREFIX void heniUnitTestFail() HENI_EXT_FUNCT_DEC_SUFFIX
{
    exit(1);
}



HENI_EXT_FUNCT_DEF_PREFIX void heniDebugStartMsg() HENI_EXT_FUNCT_DEF_SUFFIX
{
}



HENI_EXT_FUNCT_DEF_PREFIX void heniDebugContinueMsg(char const * dbgMsg, ...) HENI_EXT_FUNCT_DEF_SUFFIX
{
    // va_list   args;
    // va_start(args, dbgMsg);
    // vprintf(dbgMsg, args);
    // va_end(args);
}



HENI_EXT_FUNCT_DEC_PREFIX void heniDebugFinishMsg() HENI_EXT_FUNCT_DEC_SUFFIX
{
    printf("\n");
}



HENI_EXT_FUNCT_DEF_PREFIX void heniFatalCrash() HENI_EXT_FUNCT_DEF_SUFFIX
{
    heniUnitTestStackTrace();
    exit(-1);
}



HENI_EXT_FUNCT_DEC_PREFIX void heniFatalReportErrorID(unsigned id) HENI_EXT_FUNCT_DEC_SUFFIX
{
    printf("[UT] FATAL ERROR ID: %u\n", id);
}



HENI_EXT_FUNCT_DEC_PREFIX void heniFatalReportErrorMsgStart() HENI_EXT_FUNCT_DEC_SUFFIX
{
    printf("[UT] FATAL ERROR MSG: ");
}


HENI_EXT_FUNCT_DEC_PREFIX void heniFatalReportErrorMsgContinue(char const * errMsg, ...) HENI_EXT_FUNCT_DEC_SUFFIX
{
    va_list   args;
    va_start(args, errMsg);
    vprintf(errMsg, args);
    va_end(args);
}



HENI_EXT_FUNCT_DEC_PREFIX void heniFatalReportErrorMsgFinish() HENI_EXT_FUNCT_DEC_SUFFIX
{
    printf("\n");
}



#ifdef __GNUC__
#include <execinfo.h>
enum
{
    HENI_UT_MAX_STACK_DEPTH = 1024,
};
#endif /* __GNUC__ */

HENI_PRV_FUNCT_DEF_PREFIX void heniUnitTestStackTrace() HENI_PRV_FUNCT_DEF_SUFFIX
{
#ifdef __GNUC__
    void * *   stackAddrs = (void * *)malloc(sizeof(void *) * HENI_UT_MAX_STACK_DEPTH);
    char * *   stackStrs;
    int        stackDepth;
    int        i;

    printf("[UT] FATAL ERROR BACKTRACE:\n");
    if (stackAddrs == NULL)
    {
        goto FAILURE_ROLLBACK_NO_MEM;
    }
    stackDepth = backtrace(stackAddrs, HENI_UT_MAX_STACK_DEPTH);
    if (stackDepth <= 0)
    {
        goto FAILURE_ROLLBACK_BACKTRACE_FAILED;
    }
    stackStrs = backtrace_symbols(stackAddrs, stackDepth);
    if (stackStrs == NULL)
    {
        goto FAILURE_ROLLBACK_SYMBOLS_FAILED;
    }
    for (i = 0; i < stackDepth; ++i)
    {
        printf("[UT]      %s\n", stackStrs[i]);
    }
    free(stackStrs);
FAILURE_ROLLBACK_SYMBOLS_FAILED:
FAILURE_ROLLBACK_BACKTRACE_FAILED:
    free(stackAddrs);
FAILURE_ROLLBACK_NO_MEM:
    return;
#endif /* __GNUC__ */
}

