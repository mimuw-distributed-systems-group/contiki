/*
 * HENI: Hierarchical Embedded Network Infrastructure
 *
 * Copyright (c) 2016, University of Warsaw
 * All rights reserved.
 *
 */
#ifndef __HENI_ERROR_H__
#define __HENI_ERROR_H__

#include "HENIBase.h"

/**
 * @file
 * HENI: Error reporting functionality.
 * @author Konrad Iwanicki <iwanicki@mimuw.edu.pl>
 */

/**
 * Causes the system to crash immediately.
 */
#define HENI_CRASH do { heniFatalCrash(); } while (0)

/**
 * Causes the system to crash, but earlier
 * a unique identifier of the failure is
 * reported, that is, each failure has a different
 * identifier.
 */
#define HENI_PANIC do { unsigned const __heni_panicID = __COUNTER__; heniFatalReportErrorID(__heni_panicID); heniFatalCrash(); } while (0)

/**
 * Checks a given condition. If it does not hold
 * a user-supplied error message is reported and the system panics.
 * The check is present even in non-debug configurations.
 */
#define HENI_PASSERTM(cond, msg...) do { \
    if (!(cond)) { \
        unsigned const __heni_panicID = __COUNTER__; \
        heniFatalReportErrorMsgStart(); \
        heniFatalReportErrorMsgContinue("HENI ASSERTION %u VIOLATED [\"%s\", line %u]: ", __heni_panicID, __FILE__, __LINE__); \
        heniFatalReportErrorMsgContinue(msg); \
        heniFatalReportErrorMsgFinish(); \
        heniFatalReportErrorID(__heni_panicID); \
        heniFatalCrash(); \
    } \
} while (0)

/**
 * Checks a given condition. If it does not hold
 * a default error message is reported and the system panics.
 * The check is present even in non-debug configurations.
 */
#define HENI_PASSERT(cond) HENI_PASSERTM(cond, #cond)

#ifdef HENI_DEBUG
/**
 * Checks a given condition. If it does not hold
 * a user-supplied error message is reported and the system panics.
 * The check is removed in non-debug configurations.
 */
#define HENI_DASSERTM(cond, msg...) HENI_PASSERTM(cond, msg)
/**
 * Checks a given condition. If it does not hold
 * a default error message is reported and the system panics.
 * The check is removed in non-debug configurations.
 */
#define HENI_DASSERT(cond) HENI_PASSERT(cond)
#else
/**
 * Checks a given condition. If it does not hold
 * a user-supplied error message is reported and the system panics.
 * The check is removed in non-debug configurations.
 */
#define HENI_DASSERTM(cond, msg...)
/**
 * Checks a given condition. If it does not hold
 * a default error message is reported and the system panics.
 * The check is removed in non-debug configurations.
 */
#define HENI_DASSERT(cond)
#endif



/**
 * Causes the system to crash.
 */
HENI_EXT_FUNCT_DEC_PREFIX void heniFatalCrash() HENI_EXT_FUNCT_DEC_SUFFIX;


/**
 * Reports an error with a given ID.
 * @param errId The identifier of the error to report.
 */
HENI_EXT_FUNCT_DEC_PREFIX void heniFatalReportErrorID(unsigned id) HENI_EXT_FUNCT_DEC_SUFFIX;


/**
 * Starts reporting an error message.
 */
HENI_EXT_FUNCT_DEC_PREFIX void heniFatalReportErrorMsgStart() HENI_EXT_FUNCT_DEC_SUFFIX;


/**
 * Continues reporting an error message.
 * @param errMsg The error message to be reported.
 */
HENI_EXT_FUNCT_DEC_PREFIX void heniFatalReportErrorMsgContinue(char const * errMsg, ...) HENI_EXT_FUNCT_DEC_SUFFIX;


/**
 * Finishes reporting an error message.
 */
HENI_EXT_FUNCT_DEC_PREFIX void heniFatalReportErrorMsgFinish() HENI_EXT_FUNCT_DEC_SUFFIX;


#endif /* __HENI_ERROR_H__ */

