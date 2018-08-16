/*
 * HENI: Hierarchical Embedded Network Infrastructure
 *
 * Copyright (c) 2016, University of Warsaw
 * All rights reserved.
 *
 */
#include "HENIBase.h"
#include "HENIError.h"
#include "HENIKernel.h"
#include "HENIScheduler.h"



HENI_HID_FUNCT_DEF_PREFIX void heniKernelTaskSchedulerInit(
        heni_kernel_task_scheduler_t * sched
) HENI_HID_FUNCT_DEF_SUFFIX
{
    heni_kernel_task_count_t   taskId;
    sched->frontId = HENI_KERNEL_TASK_NULL;
    sched->backId = HENI_KERNEL_TASK_NULL;
    for (taskId = 0; taskId < HENI_KERNEL_TASK_COUNT; ++taskId)
    {
        sched->nextIds[taskId] = HENI_KERNEL_TASK_NULL;
    }
}



HENI_HID_FUNCT_DEF_PREFIX int_fast8_t heniKernelTaskSchedulerAddTask(
        heni_kernel_task_scheduler_t * sched,
        heni_kernel_task_count_t taskId
) HENI_HID_FUNCT_DEF_SUFFIX
{
    if (heniKernelTaskSchedulerIsParticularScheduled(sched, taskId))
    {
        return 0;
    }
    if (sched->backId == HENI_KERNEL_TASK_NULL)
    {
        sched->frontId = taskId;
    }
    else
    {
        sched->nextIds[sched->backId - 1] = taskId;
    }
    sched->backId = taskId;
    return 1;
}



HENI_HID_FUNCT_DEF_PREFIX heni_kernel_task_count_t heniKernelTaskSchedulerExtractTask(
        heni_kernel_task_scheduler_t * sched
) HENI_HID_FUNCT_DEF_SUFFIX
{
    heni_kernel_task_count_t   taskId;
    HENI_DASSERT(heniKernelTaskSchedulerIsAnyScheduled(sched));
    taskId = sched->frontId;
    sched->frontId = sched->nextIds[taskId - 1];
    if (sched->frontId == HENI_KERNEL_TASK_NULL)
    {
        sched->backId = HENI_KERNEL_TASK_NULL;
    }
    else
    {
        sched->nextIds[taskId - 1] = HENI_KERNEL_TASK_NULL;
    }
    return taskId;
}



HENI_HID_FUNCT_DEF_PREFIX void heniKernelTaskSchedulerExecuteTask(
        heni_kernel_task_scheduler_t * sched,
        heni_kernel_task_count_t taskId
) HENI_HID_FUNCT_DEF_SUFFIX
{
    heni_kernel_t * const ker = heniKernelAccessorsGetKernelForScheduler(sched);
    HENI_DASSERT(! heniKernelTaskSchedulerIsParticularScheduled(sched, taskId));
    switch (taskId)
    {
    /* ----------- All tasks must have their dispatches here. ----------- */
    HENI_KERNEL_TASK_DISPATCH(heniKernelControlTask);
    HENI_KERNEL_TASK_DISPATCH(heniKernelProcessOutgoingPacketsTask);
    HENI_KERNEL_TASK_DISPATCH(heniKernelProcessIncomingPacketsTask);
    HENI_KERNEL_TASK_DISPATCH(heniKernelRoutePacketsTask);
    /* ------------------------------------------------------------------ */
    default:
        HENI_PASSERTM(0, "An unrecognized task %lu", (long unsigned)taskId);
    }
}

