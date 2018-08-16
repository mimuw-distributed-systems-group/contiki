/*
 * HENI: Hierarchical Embedded Network Infrastructure
 *
 * Copyright (c) 2016, University of Warsaw
 * All rights reserved.
 *
 */
#ifndef __HENI_SCHEDULER_DETAIL_H__
#define __HENI_SCHEDULER_DETAIL_H__

#ifndef __HENI_SCHEDULER_H__
#error This is a file with implementation details. Do not include it directly!
#endif /* __HENI_BASE_H__ */

#include "HENIError.h"


/**
 * Assigns a unique number to a HENI kernel task.
 * @param taskName The name of the task.
 */
#define HENI_KERNEL_TASK_ASSIGN_NUMBER(taskName) HENI_KERNEL_TASK_##taskName

/**
 * Dispatches computations of a HENI kernel
 * task to the function implementing the task.
 * @param taskName The name of the task.
 */
#define HENI_KERNEL_TASK_DISPATCH(taskName) \
        case HENI_KERNEL_TASK_GET_UID(taskName): \
            taskName(ker); \
            break;



/**
 * The possible deferred computations within
 * a HENI kernel.
 */
enum
{
    HENI_KERNEL_TASK_NULL = 0,
    /* ----- All tasks must have their unique numbers assigned here. ---- */
    HENI_KERNEL_TASK_ASSIGN_NUMBER(heniKernelControlTask),
    HENI_KERNEL_TASK_ASSIGN_NUMBER(heniKernelProcessOutgoingPacketsTask),
    HENI_KERNEL_TASK_ASSIGN_NUMBER(heniKernelProcessIncomingPacketsTask),
    HENI_KERNEL_TASK_ASSIGN_NUMBER(heniKernelRoutePacketsTask),
    /* ------------------------------------------------------------------ */
    HENI_KERNEL_TASK_LAST,
    HENI_KERNEL_TASK_COUNT = HENI_KERNEL_TASK_LAST - 1,
};


struct heni_kernel_task_scheduler_s
{
    heni_kernel_task_count_t   frontId;
    heni_kernel_task_count_t   backId;
    heni_kernel_task_count_t   nextIds[HENI_KERNEL_TASK_COUNT];
};



HENI_INL_FUNCT_DEC_PREFIX int_fast8_t heniKernelTaskSchedulerIsAnyScheduled(
        heni_kernel_task_scheduler_t const * sched
) HENI_INL_FUNCT_DEC_SUFFIX
{
    return sched->frontId != HENI_KERNEL_TASK_NULL ? 1 : 0;
}



HENI_INL_FUNCT_DEF_PREFIX int_fast8_t heniKernelTaskSchedulerIsParticularScheduled(
        heni_kernel_task_scheduler_t const * sched,
        heni_kernel_task_count_t taskId
) HENI_INL_FUNCT_DEF_SUFFIX
{
    HENI_DASSERT(taskId > HENI_KERNEL_TASK_NULL && taskId < HENI_KERNEL_TASK_LAST);
    return (sched->backId == taskId || sched->nextIds[taskId - 1] != HENI_KERNEL_TASK_NULL) ? 1 : 0;
}



HENI_INL_FUNCT_DEF_PREFIX void heniKernelTaskSchedulerCleanup(
        heni_kernel_task_scheduler_t * sched
) HENI_INL_FUNCT_DEF_SUFFIX
{
    HENI_DASSERT(sched->frontId == HENI_KERNEL_TASK_NULL && sched->backId == HENI_KERNEL_TASK_NULL);
}



#endif /* __HENI_SCHEDULER_DETAIL_H__ */

