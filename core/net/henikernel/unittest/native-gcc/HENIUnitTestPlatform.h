/*
 * HENI: Hierarchical Embedded Network Infrastructure
 *
 * Copyright (c) 2016, University of Warsaw
 * All rights reserved.
 *
 */
#ifndef __HENI_UNITTEST_UNIT_TEST_PLATFORM_H__
#define __HENI_UNITTEST_UNIT_TEST_PLATFORM_H__


#ifndef __HENI_UNITTEST_UNIT_TEST_H__
#error This is a file with implementation details. Do not include it directly!
#endif /* __HENI_LINKED_LIST_H__ */


#define HENI_UT_START_TESTING_IMPL() do { \
        HENI_UT_LOG("STARTED UNIT TESTING"); \
    } while (0)

#define HENI_UT_FINISH_TESTING_IMPL() do { \
        HENI_UT_LOG("SUCCESSFULLY FINISHED UNIT TESTING"); \
        exit(0); \
    } while (0)

#define HENI_UT_START_TEST_IMPL(msg...) do { \
        HENI_UT_LOGS(); \
        HENI_UT_LOGC("Started test: "); \
        HENI_UT_LOGC(msg); \
        HENI_UT_LOGF(); \
    } while (0)

#define HENI_UT_FINISH_TEST_IMPL(msg...) do { \
        HENI_UT_LOGS(); \
        HENI_UT_LOGC("Finished test: "); \
        HENI_UT_LOGC(msg); \
        HENI_UT_LOGF(); \
    } while (0)


#define HENI_UT_FUNCT_DEC_PREFIX HENI_PRV_FUNCT_DEC_PREFIX
#define HENI_UT_FUNCT_DEF_PREFIX HENI_PRV_FUNCT_DEF_PREFIX
#define HENI_UT_FUNCT_DEC_SUFFIX HENI_PRV_FUNCT_DEC_SUFFIX
#define HENI_UT_FUNCT_DEF_SUFFIX HENI_PRV_FUNCT_DEF_SUFFIX


#endif /* __HENI_UNITTEST_UNIT_TEST_PLATFORM_H__ */

