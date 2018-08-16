/*
 * HENI: Hierarchical Embedded Network Infrastructure
 *
 * Copyright (c) 2016, University of Warsaw
 * All rights reserved.
 *
 */
#ifndef __HENI_KERNEL_DETAIL_H__
#define __HENI_KERNEL_DETAIL_H__

#ifndef __HENI_KERNEL_H__
#error This is a file with implementation details. Do not include it directly!
#endif /* __HENI_KERNEL_H__ */

#include "HENILabel.h"


#define HENI_MAX_NUM_INSTANCE_FLAGS (((HENI_MAX_NUM_INSTANCES) + (sizeof(uint_fast8_t) << 3) - 1) / (sizeof(uint_fast8_t) << 3))

#define heniKernelIdxToFlagIdx(idx) ((idx) / (sizeof(uint_fast8_t) << 3))
#define heniKernelIdxToFlagOff(idx) ((idx) % (sizeof(uint_fast8_t) << 3))


#define heniKernelIIDToIdx(iid) ((iid) - 1)
#define heniKernelIdxToIID(idx) ((idx) + 1)



struct heni_instance_s
{
    heni_kernel_t *      ker;
    // FIXME: try combining everything here.
    heni_instance_id_t   iid;
    heni_lspec_t         lspec;
    uint8_t              logNumZoneDiscrBitsPlusOne; // 0,1,2,4,8,16,32,64 //0,1,2,3,4,5,6,7
};



typedef struct heni_kernel_instance_flags_s
{
    uint_fast8_t   running[HENI_MAX_NUM_INSTANCE_FLAGS];
    uint_fast8_t   stopping[HENI_MAX_NUM_INSTANCE_FLAGS];
} heni_kernel_instance_flags_t;



struct heni_kernel_s
{
    heni_kernel_task_scheduler_t   taskScheduler;
    heni_instance_t *              instancePtrs[HENI_MAX_NUM_INSTANCES];
    heni_kernel_instance_flags_t   instanceFlags;
    heni_instance_count_t          numRunningInstances;
    heni_instance_count_t          numStoppingInstances;
    /** Packets just passed to the HENI kernel for sending. */
    heni_linked_list_t             pktsToSend;
    /** Packets whose sending has completed that await only a signal to the user. */
    heni_linked_list_t             pktsAlreadySent;
    /** Packets marked by the HENI kernel as those to be received locally. */
    heni_linked_list_t             pktsToReceive;
    /** Packets being received, awaiting a finish notification. */
    heni_linked_list_t             pktsAlreadyReceived;
    /** Packets currently being routed. */
    heni_linked_list_t             pktsBeingRouted;  ///< FIXME: this is rather a temporary structure
};



/**
 * This is a private implementation function.
 *
 * It schedules for execution a HENI kernel task with
 * a given identifier.
 * @param ker The HENI kernel.
 * @param taskId The identifier of the task.
 */
HENI_HID_FUNCT_DEC_PREFIX int_fast8_t heniKernelScheduleTask(
        heni_kernel_t * ker,
        heni_kernel_task_count_t taskId
) HENI_HID_FUNCT_DEC_SUFFIX;




HENI_INL_FUNCT_DEF_PREFIX heni_kernel_task_scheduler_t * heniKernelAccessorsGetSchedulerForKernel(
        heni_kernel_t * ker
) HENI_INL_FUNCT_DEF_SUFFIX
{
    return &ker->taskScheduler;
}



HENI_INL_FUNCT_DEF_PREFIX heni_kernel_t * heniKernelAccessorsGetKernelForScheduler(
        heni_kernel_task_scheduler_t * sched
) HENI_INL_FUNCT_DEF_SUFFIX
{
    return (heni_kernel_t *)(((uint8_t *)sched) - offsetof(struct heni_kernel_s, taskScheduler));
}



/**
 * This is a private implementation function.
 *
 * Returns a HENI instance number for a packet
 * processed by a given HENI kernel.
 * @param ker The HENI kernel.
 * @param packet The packet.
 */
HENI_INL_FUNCT_DEF_PREFIX heni_instance_id_t heniKernelAccessorsGetIIDForPacket(
        heni_kernel_t const * ker,
        heni_packet_t const * packet
) HENI_INL_FUNCT_DEF_SUFFIX
{
    return heniPacketAddrGetInstanceID(&packet->paddr);
}



/**
 * This is a private implementation function.
 *
 * Clears all flags for a given HENI instance.
 * @param ker The HENI kernel.
 * @param iid The identifier of the instance.
 */
HENI_HID_FUNCT_DEC_PREFIX void heniKernelAccessorsInstanceClearAllFlags(
        heni_kernel_t * ker,
        heni_instance_id_t iid
) HENI_HID_FUNCT_DEC_SUFFIX;

/**
 * This is a private implementation function.
 *
 * Sets the running flag for a given HENI instance.
 * @param ker The HENI kernel.
 * @param iid The identifier of the instance.
 */
HENI_HID_FUNCT_DEC_PREFIX void heniKernelAccessorsInstanceFlagRunningSet(
        heni_kernel_t * ker,
        heni_instance_id_t iid
) HENI_HID_FUNCT_DEC_SUFFIX;

/**
 * This is a private implementation function.
 *
 * Clears the running flag for a given HENI instance.
 * @param ker The HENI kernel.
 * @param iid The identifier of the instance.
 */
HENI_HID_FUNCT_DEC_PREFIX void heniKernelAccessorsInstanceFlagRunningClear(
        heni_kernel_t * ker,
        heni_instance_id_t iid
) HENI_HID_FUNCT_DEC_SUFFIX;

/**
 * This is a private implementation function.
 *
 * Checks whether the running flag for a given
 * HENI instance is set.
 * @param ker The HENI kernel.
 * @param iid The identifier of the instance.
 * @return Nonzero if the flag is set or zero otherwise.
 */
HENI_HID_FUNCT_DEC_PREFIX int_fast8_t heniKernelAccessorsInstanceFlagRunningIsSet(
        heni_kernel_t const * ker,
        heni_instance_id_t iid
) HENI_HID_FUNCT_DEC_SUFFIX;

/**
 * This is a private implementation function.
 *
 * Sets the STOPPING flag for a given HENI instance.
 * @param ker The HENI kernel.
 * @param iid The identifier of the instance.
 */
HENI_HID_FUNCT_DEC_PREFIX void heniKernelAccessorsInstanceFlagStoppingSet(
        heni_kernel_t * ker,
        heni_instance_id_t iid
) HENI_HID_FUNCT_DEC_SUFFIX;

/**
 * This is a private implementation function.
 *
 * Clears the STOPPING flag for a given HENI instance.
 * @param ker The HENI kernel.
 * @param iid The identifier of the instance.
 */
HENI_HID_FUNCT_DEC_PREFIX void heniKernelAccessorsInstanceFlagStoppingClear(
        heni_kernel_t * ker,
        heni_instance_id_t iid
) HENI_HID_FUNCT_DEC_SUFFIX;

/**
 * This is a private implementation function.
 *
 * Checks whether the STOPPING flag for a given
 * HENI instance is set.
 * @param ker The HENI kernel.
 * @param iid The identifier of the instance.
 * @return Nonzero if the flag is set or zero otherwise.
 */
HENI_HID_FUNCT_DEC_PREFIX int_fast8_t heniKernelAccessorsInstanceFlagStoppingIsSet(
        heni_kernel_t const * ker,
        heni_instance_id_t iid
) HENI_HID_FUNCT_DEC_SUFFIX;

/**
 * This is a private implementation function.
 *
 * Creates a new HENI instance for
 * a given HENI kernel.
 * @param ker The HENI kernel.
 * @param iid The instance ID.
 * @return A pointer to the created instance or
 *   NULL if creating the instance has not been
 *   possible.
 */
HENI_HID_FUNCT_DEC_PREFIX heni_instance_t * heniKernelInstanceCreate(
        heni_kernel_t * ker,
        heni_instance_id_t iid
) HENI_HID_FUNCT_DEC_SUFFIX;

/**
 * This is a private implementation function.
 *
 * Starts a HENI instance.
 * @param inst The HENI instance.
 */
HENI_HID_FUNCT_DEC_PREFIX void heniKernelInstanceDestroy(
        heni_instance_t * inst
) HENI_HID_FUNCT_DEC_SUFFIX;

/**
 * This is a private implementation function.
 *
 * The function is signaled when a HENI instance is to
 * be started.
 * @param inst The instance to be started.
 * @return Zero on success or a negative value denoting the
 *   number of the initialization step that failed. In such
 *   a case, the instance will not be started.
 */
HENI_HID_FUNCT_DEC_PREFIX int_fast8_t heniKernelInstanceOnStart(
        heni_instance_t * inst
) HENI_HID_FUNCT_DEC_SUFFIX;

/**
 * This is a private implementation function.
 *
 * The function is signaled when a HENI instance is to
 * be stopped.
 * @param inst The instance to be stopped.
 */
HENI_HID_FUNCT_DEC_PREFIX void heniKernelInstanceOnStopTrigger(
        heni_instance_t * inst
) HENI_HID_FUNCT_DEC_SUFFIX;

/**
 * This is a private implementation function.
 *
 * The function is signaled when a HENI instance has
 * stopped but before this fact is reported outside
 * the corresponding HENI kernel.
 * @param inst The instance to be stopped.
 */
HENI_HID_FUNCT_DEC_PREFIX void heniKernelInstanceOnStopDone(
        heni_instance_t * inst
) HENI_HID_FUNCT_DEC_SUFFIX;

/**
 * This is a private implementation function.
 *
 * Returns the HENI kernel within which
 * a given HENI instance operates.
 * @param inst The HENI instance.
 * @return ker The HENI kernel in which the instance
 *   operates.
 */
HENI_INL_FUNCT_DEC_PREFIX heni_kernel_t * heniKernelInstanceGetKernelForInstancePtr(
        heni_instance_t const * inst
) HENI_INL_FUNCT_DEC_SUFFIX;

/**
 * This is a private implementation function.
 *
 * Returns the specification for the labels used
 * in a given HENI instance.
 * @param inst The HENI instance.
 * @return The label specification.
 */
HENI_INL_FUNCT_DEC_PREFIX heni_lspec_t heniKernelInstanceGetLabelSpecForInstancePtr(
        heni_instance_t const * inst
) HENI_INL_FUNCT_DEC_SUFFIX;

/**
 * This is a private implementation function.
 *
 * Returns the number of hierarchy levels
 * in a given HENI instance.
 * @param inst The HENI instance.
 * @return The number of hierarchy levels.
 */
HENI_INL_FUNCT_DEC_PREFIX heni_level_t heniKernelInstanceGetNumLevelsForInstancePtr(
        heni_instance_t const * inst
) HENI_INL_FUNCT_DEC_SUFFIX;

/**
 * This is a private implementation function.
 *
 * Returns the number of bits for a local zone identifier (LID)
 * in a given HENI instance.
 * @param inst The HENI instance.
 * @return The number of bits for a local zone identifier.
 */
HENI_INL_FUNCT_DEC_PREFIX uint8_t heniKernelInstanceGetNumZoneLIDBitsForInstancePtr(
        heni_instance_t const * inst
) HENI_INL_FUNCT_DEC_SUFFIX;

/**
 * This is a private implementation function.
 *
 * Returns the number of bits in a zone discriminator.
 * @param inst The HENI instance.
 * @return The number of bits for a zone discriminator.
 */
HENI_INL_FUNCT_DEC_PREFIX uint8_t heniKernelInstanceGetNumZoneDiscrBitsForInstancePtr(
        heni_instance_t const * inst
) HENI_INL_FUNCT_DEC_SUFFIX;



HENI_INL_FUNCT_DEF_PREFIX heni_kernel_t * heniKernelInstanceGetKernelForInstancePtr(
        heni_instance_t const * inst
) HENI_INL_FUNCT_DEF_SUFFIX
{
    return inst->ker;
}



HENI_INL_FUNCT_DEF_PREFIX heni_lspec_t heniKernelInstanceGetLabelSpecForInstancePtr(
        heni_instance_t const * inst
) HENI_INL_FUNCT_DEF_SUFFIX
{
    return inst->lspec;
}



HENI_INL_FUNCT_DEF_PREFIX heni_level_t heniKernelInstanceGetNumLevelsForInstancePtr(
        heni_instance_t const * inst
) HENI_INL_FUNCT_DEF_SUFFIX
{
    return heniLabelSpecGetNumLevels(heniKernelInstanceGetLabelSpecForInstancePtr(inst));
}



HENI_INL_FUNCT_DEF_PREFIX uint8_t heniKernelInstanceGetNumZoneLIDBitsForInstancePtr(
        heni_instance_t const * inst
) HENI_INL_FUNCT_DEF_SUFFIX
{
    return heniLabelSpecGetZoneLIDWidth(heniKernelInstanceGetLabelSpecForInstancePtr(inst));
}



HENI_INL_FUNCT_DEF_PREFIX uint8_t heniKernelInstanceGetNumZoneDiscrBitsForInstancePtr(
        heni_instance_t const * inst
) HENI_INL_FUNCT_DEF_SUFFIX
{
    return inst->logNumZoneDiscrBitsPlusOne == 0 ? (uint8_t)0 : ((uint8_t)1 << (inst->logNumZoneDiscrBitsPlusOne - 1));
}


#endif /* __HENI_KERNEL_DETAIL_H__ */

