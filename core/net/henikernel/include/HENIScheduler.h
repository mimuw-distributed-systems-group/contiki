/*
 * HENI: Hierarchical Embedded Network Infrastructure
 *
 * Copyright (c) 2016, University of Warsaw
 * All rights reserved.
 *
 */
#ifndef __HENI_SCHEDULER_H__
#define __HENI_SCHEDULER_H__

#include "HENIBase.h"

/**
 * @file
 * HENI: Scheduling of deferred computations in the HENI kernel.
 * @author Konrad Iwanicki <iwanicki@mimuw.edu.pl>
 */


/** A type holding the number of HENI kernel tasks. */
typedef uint_fast8_t   heni_kernel_task_count_t;



struct heni_kernel_task_scheduler_s;
/**
 * A scheduler for deferred computations,
 * so-called tasks, within a HENI kernel.
 */
typedef struct heni_kernel_task_scheduler_s   heni_kernel_task_scheduler_t;



/* ---------------------------------------------------------------------- *
 *                                                                        *
 *                         Generic functionality                          *
 *                                                                        *
 * ---------------------------------------------------------------------- */

/**
 * Initializes a scheduler for deferred computations,
 * so-called tasks, within a HENI kernel.
 * @param sched The scheduler.
 */
HENI_HID_FUNCT_DEC_PREFIX void heniKernelTaskSchedulerInit(
        heni_kernel_task_scheduler_t * sched
) HENI_HID_FUNCT_DEC_SUFFIX;

/**
 * Checks if any task has been scheduled
 * within a HENI kernel.
 * @param sched The scheduler.
 * @return Nonzero if there is a task scheduled,
 *   or zero otherwise.
 */
HENI_INL_FUNCT_DEC_PREFIX int_fast8_t heniKernelTaskSchedulerIsAnyScheduled(
        heni_kernel_task_scheduler_t const * sched
) HENI_INL_FUNCT_DEC_SUFFIX;

/**
 * Checks if a task with a given id has been scheduled
 * within a HENI kernel.
 * @param sched The scheduler.
 * @param taskId The identifier of the task.
 * @return Nonzero if the task with a given id has been
 *   scheduled, or zero otherwise.
 */
HENI_INL_FUNCT_DEC_PREFIX int_fast8_t heniKernelTaskSchedulerIsParticularScheduled(
        heni_kernel_task_scheduler_t const * sched,
        heni_kernel_task_count_t taskId
) HENI_INL_FUNCT_DEC_SUFFIX;

/**
 * Schedules a task with a given id within
 * a HENI kernel.
 * @param sched The scheduler.
 * @param taskId The identifier of the task.
 * @return Zero if the task has already been
 *  scheduled, or nonzero otherwise.
 */
HENI_HID_FUNCT_DEC_PREFIX int_fast8_t heniKernelTaskSchedulerAddTask(
        heni_kernel_task_scheduler_t * sched,
        heni_kernel_task_count_t taskId
) HENI_HID_FUNCT_DEC_SUFFIX;

/**
 * Extracts the next task that should be executed
 * within a HENI kernel. There must be a task
 * to be executed.
 * @param sched The scheduler.
 * @return The identifier of the task that should
 *   be executed.
 */
HENI_HID_FUNCT_DEC_PREFIX heni_kernel_task_count_t heniKernelTaskSchedulerExtractTask(
        heni_kernel_task_scheduler_t * sched
) HENI_HID_FUNCT_DEC_SUFFIX;

/**
 * Executes a given HENI kernel task.
 * @param sched The scheduler.
 * @param taskId The identifier of the task.
 */
HENI_HID_FUNCT_DEC_PREFIX void heniKernelTaskSchedulerExecuteTask(
        heni_kernel_task_scheduler_t * sched,
        heni_kernel_task_count_t taskId
) HENI_HID_FUNCT_DEC_SUFFIX;

/**
 * Cleans up a scheduler for deferred computations,
 * so-called tasks, within a HENI kernel.
 * @param sched The scheduler.
 */
HENI_INL_FUNCT_DEC_PREFIX void heniKernelTaskSchedulerCleanup(
        heni_kernel_task_scheduler_t * sched
) HENI_INL_FUNCT_DEC_SUFFIX;



/* ---------------------------------------------------------------------- *
 *                                                                        *
 *                            Particular tasks                            *
 *                                                                        *
 * ---------------------------------------------------------------------- */

/**
 * Maps computations represented by a given
 * HENI kernel task function to the corresponding
 * task number.
 * @param taskName The name of the task.
 */
#define HENI_KERNEL_TASK_GET_UID(taskName) (HENI_KERNEL_TASK_##taskName)

#include "detail/HENISchedulerDetail.h"

#endif /* __HENI_SCHEDULER_H__ */

