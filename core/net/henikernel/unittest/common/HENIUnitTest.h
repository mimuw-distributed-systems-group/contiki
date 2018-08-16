/*
 * HENI: Hierarchical Embedded Network Infrastructure
 *
 * Copyright (c) 2016, University of Warsaw
 * All rights reserved.
 *
 */
#ifndef __HENI_UNITTEST_UNIT_TEST_H__
#define __HENI_UNITTEST_UNIT_TEST_H__

#include "HENIBase.h"
#include "HENIError.h"
#include "HENIDebug.h"

/**
 * @file
 * HENI: Common types, marcos, and functions for unit testing.
 * @author Konrad Iwanicki <iwanicki@mimuw.edu.pl>
 */

/** Indicates a start of all tests. */
#define HENI_UT_START_TESTING() HENI_UT_START_TESTING_IMPL()

/** Indicates a successfull finish of all tests. */
#define HENI_UT_FINISH_TESTING() HENI_UT_FINISH_TESTING_IMPL()

/** Indicates a start of a test together with a message. */
#define HENI_UT_START_TEST(msg...) HENI_UT_START_TEST_IMPL(msg)

/** Indicates a successful finish of a test together with a message. */
#define HENI_UT_FINISH_TEST(msg...) HENI_UT_FINISH_TEST_IMPL(msg)

/** Synchronously runs a single test. */
#define HENI_UT_RUN_TEST(t) do { \
        HENI_UT_START_TEST(#t); \
        t(); \
        HENI_UT_FINISH_TEST(#t); \
    } while (0)


/* Text message printing macros. */
#define HENI_UT_LOGS() do { heniDebugStartMsg(); heniDebugContinueMsg("[UT] "); } while (0)
#define HENI_UT_LOGC(msg...) heniDebugContinueMsg(msg)
#define HENI_UT_LOGF(msg...) heniDebugFinishMsg()
#define HENI_UT_LOG(msg...) do { HENI_UT_LOGS(); HENI_UT_LOGC(msg); HENI_UT_LOGF(); } while (0)

/** Checks a given condition in a unit test. */
#define HENI_UT_CHECK(cond) do { if (!(cond)) { \
            HENI_UT_LOG("ERROR: Condition \"" #cond "\" failed in function %s (file %s, line %u)!\n", __FUNCTION__, __FILE__, __LINE__); \
            heniUnitTestFail(); \
        } \
    } while (0)

/**
 * Checks for equality between two values.
 * If function calls are passed instead of values,
 * it is assumed that the functions are idempotent.
 */
#define HENI_UT_CHECK_EQ(val, exp, pfmt) do { if ((val) != (exp)) { \
            HENI_UT_LOG("ERROR: Value \"" #val "\" = " pfmt " is not the expected \"" #exp "\" = " pfmt " in function %s (file %s, line %u)!\n", (val), (exp), __FUNCTION__, __FILE__, __LINE__); \
            heniUnitTestFail(); \
        } \
    } while (0)

/**
 * Checks for non-equality between two values.
 * If function calls are passed instead of values,
 * it is assumed that the functions are idempotent.
 */
#define HENI_UT_CHECK_NE(val, exp, pfmt) do { if ((val) == (exp)) { \
            HENI_UT_LOG("ERROR: Value \"" #val "\" = " pfmt " is the undesirable \"" #exp "\" = " pfmt " in function %s (file %s, line %u)!\n", (val), (exp), __FUNCTION__, __FILE__, __LINE__); \
            heniUnitTestFail(); \
        } \
    } while (0)

/**
 * Checks for equality between two pointer values.
 * If function calls are passed instead of values,
 * it is assumed that the functions are idempotent.
 */
#define HENI_UT_CHECK_PTR_EQ(val, exp) HENI_UT_CHECK_EQ(val, exp, "%p")

/**
 * Checks for nonequality between two pointer values.
 * If function calls are passed instead of values,
 * it is assumed that the functions are idempotent.
 */
#define HENI_UT_CHECK_PTR_NE(val, exp) HENI_UT_CHECK_NE(val, exp, "%p")

/**
 * Checks for equality between two size_t values.
 * If function calls are passed instead of values,
 * it is assumed that the functions are idempotent.
 */
#define HENI_UT_CHECK_SZ_EQ(val, exp) HENI_UT_CHECK_EQ(val, exp, "%zu")

/**
 * Checks for nonequality between two size_t values.
 * If function calls are passed instead of values,
 * it is assumed that the functions are idempotent.
 */
#define HENI_UT_CHECK_SZ_NE(val, exp) HENI_UT_CHECK_NE(val, exp, "%zu")


/**
 * Fails a HENI unit test.
 */
HENI_EXT_FUNCT_DEC_PREFIX void heniUnitTestFail() HENI_EXT_FUNCT_DEC_SUFFIX;

/**
 * This function should be defined in a unit test if
 * the test boils down to running all tests as synchronous
 * functions.
 */
HENI_EXT_FUNCT_DEC_PREFIX void heniUnitTestSynchronouslyRunAll() HENI_EXT_FUNCT_DEC_SUFFIX;


#include "HENIUnitTestPlatform.h"

#endif /* __HENI_UNITTEST_UNIT_TEST_H__ */

