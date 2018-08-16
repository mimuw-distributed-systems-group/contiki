#ifndef __LIBHENI_C_GUARD__
#define __LIBHENI_C_GUARD__
#line 1 "/home/gumol/heni-whip6/nesc/whip6/lib/henikernel/base/src/HENIFrame.c"
/*
 * HENI: Hierarchical Embedded Network Infrastructure
 *
 * Copyright (c) 2016, University of Warsaw
 * All rights reserved.
 *
 */
#include "HENIBase.h"
#include "HENIFrame.h"
#include "HENILinkAddress.h"


HENI_API_FUNCT_DEC_PREFIX void heniFrameAddrReset(
        heni_frame_addr_t * faddr
) HENI_API_FUNCT_DEC_SUFFIX
{
    heniLinkAddrInvalidate(heniFrameAddrGetDstLinkAddrPtr(faddr));
    heniLinkAddrInvalidate(heniFrameAddrGetSrcLinkAddrPtr(faddr));
}

#line 1 "/home/gumol/heni-whip6/nesc/whip6/lib/henikernel/base/src/HENIKernel.c"
/*
 * HENI: Hierarchical Embedded Network Infrastructure
 *
 * Copyright (c) 2016, University of Warsaw
 * All rights reserved.
 *
 */
#include "HENIBase.h"
#include "HENIFrame.h"
#include "HENIKernel.h"
#include "HENILinkAddress.h"
#include "HENIPacket.h"
#include "HENIScheduler.h"



/**
 * This is a private implementation function.
 *
 * Checks whether a given HENI instance has any
 * ongoing tasks that preclude stopping the
 * instance at a particular moment.
 * @param ker The HENI kernel.
 * @param iid The identifier of the instance.
 * @return Nonzero if the instance has any ongoing
 *   operations or zero otherwise.
 */
HENI_PRV_FUNCT_DEC_PREFIX int_fast8_t heniKernelHasInstanceAnyOngoingOperations(
        heni_kernel_t * ker,
        heni_instance_id_t iid
) HENI_PRV_FUNCT_DEC_SUFFIX;

/**
 * This is a private implementation function.
 *
 * Cancels ongoing operations, which can be canceled,
 * of an instance that is stopping. Note that only if
 * this function returns zero, is the caller guaranteed
 * that no more ongoing operations can be canceled.
 * @param ker The HENI kernel.
 * @param iid The identifier of the instance.
 * @return Nonzero if some operations have been canceled
 *   or zero otherwise.
 */
HENI_PRV_FUNCT_DEC_PREFIX int_fast8_t heniKernelTryCancelingOngoingOperations(
        heni_kernel_t * ker,
        heni_instance_id_t iid
) HENI_PRV_FUNCT_DEC_SUFFIX;

/**
 * This is a private implementation function.
 *
 * Handles the stop of a HENI instance.
 * @param ker The HENI kernel.
 * @param iid The identifier of the instance.
 */
HENI_PRV_FUNCT_DEC_PREFIX void heniKernelHandleInstanceStop(
        heni_kernel_t * ker,
        heni_instance_id_t iid
) HENI_PRV_FUNCT_DEC_SUFFIX;

/**
 * This is a private implementation function.
 *
 * Resets a newly allocated HENI packet.
 * @param packet The packet to reset.
 */
HENI_PRV_FUNCT_DEC_PREFIX void heniKernelResetFreshPacket(
        heni_packet_t * packet
) HENI_PRV_FUNCT_DEC_SUFFIX;

/**
 * This is a private implementation function.
 *
 * Marks a given packet as not sent because the
 * instance the packet is assigned to is stopping.
 * @param packet The packet to be marked.
 */
HENI_INL_FUNCT_DEC_PREFIX void heniKernelMarkOutgoingPacketAsNotSentDueToInstanceStop(
        heni_packet_t * packet
) HENI_INL_FUNCT_DEC_SUFFIX;

/**
 * This is a private implementation function.
 *
 * Marks a given packet as not sent because of a
 * failure when initiating packet routing.
 * @param packet The packet to be marked.
 * @param status The status to be set for the packet.
 */
HENI_INL_FUNCT_DEC_PREFIX void heniKernelMarkOutgoingPacketAsNotSentDueToFailureAtRoutingInitiation(
        heni_packet_t * packet,
        uint_fast8_t status
) HENI_INL_FUNCT_DEC_SUFFIX;

/**
 * This is a private implementation function.
 *
 * Marks a given packet as not sent because a
 * frame corresponding to the packet has not
 * been accepted by the lower layer.
 * @param packet The packet to be marked.
 */
HENI_INL_FUNCT_DEC_PREFIX void heniKernelMarkOutgoingPacketAsNotSentDueToLowerLayerFailureUponFrameForwarding(
        heni_packet_t * packet
) HENI_INL_FUNCT_DEC_SUFFIX;

/**
 * This is a private implementation function.
 *
 * Starts routing a given HENI packet.
 * @param ker The HENI kernel.
 * @param packet The packet to be routed.
 * @return Nonzero if routing the packet has been
 *   initiated successfully or zero otherwise.
 */
HENI_PRV_FUNCT_DEC_PREFIX uint_fast8_t heniKernelStartRoutingPacket(
        heni_kernel_t * ker,
        heni_packet_t * packet
) HENI_PRV_FUNCT_DEC_SUFFIX;


/**
 * This is a private implementation function.
 *
 * Checks if a given destination link-layer address of a
 * frame allows the present node to accept the frame.
 * @param faddr The address.
 * @return Nonzero if the present node can accept the
 *   frame or zero otherwise.
 */
HENI_PRV_FUNCT_DEC_PREFIX int_fast8_t heniKernelFrameReceivedCanBeAcceptedLocally(
        heni_frame_addr_t const * faddr
) HENI_PRV_FUNCT_DEC_SUFFIX;

/**
 * This is a private implementation function.
 *
 * Disposes of a packet that has already been received.
 * @param ker The HENI kernel.
 * @param packet The packet.
 */
HENI_PRV_FUNCT_DEC_PREFIX void heniKernelDisposeOfReceivedPacket(
        heni_kernel_t * ker,
        heni_packet_t * packet
) HENI_PRV_FUNCT_DEC_SUFFIX;



HENI_API_FUNCT_DEF_PREFIX int_fast8_t heniKernelInit(
        heni_kernel_t * ker
) HENI_API_FUNCT_DEF_SUFFIX
{
    heni_instance_count_t   icount;
    // FIXME: uncomment this if we have some failures
    /*int_fast8_t   step = 0;
    --step;*/
    heniKernelTaskSchedulerInit(&ker->taskScheduler);
    for (icount = 0; icount < HENI_MAX_NUM_INSTANCES; ++icount)
    {
        ker->instancePtrs[icount] = NULL;
    }
    for (icount = 0; icount < HENI_MAX_NUM_INSTANCE_FLAGS; ++icount)
    {
        ker->instanceFlags.running[icount] = 0;
        ker->instanceFlags.stopping[icount] = 0;
    }
    ker->numRunningInstances = 0;
    ker->numStoppingInstances = 0;
    heniLinkedListInit(&ker->pktsToSend);
    heniLinkedListInit(&ker->pktsAlreadySent);
    heniLinkedListInit(&ker->pktsToReceive);
    heniLinkedListInit(&ker->pktsAlreadyReceived);
    heniLinkedListInit(&ker->pktsBeingRouted);

    // FIXME: add more if necessary
    return 0;

    // FIXME: uncomment this if we have some failures
/*FAILURE_ROLLBACK_:
    heniKernelTaskSchedulerCleanup(
            heniKernelAccessorsGetSchedulerForKernel(ker)
    );
    return step;*/
}



HENI_API_FUNCT_DEF_PREFIX int_fast8_t heniKernelInstanceStart(
        heni_kernel_t * ker,
        heni_instance_id_t iid
) HENI_API_FUNCT_DEF_SUFFIX
{
    heni_instance_t *       inst;
    int_fast8_t             step = 0;
    --step;
    if (iid < HENI_INSTANCE_ID_MIN || iid > HENI_INSTANCE_ID_MAX)
    {
        /* An invalid instance identifier. */
        goto FAILURE_ROLLBACK_INVALID_IID;
    }
    --step;
    inst = ker->instancePtrs[heniKernelIIDToIdx(iid)];
    if (inst != NULL)
    {
        if (heniKernelAccessorsInstanceFlagRunningIsSet(ker, iid))
        {
            /* The instance is already running. */
            return 1;
        }
        /* The instance is likely stopping. (The system is busy.) */
        goto FAILURE_ROLLBACK_INSTANCE_ACTIVE;
    }
    --step;
    inst = heniKernelInstanceCreate(ker, iid);
    if (inst == NULL)
    {
        /* No memory to create the instance. */
        goto FAILURE_ROLLBACK_INSTANCE_CREATION_FAILED;
    }
    ker->instancePtrs[heniKernelIIDToIdx(iid)] = inst;
    ++ker->numRunningInstances;
    heniKernelAccessorsInstanceClearAllFlags(ker, iid);
    heniKernelAccessorsInstanceFlagRunningSet(ker, iid);
    --step;
    if (heniKernelInstanceOnStart(inst) < 0)
    {
        goto FAILURE_ROLLBACK_INSTANCE_START_FAILED;
    }
    return 0;
FAILURE_ROLLBACK_INSTANCE_START_FAILED:
    heniKernelAccessorsInstanceClearAllFlags(ker, iid);
    --ker->numRunningInstances;
    heniKernelInstanceDestroy(inst);
    ker->instancePtrs[heniKernelIIDToIdx(iid)] = NULL;
FAILURE_ROLLBACK_INSTANCE_CREATION_FAILED:
FAILURE_ROLLBACK_INSTANCE_ACTIVE:
FAILURE_ROLLBACK_INVALID_IID:
    return step;
}



HENI_API_FUNCT_DEF_PREFIX int_fast8_t heniKernelInstanceIsRunning(
        heni_kernel_t const * ker,
        heni_instance_id_t iid
) HENI_API_FUNCT_DEF_SUFFIX
{
    if (iid < HENI_INSTANCE_ID_MIN || iid > HENI_INSTANCE_ID_MAX)
    {
        return 0;
    }
    if (ker->instancePtrs[heniKernelIIDToIdx(iid)] == NULL)
    {
        return 0;
    }
    return heniKernelAccessorsInstanceFlagRunningIsSet(ker, iid);
}



HENI_API_FUNCT_DEF_PREFIX int_fast8_t heniKernelInstanceIsStopping(
        heni_kernel_t const * ker,
        heni_instance_id_t iid
) HENI_API_FUNCT_DEF_SUFFIX
{
    if (iid < HENI_INSTANCE_ID_MIN || iid > HENI_INSTANCE_ID_MAX)
    {
        return 0;
    }
    if (ker->instancePtrs[heniKernelIIDToIdx(iid)] == NULL)
    {
        return 0;
    }
    return heniKernelAccessorsInstanceFlagStoppingIsSet(ker, iid);
}



HENI_API_FUNCT_DEF_PREFIX void heniKernelInstanceStopTrigger(
        heni_kernel_t * ker,
        heni_instance_id_t iid
) HENI_API_FUNCT_DEF_SUFFIX
{
    heni_instance_t *       inst;

    HENI_DASSERT(iid >= HENI_INSTANCE_ID_MIN && iid <= HENI_INSTANCE_ID_MAX);
    inst = ker->instancePtrs[heniKernelIIDToIdx(iid)];
    if (inst == NULL)
    {
        return;
    }
    if (! heniKernelAccessorsInstanceFlagRunningIsSet(ker, iid))
    {
        return;
    }
    HENI_DASSERT(! heniKernelAccessorsInstanceFlagStoppingIsSet(ker, iid));
    heniKernelAccessorsInstanceFlagRunningClear(ker, iid);
    heniKernelAccessorsInstanceFlagStoppingSet(ker, iid);
    HENI_DASSERT(ker->numRunningInstances > 0);
    --ker->numRunningInstances;
    ++ker->numStoppingInstances;
    HENI_KERNEL_TASK_POST(ker, heniKernelControlTask);
    heniKernelInstanceOnStopTrigger(inst);
}



HENI_API_FUNCT_DEF_PREFIX void heniKernelInstanceStopAllTrigger(
        heni_kernel_t * ker
) HENI_API_FUNCT_DEF_SUFFIX
{
    heni_instance_count_t   icount;

    if (ker->numRunningInstances == 0)
    {
        return;
    }
    for (icount = 0; icount < HENI_MAX_NUM_INSTANCE_FLAGS; ++icount)
    {
        heni_instance_t *   inst;
        inst = ker->instancePtrs[icount];
        if (inst != NULL)
        {
            heniKernelInstanceStopTrigger(ker, heniKernelIdxToIID(icount));
        }
    }
}



HENI_HID_FUNCT_DEF_PREFIX void heniKernelControlTask(
        heni_kernel_t * ker
) HENI_HID_FUNCT_DEF_SUFFIX
{
    /* Check the cause for the task. */
    if (ker->numStoppingInstances > 0)
    {
        /* Some instances may have stopped. */
        heni_instance_count_t   icount;
        for (icount = 0; icount < HENI_MAX_NUM_INSTANCE_FLAGS; ++icount)
        {
            if (ker->instancePtrs[icount] != NULL)
            {
                heni_instance_id_t   iid = heniKernelIdxToIID(icount);
                if (heniKernelAccessorsInstanceFlagStoppingIsSet(ker, iid))
                {
                    /* The instance is stopping. */
                    if (heniKernelTryCancelingOngoingOperations(ker, iid) != 0)
                    {
                        /* Some operations have been canceled,     */
                        /* so let's stop for now and resume later. */
                        HENI_KERNEL_TASK_POST(ker, heniKernelControlTask);
                        return;
                    }
                    if (! heniKernelHasInstanceAnyOngoingOperations(ker, iid))
                    {
                        /* The instance has in fact stopped. */
                        heniKernelHandleInstanceStop(ker, iid);
                        return;
                    }
                }
            }
        }
    }
}



HENI_PRV_FUNCT_DEF_PREFIX int_fast8_t heniKernelTryCancelingOngoingOperations(
        heni_kernel_t * ker,
        heni_instance_id_t iid
) HENI_PRV_FUNCT_DEF_SUFFIX
{
    /* NOTICE iwanicki 2016-12-28:            */
    /* This may be implemented better, but    */
    /* the present implementation is correct. */
    return 0;
}



HENI_PRV_FUNCT_DEF_PREFIX int_fast8_t heniKernelHasInstanceAnyOngoingOperations(
        heni_kernel_t * ker,
        heni_instance_id_t iid
) HENI_PRV_FUNCT_DEF_SUFFIX
{
    /* FIXME iwanicki 2016-12-28:                */
    /* This implementation may starve stopping   */
    /* some instances, because it does not check */
    /* to which instance a packet belongs.       */
    /* We may also ignore some of the lists.     */
    return ! (heniLinkedListIsEmpty(&ker->pktsToSend) &&
            heniLinkedListIsEmpty(&ker->pktsAlreadySent) &&
            heniLinkedListIsEmpty(&ker->pktsToReceive) &&
            heniLinkedListIsEmpty(&ker->pktsAlreadyReceived) &&
            heniLinkedListIsEmpty(&ker->pktsBeingRouted));
}



HENI_PRV_FUNCT_DEF_PREFIX void heniKernelHandleInstanceStop(
        heni_kernel_t * ker,
        heni_instance_id_t iid
) HENI_PRV_FUNCT_DEF_SUFFIX
{
    heni_instance_t *       inst;

    HENI_DASSERT(iid >= HENI_INSTANCE_ID_MIN && iid <= HENI_INSTANCE_ID_MAX);
    inst = ker->instancePtrs[heniKernelIIDToIdx(iid)];
    HENI_DASSERT(inst != NULL);
    HENI_DASSERT(! heniKernelAccessorsInstanceFlagRunningIsSet(ker, iid));
    HENI_DASSERT(heniKernelAccessorsInstanceFlagStoppingIsSet(ker, iid));
    heniKernelAccessorsInstanceFlagStoppingClear(ker, iid);
    HENI_DASSERT(ker->numStoppingInstances > 0);
    --ker->numStoppingInstances;
    HENI_KERNEL_TASK_POST(ker, heniKernelControlTask);
    ker->instancePtrs[heniKernelIIDToIdx(iid)] = NULL;
    heniKernelInstanceOnStopDone(inst);
    heniKernelInstanceDestroy(inst);
    heniKernelInstanceStopDone(ker, iid);
    if (ker->numStoppingInstances == 0 && ker->numRunningInstances == 0)
    {
        heniKernelInstanceStopAllDone(ker);
    }
}



HENI_HID_FUNCT_DEF_PREFIX int_fast8_t heniKernelScheduleTask(
        heni_kernel_t * ker,
        heni_kernel_task_count_t taskId
) HENI_HID_FUNCT_DEF_SUFFIX
{
    heni_kernel_task_scheduler_t *   sched;
    int_fast8_t                      needPost;
    int_fast8_t                      taskSched;

    sched = heniKernelAccessorsGetSchedulerForKernel(ker);
    needPost = ! heniKernelTaskSchedulerIsAnyScheduled(sched);
    taskSched = heniKernelTaskSchedulerAddTask(sched, taskId);
    HENI_DASSERT(! needPost || taskSched);
    if (needPost)
    {
        heniKernelPostponeComputations(ker);
    }
    return taskSched;
}



HENI_API_FUNCT_DEF_PREFIX void heniKernelResumeComputations(
        heni_kernel_t * ker
) HENI_API_FUNCT_DEF_SUFFIX
{
    heni_kernel_task_count_t         taskId;
    int_fast8_t                      needRepost;

    HENI_DASSERT(heniKernelTaskSchedulerIsAnyScheduled(&ker->taskScheduler));
    if (! heniKernelTaskSchedulerIsAnyScheduled(&ker->taskScheduler))
    {
        return;
    }
    taskId = heniKernelTaskSchedulerExtractTask(&ker->taskScheduler);
    needRepost = heniKernelTaskSchedulerIsAnyScheduled(&ker->taskScheduler);
    heniKernelTaskSchedulerExecuteTask(&ker->taskScheduler, taskId);
    if (needRepost)
    {
        heniKernelPostponeComputations(ker);
    }
}



HENI_API_FUNCT_DEF_PREFIX void heniKernelCleanup(
        heni_kernel_t * ker
) HENI_API_FUNCT_DEF_SUFFIX
{
    HENI_DASSERT(ker->numRunningInstances == 0);
    HENI_DASSERT(ker->numStoppingInstances == 0);
    HENI_DASSERT(heniLinkedListIsEmpty(&ker->pktsToSend));
    HENI_DASSERT(heniLinkedListIsEmpty(&ker->pktsAlreadySent));
    HENI_DASSERT(heniLinkedListIsEmpty(&ker->pktsToReceive));
    HENI_DASSERT(heniLinkedListIsEmpty(&ker->pktsAlreadyReceived));
    HENI_DASSERT(heniLinkedListIsEmpty(&ker->pktsBeingRouted));
    heniKernelTaskSchedulerCleanup(&ker->taskScheduler);
}



HENI_API_FUNCT_DEF_PREFIX int_fast8_t heniPacketSendStart(
        heni_kernel_t * ker,
        heni_packet_addr_t const * paddr,
        heni_iobuf_list_t * ppld
) HENI_API_FUNCT_DEF_SUFFIX
{
    heni_packet_t *      packet;
    heni_instance_t *    inst;
    heni_instance_id_t   iid;
    int_fast8_t          status;
    iid = heniPacketAddrGetInstanceID(paddr);
    if (iid < HENI_INSTANCE_ID_MIN || iid > HENI_INSTANCE_ID_MAX)
    {
        status = HENI_PACKET_ROUTING_ERROR_INVALID_IID;
        goto FAILURE_ROLLBACK_INVALID_IID;
    }
    inst = ker->instancePtrs[heniKernelIIDToIdx(iid)];
    if (inst == NULL)
    {
        status = HENI_PACKET_ROUTING_ERROR_INSTANCE_NOT_RUNNING;
        goto FAILURE_ROLLBACK_INSTANCE_NONEXISTING;
    }
    if (! heniKernelAccessorsInstanceFlagRunningIsSet(ker, iid))
    {
        status = HENI_PACKET_ROUTING_ERROR_INSTANCE_NOT_RUNNING;
        goto FAILURE_ROLLBACK_INSTANCE_INACTIVE;
    }
    packet = heniPacketAlloc(ker, iid);
    if (packet == NULL)
    {
        status = HENI_PACKET_ROUTING_ERROR_OUT_OF_TOKENS;
        goto FAILURE_ROLLBACK_UNABLE_TO_ALLOCATE_PACKET;
    }
    heniKernelResetFreshPacket(packet);
    heniPacketTxInfoReset(&packet->opInfo.ptx);
    // FIXME iwanicki 2016-10-18:
    // Here, instead of copying, we may want
    // to already do the serialization.
    heniPacketAddrCopy(paddr, &packet->paddr);
    heniPacketAddrSetInstanceID(&packet->paddr, iid);
    packet->ppld = ppld;
    heniLinkAddrFetchMine(
            heniPacketGetLinkLayerSrcNeighborAddrPtr(packet)
    );
    heniLinkedListNodeAddBack(
        &ker->pktsToSend,
        heniPacketGetActiveListNodeForPacket(packet)
    );
    HENI_KERNEL_TASK_POST(ker, heniKernelProcessOutgoingPacketsTask);
    return HENI_PACKET_ROUTING_ERROR_NONE;

FAILURE_ROLLBACK_UNABLE_TO_ALLOCATE_PACKET:
FAILURE_ROLLBACK_INSTANCE_INACTIVE:
FAILURE_ROLLBACK_INSTANCE_NONEXISTING:
FAILURE_ROLLBACK_INVALID_IID:
    return -status;
}



HENI_HID_FUNCT_DEF_PREFIX void heniKernelProcessOutgoingPacketsTask(
        heni_kernel_t * ker
) HENI_HID_FUNCT_DEF_SUFFIX
{
    heni_packet_addr_t          paddr;
    heni_packet_tx_info_t       psts;
    heni_linked_list_node_t *   lnode;
    heni_packet_t *             packet;
    heni_instance_id_t          iid;
    uint8_t                     needsReposting = 0;
    uint8_t                     stoppingInstanceAffected = 0;

    lnode = heniLinkedListNodeTryRemoveFront(&ker->pktsToSend);
    if (lnode != NULL)
    {
        packet = heniPacketGetPacketForActiveListNode(lnode);
        iid = heniKernelAccessorsGetIIDForPacket(ker, packet);
        HENI_DASSERT(iid >= HENI_INSTANCE_ID_MIN && iid <= HENI_INSTANCE_ID_MAX);
        HENI_DASSERT(ker->instancePtrs[heniKernelIIDToIdx(iid)] != NULL);
        if (! heniKernelAccessorsInstanceFlagRunningIsSet(ker, iid))
        {
            heniKernelMarkOutgoingPacketAsNotSentDueToInstanceStop(packet);
            heniLinkedListNodeAddBack(&ker->pktsAlreadySent, lnode);
            /* NOTICE iwanicki 2016-10-20:   */
            /* The stopping instance is not  */
            /* really affected here.         */
            /* stoppingInstanceAffected = 1; */
            /* NOTICE iwanicki 2016-10-20:              */
            /* There is no need for reposting this task */
            /* because the code for handling completed  */
            /* packets is below.                        */
        }
        else
        {
            uint_fast8_t status = heniKernelStartRoutingPacket(ker, packet);
            if (status != 0)
            {
                heniKernelMarkOutgoingPacketAsNotSentDueToFailureAtRoutingInitiation(packet, status);
                heniLinkedListNodeAddBack(&ker->pktsAlreadySent, lnode);
                /* NOTICE iwanicki 2016-10-20:   */
                /* The stopping instance is not  */
                /* really affected here.         */
                /* stoppingInstanceAffected = 1; */
                /* NOTICE iwanicki 2016-10-20:              */
                /* There is no need for reposting this task */
                /* because the code for handling completed  */
                /* packets is below.                        */
            }
        }
        if (! heniLinkedListIsEmpty(&ker->pktsToSend))
        {
            needsReposting = 1;
        }
    }
    lnode = heniLinkedListNodeTryRemoveFront(&ker->pktsAlreadySent);
    if (lnode != NULL)
    {
        heni_iobuf_list_t *   payloadIOVPtr;

        packet = heniPacketGetPacketForActiveListNode(lnode);
        iid = heniKernelAccessorsGetIIDForPacket(ker, packet);
        payloadIOVPtr = heniPacketGetPayloadIOVectorPtr(packet);
        heniPacketAddrCopy(&packet->paddr, &paddr);
        heniPacketTxInfoCopy(&packet->opInfo.ptx, &psts);
        heniPacketFree(ker, iid, packet);
        if (! heniKernelAccessorsInstanceFlagRunningIsSet(ker, iid))
        {
            stoppingInstanceAffected = 1;
        }
        if (! heniLinkedListIsEmpty(&ker->pktsAlreadySent))
        {
            needsReposting = 1;
        }
        heniPacketSendFinish(ker, &paddr, payloadIOVPtr, &psts);
    }
    if (needsReposting)
    {
        HENI_KERNEL_TASK_POST(ker, heniKernelProcessOutgoingPacketsTask);
    }
    if (stoppingInstanceAffected)
    {
        HENI_KERNEL_TASK_POST(ker, heniKernelControlTask);
    }
}



HENI_INL_FUNCT_DEF_PREFIX void heniKernelMarkOutgoingPacketAsNotSentDueToInstanceStop(
        heni_packet_t * packet
) HENI_INL_FUNCT_DEF_SUFFIX
{
    heniPacketTxInfoSetError(
            &packet->opInfo.ptx,
            HENI_PACKET_ROUTING_ERROR_INSTANCE_NOT_RUNNING
    );
}



HENI_INL_FUNCT_DEF_PREFIX void heniKernelMarkOutgoingPacketAsNotSentDueToFailureAtRoutingInitiation(
        heni_packet_t * packet,
        uint_fast8_t status
) HENI_INL_FUNCT_DEF_SUFFIX
{
    HENI_DASSERT(((uint8_t)status) != HENI_PACKET_ROUTING_ERROR_NONE);
    heniPacketTxInfoSetError(
            &packet->opInfo.ptx,
            (uint8_t)status
    );
}



HENI_PRV_FUNCT_DEF_PREFIX uint_fast8_t heniKernelStartRoutingPacket(
        heni_kernel_t * ker,
        heni_packet_t * packet
) HENI_PRV_FUNCT_DEF_SUFFIX
{
    /* FIXME iwanicki 2016-10-25:              */
    /* This is a temporary implementation.     */
    /* It should be fixed as soon as possible. */
    heni_linked_list_node_t *   lnode;
    int_fast8_t                 needsAction;

    needsAction = heniLinkedListIsEmpty(&ker->pktsBeingRouted);
    lnode = heniPacketGetActiveListNodeForPacket(packet);
    heniLinkedListNodeAddBack(&ker->pktsBeingRouted, lnode);
    if (needsAction)
    {
        HENI_KERNEL_TASK_POST(ker, heniKernelRoutePacketsTask);
    }
    return 0;
}




HENI_HID_FUNCT_DEF_PREFIX void heniKernelRoutePacketsTask(
        heni_kernel_t * ker
) HENI_HID_FUNCT_DEF_SUFFIX
{
    /* FIXME iwanicki 2016-10-25:              */
    /* This is a temporary implementation.     */
    /* It should be fixed as soon as possible. */
    heni_frame_addr_t           faddr;
    heni_linked_list_node_t *   lnode;
    heni_packet_t *             packet;

    lnode = heniLinkedListNodeGetFront(&ker->pktsBeingRouted);
    HENI_PASSERT(lnode != NULL);
    packet = heniPacketGetPacketForActiveListNode(lnode);
    heniLinkAddrFetchMine(
            heniFrameAddrGetSrcLinkAddrPtr(&faddr)
    );
    heniLinkAddrFetchAllNeighbors(
            heniFrameAddrGetDstLinkAddrPtr(&faddr)
    );
    if (heniKernelFrameSendStart(ker, &faddr, heniPacketGetPayloadIOVectorPtr(packet)))
    {
        heniLinkedListNodeTryRemoveFront(&ker->pktsBeingRouted);
        heniKernelMarkOutgoingPacketAsNotSentDueToLowerLayerFailureUponFrameForwarding(packet);
        heniLinkedListNodeAddBack(&ker->pktsAlreadySent, lnode);
        HENI_KERNEL_TASK_POST(ker, heniKernelProcessOutgoingPacketsTask);
        if (! heniLinkedListIsEmpty(&ker->pktsBeingRouted))
        {
            HENI_KERNEL_TASK_POST(ker, heniKernelRoutePacketsTask);
        }
    }
}



HENI_PRV_FUNCT_DEF_PREFIX void heniKernelMarkOutgoingPacketAsNotSentDueToLowerLayerFailureUponFrameForwarding(
        heni_packet_t * packet
) HENI_PRV_FUNCT_DEF_SUFFIX
{
    heniPacketTxInfoSetError(
            &packet->opInfo.ptx,
            HENI_PACKET_ROUTING_ERROR_FRAME_FORWARDING_NOT_INITIATED
    );
}



HENI_API_FUNCT_DEF_PREFIX void heniKernelFrameSendFinish(
        heni_kernel_t * ker,
        heni_iobuf_list_t * fpld,
        heni_frame_tx_info_t const * fsts
) HENI_API_FUNCT_DEF_SUFFIX
{
    /* FIXME iwanicki 2016-10-25:              */
    /* This is a temporary implementation.     */
    /* It should be fixed as soon as possible. */
    /* In particular, it assumes that a packet */
    /* consists of just one frame.             */
    heni_linked_list_node_t *   lnode;
    heni_packet_t *             packet;

    lnode = heniLinkedListNodeTryRemoveFront(&ker->pktsBeingRouted);
    HENI_PASSERT(lnode != NULL);
    packet = heniPacketGetPacketForActiveListNode(lnode);
    HENI_PASSERT(fpld == heniPacketGetPayloadIOVectorPtr(packet));
    heniPacketTxInfoUpdateWithFrameTxInfo(
            &packet->opInfo.ptx,
            fsts
    );
    heniLinkedListNodeAddBack(&ker->pktsAlreadySent, lnode);
    HENI_KERNEL_TASK_POST(ker, heniKernelProcessOutgoingPacketsTask);
    /* NOTICE iwanicki 2016-10-27:                  */
    /* If the instance is stopping, the posted task */
    /* will behave accordingly.                     */
    if (! heniLinkedListIsEmpty(&ker->pktsBeingRouted))
    {
        HENI_KERNEL_TASK_POST(ker, heniKernelRoutePacketsTask);
    }
}



HENI_API_FUNCT_DEF_PREFIX int_fast8_t heniKernelFrameReceiveStart(
        heni_kernel_t * ker,
        heni_frame_addr_t const * faddr,
        heni_iobuf_list_t * fpld,
        heni_frame_rx_info_t const * fsts
) HENI_API_FUNCT_DEF_SUFFIX
{
    /* FIXME iwanicki 2016-11-22:              */
    /* This is a temporary implementation.     */
    /* It should be fixed as soon as possible. */
    /* In particular, it assumes that a packet */
    /* consists of just one frame.             */
    heni_instance_t *    inst;
    heni_packet_t *      packet;
    heni_instance_id_t   iid;
    int_fast8_t          status;

    if (! heniKernelFrameReceivedCanBeAcceptedLocally(faddr))
    {
        status = HENI_PACKET_ROUTING_ERROR_NOT_FOR_ME;
        goto FAILURE_ROLLBACK_FRAME_NOT_FOR_ME;
    }
    /* FIXME iwanicki 2016-11-22:                  */
    /* The instance identifier should be extracted */
    /* somehow from the frame itself.              */
    iid = HENI_INSTANCE_ID_MIN;
    if (iid < HENI_INSTANCE_ID_MIN || iid > HENI_INSTANCE_ID_MAX)
    {
        status = HENI_PACKET_ROUTING_ERROR_INVALID_IID;
        goto FAILURE_ROLLBACK_INVALID_IID;
    }
    inst = ker->instancePtrs[heniKernelIIDToIdx(iid)];
    if (inst == NULL)
    {
        status = HENI_PACKET_ROUTING_ERROR_INSTANCE_NOT_RUNNING;
        goto FAILURE_ROLLBACK_INSTANCE_NONEXISTING;
    }
    if (! heniKernelAccessorsInstanceFlagRunningIsSet(ker, iid))
    {
        status = HENI_PACKET_ROUTING_ERROR_INSTANCE_NOT_RUNNING;
        goto FAILURE_ROLLBACK_INSTANCE_INACTIVE;
    }
    /* FIXME iwanicki 2016-12-28:                  */
    /* Normally, a packet for the frame should be  */
    /* looked up.                                  */
    packet = heniPacketAlloc(ker, iid);
    if (packet == NULL)
    {
        status = HENI_PACKET_ROUTING_ERROR_OUT_OF_TOKENS;
        goto FAILURE_ROLLBACK_UNABLE_TO_ALLOCATE_PACKET;
    }
    heniKernelResetFreshPacket(packet);
    heniPacketRxInfoReset(&packet->opInfo.prx);
    heniLinkAddrCopy(
            heniFrameAddrGetSrcLinkAddrConstPtr(faddr),
            heniPacketGetLinkLayerSrcNeighborAddrPtr(packet)
    );
    heniPacketAddrSetInstanceID(&packet->paddr, iid);
    heniPacketRxInfoUpdateWithFrameRxInfo(&packet->opInfo.prx, fsts);
    /* FIXME iwanicki 2016-11-22:                    */
    /* These addresses are invalid.                  */
    /* The payload is not reassembled.               */
    packet->ppld = fpld;
    heniLinkedListNodeAddBack(
        &ker->pktsToReceive,
        heniPacketGetActiveListNodeForPacket(packet)
    );
    HENI_KERNEL_TASK_POST(ker, heniKernelProcessIncomingPacketsTask);
    return HENI_PACKET_ROUTING_ERROR_NONE;

FAILURE_ROLLBACK_UNABLE_TO_ALLOCATE_PACKET:
FAILURE_ROLLBACK_INSTANCE_INACTIVE:
FAILURE_ROLLBACK_INSTANCE_NONEXISTING:
FAILURE_ROLLBACK_INVALID_IID:
FAILURE_ROLLBACK_FRAME_NOT_FOR_ME:
    return -status;
}



HENI_PRV_FUNCT_DEF_PREFIX int_fast8_t heniKernelFrameReceivedCanBeAcceptedLocally(
        heni_frame_addr_t const * faddr
) HENI_PRV_FUNCT_DEF_SUFFIX
{
    uint8_t const * rawAddrPtr = heniFrameAddrGetDstLinkAddrConstPtr(faddr);
    return heniLinkAddrIsMine(rawAddrPtr) ||
            heniLinkAddrIsAllNeighbors(rawAddrPtr) ? 1 : 0;
}



HENI_HID_FUNCT_DEF_PREFIX void heniKernelProcessIncomingPacketsTask(
        heni_kernel_t * ker
) HENI_HID_FUNCT_DEF_SUFFIX
{
    heni_linked_list_node_t *   lnode;
    heni_packet_t *             packet;
    heni_instance_id_t          iid;
    uint8_t                     needsReposting = 0;
    uint8_t                     stoppingInstanceAffected = 0;

    lnode = heniLinkedListNodeTryRemoveFront(&ker->pktsToReceive);
    if (lnode != NULL)
    {
        packet = heniPacketGetPacketForActiveListNode(lnode);
        iid = heniKernelAccessorsGetIIDForPacket(ker, packet);
        HENI_DASSERT(iid >= HENI_INSTANCE_ID_MIN && iid <= HENI_INSTANCE_ID_MAX);
        HENI_DASSERT(ker->instancePtrs[heniKernelIIDToIdx(iid)] != NULL);
        if (! heniKernelAccessorsInstanceFlagRunningIsSet(ker, iid))
        {
            heniKernelDisposeOfReceivedPacket(ker, packet);
            if (heniLinkedListIsEmpty(&ker->pktsToReceive) &&
                    heniLinkedListIsEmpty(&ker->pktsAlreadyReceived))
            {
                stoppingInstanceAffected = 1;
            }
        }
        else
        {
            /* FIXME iwanicki 2016-11-22:              */
            /* Currently, there is no routing here.    */
            /* We should do it and signal reception    */
            /* only if the packet is for us.           */
            heniLinkedListNodeAddBack(&ker->pktsAlreadyReceived, lnode);
            if (heniPacketReceiveStart(ker, &packet->paddr, packet->ppld, &packet->opInfo.prx))
            {
                HENI_PASSERT(heniLinkedListNodeTryRemoveBack(&ker->pktsAlreadyReceived) == lnode);
                heniKernelDisposeOfReceivedPacket(ker, packet);
                if (! heniKernelAccessorsInstanceFlagRunningIsSet(ker, iid) &&
                        heniLinkedListIsEmpty(&ker->pktsToReceive) &&
                        heniLinkedListIsEmpty(&ker->pktsAlreadyReceived))
                {
                    stoppingInstanceAffected = 1;
                }
            }
            /* When the reception went fine, there is not much to do here. */
        }
        if (! heniLinkedListIsEmpty(&ker->pktsToReceive))
        {
            needsReposting = 1;
        }
    }
    if (needsReposting)
    {
        HENI_KERNEL_TASK_POST(ker, heniKernelProcessIncomingPacketsTask);
    }
    if (stoppingInstanceAffected)
    {
        HENI_KERNEL_TASK_POST(ker, heniKernelControlTask);
    }
}



HENI_PRV_FUNCT_DEF_PREFIX void heniKernelDisposeOfReceivedPacket(
        heni_kernel_t * ker,
        heni_packet_t * packet
) HENI_PRV_FUNCT_DEF_SUFFIX
{
    /* FIXME iwanicki 2016-11-22:              */
    /* This is a temporary implementation.     */
    /* It should be fixed as soon as possible. */
    /* The true implementation should check    */
    /* where the packet originated (i.e.,      */
    /* local send or network (based on the     */
    /* neighbors link-local address) and       */
    /* return it there (e.g., free up all      */
    /* frames).                                */
    heni_instance_id_t   iid;

    iid = heniKernelAccessorsGetIIDForPacket(ker, packet);
    HENI_DASSERT(iid >= HENI_INSTANCE_ID_MIN && iid <= HENI_INSTANCE_ID_MAX);
    HENI_DASSERT(ker->instancePtrs[heniKernelIIDToIdx(iid)] != NULL);
    heniKernelFrameReceiveFinish(ker, packet->ppld);
    packet->ppld = NULL;
    heniPacketFree(ker, iid, packet);
}



HENI_API_FUNCT_DEF_PREFIX void heniPacketReceiveFinish(
        heni_kernel_t * ker,
        heni_iobuf_list_t * ppld
) HENI_API_FUNCT_DEF_SUFFIX
{
    heni_linked_list_fiter_t   fiter;

    heniLinkedListFIterInit(&fiter, &ker->pktsAlreadyReceived);
    while (heniLinkedListFIterIsActive(&fiter))
    {
        heni_linked_list_node_t *   lnode;
        heni_packet_t *             packet;
        lnode = heniLinkedListFIterGetNode(&fiter);
        packet = heniPacketGetPacketForActiveListNode(lnode);
        if (packet->ppld == ppld)
        {
            heni_instance_id_t   iid;
            iid = heniKernelAccessorsGetIIDForPacket(ker, packet);
            HENI_DASSERT(iid >= HENI_INSTANCE_ID_MIN && iid <= HENI_INSTANCE_ID_MAX);
            HENI_DASSERT(ker->instancePtrs[heniKernelIIDToIdx(iid)] != NULL);
            heniLinkedListFIterRemoveAndAdvance(&fiter);
            heniKernelDisposeOfReceivedPacket(ker, packet);
            if (! heniKernelAccessorsInstanceFlagRunningIsSet(ker, iid) &&
                    heniLinkedListIsEmpty(&ker->pktsToReceive) &&
                    heniLinkedListIsEmpty(&ker->pktsAlreadyReceived))
            {
                HENI_KERNEL_TASK_POST(ker, heniKernelControlTask);
            }
            return;
        }
        else
        {
            heniLinkedListFIterAdvance(&fiter);
        }
    }
    HENI_PASSERT(0);
}



/* ---------------------------------------------------------------------- *
 *                                                                        *
 *                        Kernel object accessors                         *
 *                                                                        *
 * ---------------------------------------------------------------------- */

#define __heniKernelAccessorsInstanceFlagXXXReadWriteCommon(ker, iid) \
    heni_instance_count_t   icount; \
    heni_instance_count_t   flagIdx; \
    heni_instance_count_t   flagOff; \
    uint_fast8_t            mask; \
    HENI_DASSERT((iid) >= HENI_INSTANCE_ID_MIN && (iid) <= HENI_INSTANCE_ID_MAX); \
    icount = heniKernelIIDToIdx(iid); \
    flagIdx = heniKernelIdxToFlagIdx(icount); \
    flagOff = heniKernelIdxToFlagOff(icount); \
    mask = (uint_fast8_t)(((uint_fast8_t)1) << flagOff);



HENI_HID_FUNCT_DEF_PREFIX void heniKernelAccessorsInstanceClearAllFlags(
        heni_kernel_t * ker,
        heni_instance_id_t iid
) HENI_HID_FUNCT_DEF_SUFFIX
{
    __heniKernelAccessorsInstanceFlagXXXReadWriteCommon(ker, iid);
    mask = (uint_fast8_t)(~mask);
    ker->instanceFlags.running[flagIdx] &= mask;
    ker->instanceFlags.stopping[flagIdx] &= mask;
}



HENI_HID_FUNCT_DEF_PREFIX void heniKernelAccessorsInstanceFlagRunningSet(
        heni_kernel_t * ker,
        heni_instance_id_t iid
) HENI_HID_FUNCT_DEF_SUFFIX
{
    __heniKernelAccessorsInstanceFlagXXXReadWriteCommon(ker, iid);
    ker->instanceFlags.running[flagIdx] |= mask;
}



HENI_HID_FUNCT_DEF_PREFIX void heniKernelAccessorsInstanceFlagRunningClear(
        heni_kernel_t * ker,
        heni_instance_id_t iid
) HENI_HID_FUNCT_DEF_SUFFIX
{
    __heniKernelAccessorsInstanceFlagXXXReadWriteCommon(ker, iid);
    ker->instanceFlags.running[flagIdx] &= (uint_fast8_t)(~mask);
}



HENI_HID_FUNCT_DEF_PREFIX int_fast8_t heniKernelAccessorsInstanceFlagRunningIsSet(
        heni_kernel_t const * ker,
        heni_instance_id_t iid
) HENI_HID_FUNCT_DEF_SUFFIX
{
    __heniKernelAccessorsInstanceFlagXXXReadWriteCommon(ker, iid);
    return (ker->instanceFlags.running[flagIdx] & mask) != 0 ? 1 : 0;
}



HENI_HID_FUNCT_DEF_PREFIX void heniKernelAccessorsInstanceFlagStoppingSet(
        heni_kernel_t * ker,
        heni_instance_id_t iid
) HENI_HID_FUNCT_DEF_SUFFIX
{
    __heniKernelAccessorsInstanceFlagXXXReadWriteCommon(ker, iid);
    ker->instanceFlags.stopping[flagIdx] |= mask;
}



HENI_HID_FUNCT_DEF_PREFIX void heniKernelAccessorsInstanceFlagStoppingClear(
        heni_kernel_t * ker,
        heni_instance_id_t iid
) HENI_HID_FUNCT_DEF_SUFFIX
{
    __heniKernelAccessorsInstanceFlagXXXReadWriteCommon(ker, iid);
    ker->instanceFlags.stopping[flagIdx] &= (uint_fast8_t)(~mask);
}



HENI_HID_FUNCT_DEF_PREFIX int_fast8_t heniKernelAccessorsInstanceFlagStoppingIsSet(
        heni_kernel_t const * ker,
        heni_instance_id_t iid
) HENI_HID_FUNCT_DEF_SUFFIX
{
    __heniKernelAccessorsInstanceFlagXXXReadWriteCommon(ker, iid);
    return (ker->instanceFlags.stopping[flagIdx] & mask) != 0 ? 1 : 0;
}



HENI_PRV_FUNCT_DEF_PREFIX void heniKernelResetFreshPacket(
        heni_packet_t * packet
) HENI_PRV_FUNCT_DEF_SUFFIX
{
    /* packet->activeListNode; */
    heniPacketAddrReset(&packet->paddr);
    packet->ppld = NULL;
    heniLinkAddrInvalidate(heniPacketGetLinkLayerSrcNeighborAddrPtr(packet));
    /* packet->opInfo; */
}



HENI_HID_FUNCT_DEF_PREFIX heni_instance_t * heniKernelInstanceCreate(
        heni_kernel_t * ker,
        heni_instance_id_t iid
) HENI_HID_FUNCT_DEF_SUFFIX
{
    heni_instance_t *   inst;

    inst = heniKernelInstanceAlloc(ker, iid);
    if (inst == NULL)
    {
        goto FAILURE_ROLLBACK_ALLOC_FAILED;
    }
    inst->ker = ker;
    inst->iid = iid;
    /* FIXME iwanicki 2017-06-06:       */
    /* The spec should be configurable. */
    inst->lspec = 7;
    inst->logNumZoneDiscrBitsPlusOne = 7; /* a discriminator has 64 bits */
    return inst;
FAILURE_ROLLBACK_ALLOC_FAILED:
    return NULL;
}



HENI_HID_FUNCT_DEF_PREFIX void heniKernelInstanceDestroy(
        heni_instance_t * inst
) HENI_HID_FUNCT_DEF_SUFFIX
{
    HENI_DASSERT(inst != NULL);
    heniKernelInstanceFree(inst->ker, inst->iid, inst);
}



HENI_HID_FUNCT_DEF_PREFIX int_fast8_t heniKernelInstanceOnStart(
        heni_instance_t * inst
) HENI_HID_FUNCT_DEF_SUFFIX
{
    /* NOTICE iwanicki 2016-12-29:                   */
    /* Currently, there are no initialization steps. */
    return 0;
}



HENI_HID_FUNCT_DEF_PREFIX void heniKernelInstanceOnStopTrigger(
        heni_instance_t * inst
) HENI_HID_FUNCT_DEF_SUFFIX
{
    /* NOTICE iwanicki 2016-12-29:                   */
    /* Currently, there are no initialization steps. */
}



HENI_HID_FUNCT_DEF_PREFIX void heniKernelInstanceOnStopDone(
        heni_instance_t * inst
) HENI_HID_FUNCT_DEF_SUFFIX
{
    /* NOTICE iwanicki 2016-12-29:                   */
    /* Currently, there are no initialization steps. */
}
#line 1 "/home/gumol/heni-whip6/nesc/whip6/lib/henikernel/base/src/HENILabel.c"
/*
 * HENI: Hierarchical Embedded Network Infrastructure
 *
 * Copyright (c) 2016, University of Warsaw
 * All rights reserved.
 *
 */
#include "HENIBase.h"
#include "HENIError.h"
#include "HENILabel.h"



HENI_PRV_DATA_DEF_PREFIX heni_level_t const __const_heniLabelSpecNumLevels[HENI_LABEL_SPEC_NUM_VALUES] =
        {0,  1,  2,  1,  4,  2,  1,  8,  4,  2,  1, 16,  8,  4,  2,  1};
HENI_PRV_DATA_DEF_PREFIX uint8_t const __const_heniLabelSpecZoneLIDWidths[HENI_LABEL_SPEC_NUM_VALUES] =
        {0,  4,  4,  8,  4,  8, 16,  4,  8, 16, 32,  4,  8, 16, 32, 64};



HENI_API_FUNCT_DEF_PREFIX heni_zone_lid_t heniLabelSpecGetZoneLIDAll(
        heni_lspec_t labSpec
) HENI_API_FUNCT_DEF_SUFFIX
{
    switch (heniLabelSpecGetZoneLIDWidth(labSpec))
    {
#if ((HENI_ZONE_LID_MAX_BIT_SIZE) >= 4)
    case 4:
        return (heni_zone_lid_t)0x0fU;
#if ((HENI_ZONE_LID_MAX_BIT_SIZE) >= 8)
    case 8:
        return (heni_zone_lid_t)UINT8_MAX;
#if ((HENI_ZONE_LID_MAX_BIT_SIZE) >= 16)
    case 16:
        return (heni_zone_lid_t)UINT16_MAX;
#if ((HENI_ZONE_LID_MAX_BIT_SIZE) >= 32)
    case 32:
        return (heni_zone_lid_t)UINT32_MAX;
#if ((HENI_ZONE_LID_MAX_BIT_SIZE) >= 64)
    case 64:
        return (heni_zone_lid_t)UINT64_MAX;
#endif /* ((HENI_ZONE_LID_MAX_BIT_SIZE) >= 64) */
#endif /* ((HENI_ZONE_LID_MAX_BIT_SIZE) >= 32) */
#endif /* ((HENI_ZONE_LID_MAX_BIT_SIZE) >= 16) */
#endif /* ((HENI_ZONE_LID_MAX_BIT_SIZE) >= 8) */
#endif /* ((HENI_ZONE_LID_MAX_BIT_SIZE) >= 4) */
    default:
        HENI_PASSERT(0);
        return 0/*whatever*/;
    }
}



#if ((HENI_ZONE_LID_MAX_BIT_SIZE) >= 4)

HENI_INL_FUNCT_DEF_PREFIX uint8_t heniLabelGetElement4(
        uint8_t const * labPtr,
        heni_level_t i
) HENI_INL_FUNCT_DEF_SUFFIX
{
    uint8_t   byteIdx = (i >> 1);
    uint8_t   noMaskShift = (i & 1);
    uint8_t   byteVal = labPtr[byteIdx];
    return noMaskShift ? (byteVal & 0x0f) : (byteVal >> 4);
}



HENI_INL_FUNCT_DEF_PREFIX void heniLabelSetElement4(
        uint8_t * labPtr,
        heni_level_t i,
        uint8_t elem
) HENI_INL_FUNCT_DEF_SUFFIX
{
    uint8_t   byteIdx = (i >> 1);
    uint8_t   noMaskShift = (i & 1);
    uint8_t   byteVal = labPtr[byteIdx];
    if (noMaskShift)
    {
       byteVal &= 0xf0;
       byteVal |= ((uint8_t)elem & 0x0f);
    }
    else
    {
       byteVal &= 0x0f;
       byteVal |= ((uint8_t)elem << 4);
    }
    labPtr[byteIdx] = byteVal;
}



#if ((HENI_ZONE_LID_MAX_BIT_SIZE) >= 8)

HENI_INL_FUNCT_DEF_PREFIX uint8_t heniLabelGetElement8(
        uint8_t const * labPtr,
        heni_level_t i
) HENI_INL_FUNCT_DEF_SUFFIX
{
    return labPtr[i];
}



HENI_INL_FUNCT_DEF_PREFIX void heniLabelSetElement8(
        uint8_t * labPtr,
        heni_level_t i,
        uint8_t elem
) HENI_INL_FUNCT_DEF_SUFFIX
{
    labPtr[i] = elem;
}



#if ((HENI_ZONE_LID_MAX_BIT_SIZE) >= 16)

HENI_INL_FUNCT_DEF_PREFIX uint16_t heniLabelGetElement16(
        uint8_t const * labPtr,
        heni_level_t i
) HENI_INL_FUNCT_DEF_SUFFIX
{
    uint8_t byteIdx = (i << 1);
    return ((((uint16_t)labPtr[byteIdx + 0]) << 8) | (labPtr[byteIdx + 1]));
}



HENI_INL_FUNCT_DEF_PREFIX void heniLabelSetElement16(
        uint8_t * labPtr,
        heni_level_t i,
        uint16_t elem
) HENI_INL_FUNCT_DEF_SUFFIX
{
    uint8_t byteIdx = (i << 1);
    labPtr[byteIdx + 0] = (uint8_t)(elem >> 8);
    labPtr[byteIdx + 1] = (uint8_t)(elem);
}



#if ((HENI_ZONE_LID_MAX_BIT_SIZE) >= 32)

HENI_INL_FUNCT_DEF_PREFIX uint32_t heniLabelGetElement32(
        uint8_t const * labPtr,
        heni_level_t i
) HENI_INL_FUNCT_DEF_SUFFIX
{
    uint8_t byteIdx = (i << 2);
    return ((((uint32_t)labPtr[byteIdx + 0]) << 24) |
            (((uint32_t)labPtr[byteIdx + 1]) << 16) |
            (((uint32_t)labPtr[byteIdx + 2]) << 8) |
            (labPtr[byteIdx + 3]));
}



HENI_INL_FUNCT_DEF_PREFIX void heniLabelSetElement32(
        uint8_t * labPtr,
        heni_level_t i,
        uint32_t elem
) HENI_INL_FUNCT_DEF_SUFFIX
{
    uint8_t byteIdx = (i << 2);
    labPtr[byteIdx + 0] = (uint8_t)(elem >> 24);
    labPtr[byteIdx + 1] = (uint8_t)(elem >> 16);
    labPtr[byteIdx + 2] = (uint8_t)(elem >> 8);
    labPtr[byteIdx + 3] = (uint8_t)(elem);
}



#if ((HENI_ZONE_LID_MAX_BIT_SIZE) >= 64)

HENI_INL_FUNCT_DEF_PREFIX uint64_t heniLabelGetElement64(
        uint8_t const * labPtr,
        heni_level_t i
) HENI_INL_FUNCT_DEF_SUFFIX
{
    uint8_t byteIdx = (i << 3);
    return ((((uint64_t)labPtr[byteIdx + 0]) << 56) |
            (((uint64_t)labPtr[byteIdx + 1]) << 48) |
            (((uint64_t)labPtr[byteIdx + 2]) << 40) |
            (((uint64_t)labPtr[byteIdx + 3]) << 32) |
            (((uint64_t)labPtr[byteIdx + 4]) << 24) |
            (((uint64_t)labPtr[byteIdx + 5]) << 16) |
            (((uint64_t)labPtr[byteIdx + 6]) << 8) |
            (labPtr[byteIdx + 7]));
}



HENI_INL_FUNCT_DEF_PREFIX void heniLabelSetElement64(
        uint8_t * labPtr,
        heni_level_t i,
        uint64_t elem
) HENI_INL_FUNCT_DEF_SUFFIX
{
    uint8_t byteIdx = (i << 3);
    labPtr[byteIdx + 0] = (uint8_t)(elem >> 56);
    labPtr[byteIdx + 1] = (uint8_t)(elem >> 48);
    labPtr[byteIdx + 2] = (uint8_t)(elem >> 40);
    labPtr[byteIdx + 3] = (uint8_t)(elem >> 32);
    labPtr[byteIdx + 4] = (uint8_t)(elem >> 24);
    labPtr[byteIdx + 5] = (uint8_t)(elem >> 16);
    labPtr[byteIdx + 6] = (uint8_t)(elem >> 8);
    labPtr[byteIdx + 7] = (uint8_t)(elem);
}

#endif /* ((HENI_ZONE_LID_MAX_BIT_SIZE) >= 64) */
#endif /* ((HENI_ZONE_LID_MAX_BIT_SIZE) >= 32) */
#endif /* ((HENI_ZONE_LID_MAX_BIT_SIZE) >= 16) */
#endif /* ((HENI_ZONE_LID_MAX_BIT_SIZE) >= 8) */
#endif /* ((HENI_ZONE_LID_MAX_BIT_SIZE) >= 4) */



HENI_API_FUNCT_DEF_PREFIX heni_zone_lid_t heniLabelGetElement(
        uint8_t const * labPtr,
        heni_lspec_t labSpec,
        heni_level_t i
) HENI_API_FUNCT_DEF_SUFFIX
{
    if (i >= heniLabelSpecGetNumLevels(labSpec))
    {
        goto RETURN_INVALID_ELEMENT;
    }
    switch (heniLabelSpecGetZoneLIDWidth(labSpec))
    {
#if ((HENI_ZONE_LID_MAX_BIT_SIZE) >= 4)
    case 4:
        return heniLabelGetElement4(labPtr, i);
#if ((HENI_ZONE_LID_MAX_BIT_SIZE) >= 8)
    case 8:
        return heniLabelGetElement8(labPtr, i);
#if ((HENI_ZONE_LID_MAX_BIT_SIZE) >= 16)
    case 16:
        return heniLabelGetElement16(labPtr, i);
#if ((HENI_ZONE_LID_MAX_BIT_SIZE) >= 32)
    case 32:
        return heniLabelGetElement32(labPtr, i);
#if ((HENI_ZONE_LID_MAX_BIT_SIZE) >= 64)
    case 64:
        return heniLabelGetElement64(labPtr, i);
#endif /* ((HENI_ZONE_LID_MAX_BIT_SIZE) >= 64) */
#endif /* ((HENI_ZONE_LID_MAX_BIT_SIZE) >= 32) */
#endif /* ((HENI_ZONE_LID_MAX_BIT_SIZE) >= 16) */
#endif /* ((HENI_ZONE_LID_MAX_BIT_SIZE) >= 8) */
#endif /* ((HENI_ZONE_LID_MAX_BIT_SIZE) >= 4) */
    default:
        HENI_PASSERT(0);
        return 0/*whatever*/;
    }
RETURN_INVALID_ELEMENT:
    return 0;
}



HENI_API_FUNCT_DEF_PREFIX void heniLabelSetElement(
        uint8_t * labPtr,
        heni_lspec_t labSpec,
        heni_level_t i,
        heni_zone_lid_t lid
) HENI_API_FUNCT_DEF_SUFFIX
{
    if (i >= heniLabelSpecGetNumLevels(labSpec))
    {
        goto SET_ELEMENT_AT_INVALID_INDEX;
    }
    switch (heniLabelSpecGetZoneLIDWidth(labSpec))
    {
#if ((HENI_ZONE_LID_MAX_BIT_SIZE) >= 4)
    case 4:
        heniLabelSetElement4(labPtr, i, (uint8_t)lid);
        break;
#if ((HENI_ZONE_LID_MAX_BIT_SIZE) >= 8)
    case 8:
        heniLabelSetElement8(labPtr, i, (uint8_t)lid);
        break;
#if ((HENI_ZONE_LID_MAX_BIT_SIZE) >= 16)
    case 16:
        heniLabelSetElement16(labPtr, i, (uint16_t)lid);
        break;
#if ((HENI_ZONE_LID_MAX_BIT_SIZE) >= 32)
    case 32:
        heniLabelSetElement32(labPtr, i, (uint32_t)lid);
        break;
#if ((HENI_ZONE_LID_MAX_BIT_SIZE) >= 64)
    case 64:
        heniLabelSetElement64(labPtr, i, (uint64_t)lid);
        break;
#endif /* ((HENI_ZONE_LID_MAX_BIT_SIZE) >= 64) */
#endif /* ((HENI_ZONE_LID_MAX_BIT_SIZE) >= 32) */
#endif /* ((HENI_ZONE_LID_MAX_BIT_SIZE) >= 16) */
#endif /* ((HENI_ZONE_LID_MAX_BIT_SIZE) >= 8) */
#endif /* ((HENI_ZONE_LID_MAX_BIT_SIZE) >= 4) */
    default:
        HENI_PASSERT(0);
    }
    return;
SET_ELEMENT_AT_INVALID_INDEX:
    HENI_PASSERTM(0, "An invalid label element index, " PRIu8 "!", i);
}



HENI_API_FUNCT_DEF_PREFIX int_fast8_t heniLabelCmp(
        uint8_t const * labPtr1,
        uint8_t const * labPtr2,
        heni_lspec_t labSpec1,
        heni_lspec_t labSpec2
) HENI_API_FUNCT_DEF_SUFFIX
{
    int_fast8_t    res;
    heni_level_t   i, n;
    
    res = heniLabelSpecCmp(labSpec1, labSpec2);
    if (res != 0)
    {
        return res;
    }
    for (i = 0, n = heniLabelSpecGetNumLevels(labSpec1); i < n; ++i)
    {
        heni_zone_lid_t   lid1 = heniLabelGetElement(labPtr1, labSpec1, i);
        heni_zone_lid_t   lid2 = heniLabelGetElement(labPtr2, labSpec2, i);
        if (lid1 != lid2)
        {
            return lid1 < lid2 ? (int_fast8_t)-1 : (int_fast8_t)1;
        }
    }
    return 0;
}



#ifdef HENI_DEBUG

HENI_API_FUNCT_DEF_PREFIX void heniDebugContinueLabel(
        uint8_t const * labPtr,
        heni_lspec_t labSpec
) HENI_API_FUNCT_DEF_SUFFIX
{
    heni_level_t   i, n;

    HENI_DLOGC("|");
    n = heniLabelSpecGetNumLevels(labSpec);
    if (n > 0)
    {
        heni_zone_lid_t   lid = heniLabelGetElement(labPtr, labSpec, 0);
        HENI_DLOGC(HENI_PRI_ZONE_LID, lid);
        for (i = 1; i < n; ++i)
        {
            lid = heniLabelGetElement(labPtr, labSpec, i);
            HENI_DLOGC("|" HENI_PRI_ZONE_LID, lid);
        }
    }
    HENI_DLOGC("|");
}

#endif /* HENI_DEBUG */

#line 1 "/home/gumol/heni-whip6/nesc/whip6/lib/henikernel/base/src/HENILinkedList.c"
/*
 * HENI: Hierarchical Embedded Network Infrastructure
 *
 * Copyright (c) 2016, University of Warsaw
 * All rights reserved.
 *
 */
#include "HENIBase.h"
#include "HENIError.h"
#include "HENILinkedList.h"



HENI_API_FUNCT_DEF_PREFIX int_fast8_t heniLinkedListContainsNode(
        heni_linked_list_t * list,
        heni_linked_list_node_t * node
) HENI_API_FUNCT_DEF_SUFFIX
{
    heni_linked_list_fiter_t   fiter;
    heniLinkedListFIterInit(&fiter, list);
    while (heniLinkedListFIterIsActive(&fiter))
    {
        if (heniLinkedListFIterGetNode(&fiter) == node)
        {
            return 1;
        }
        heniLinkedListFIterAdvance(&fiter);
    }
    return 0;
}



HENI_API_FUNCT_DEF_PREFIX void heniLinkedListMerge(
        heni_linked_list_t * headList,
        heni_linked_list_t * tailList
) HENI_API_FUNCT_DEF_SUFFIX
{
    if (headList != tailList)
    {
        if (tailList->first != NULL)
        {
            tailList->first->prev = headList->last;
            if (headList->last != NULL)
            {
                headList->last->next = tailList->first;
            }
            else
            {
                headList->first = tailList->first;
            }
            headList->last = tailList->last;
            tailList->first = NULL;
            tailList->last = NULL;
        }
    }
}



HENI_HID_FUNCT_DEF_PREFIX void heniLinkedListNodeAddAfter(
        heni_linked_list_t * list,
        heni_linked_list_node_t * nodePrececingOrNull,
        heni_linked_list_node_t * nodeToAdd
) HENI_HID_FUNCT_DEF_SUFFIX
{
    HENI_DASSERT(list != NULL);
    HENI_DASSERT(nodeToAdd != NULL);
    /* HENI_DASSERT(nodeToAdd->next == NULL && nodeToAdd->prev == NULL); */

    nodeToAdd->prev = nodePrececingOrNull;
    if (nodePrececingOrNull != NULL)
    {
        nodeToAdd->next = nodePrececingOrNull->next;
        if (nodePrececingOrNull->next != NULL)
        {
            nodePrececingOrNull->next->prev = nodeToAdd;
        }
        else
        {
            list->last = nodeToAdd;
        }
        nodePrececingOrNull->next = nodeToAdd;
    }
    else
    {
        nodeToAdd->next = list->first;
        if (list->first != NULL)
        {
            list->first->prev = nodeToAdd;
        }
        else
        {
            list->last = nodeToAdd;
        }
        list->first = nodeToAdd;
    }
}



HENI_HID_FUNCT_DEF_PREFIX void heniLinkedListNodeAddBefore(
        heni_linked_list_t * list,
        heni_linked_list_node_t * nodeSucceedingOrNull,
        heni_linked_list_node_t * nodeToAdd
) HENI_HID_FUNCT_DEF_SUFFIX
{
    HENI_DASSERT(list != NULL);
    HENI_DASSERT(nodeToAdd != NULL);
    /* HENI_DASSERT(nodeToAdd->next == NULL && nodeToAdd->prev == NULL); */

    nodeToAdd->next = nodeSucceedingOrNull;
    if (nodeSucceedingOrNull != NULL)
    {
        nodeToAdd->prev = nodeSucceedingOrNull->prev;
        if (nodeSucceedingOrNull->prev != NULL)
        {
            nodeSucceedingOrNull->prev->next = nodeToAdd;
        }
        else
        {
            list->first = nodeToAdd;
        }
        nodeSucceedingOrNull->prev = nodeToAdd;
    }
    else
    {
        nodeToAdd->prev = list->last;
        if (list->last != NULL)
        {
            list->last->next = nodeToAdd;
        }
        else
        {
            list->first = nodeToAdd;
        }
        list->last = nodeToAdd;
    }
}



HENI_HID_FUNCT_DEF_PREFIX void heniLinkedListNodeRemove(
        heni_linked_list_t * list,
        heni_linked_list_node_t * nodeToRemove
) HENI_HID_FUNCT_DEF_SUFFIX
{
    heni_linked_list_node_t *   prevNode;
    heni_linked_list_node_t *   nextNode;

    HENI_DASSERT(list != NULL);
    HENI_DASSERT(nodeToRemove != NULL);

    prevNode = nodeToRemove->prev;
    nextNode = nodeToRemove->next;
    if (prevNode != NULL)
    {
        prevNode->next = nextNode;
        nodeToRemove->prev = NULL;
    }
    else
    {
        HENI_DASSERT(list->first == nodeToRemove);
        list->first = nextNode;
    }
    if (nextNode != NULL)
    {
        nextNode->prev = prevNode;
        nodeToRemove->next = NULL;
    }
    else
    {
        HENI_DASSERT(list->last == nodeToRemove);
        list->last = prevNode;
    }
}



HENI_HID_FUNCT_DEF_PREFIX void heniLinkedListSplitAfterNode(
        heni_linked_list_t * listToSplit,
        heni_linked_list_t * listAfterSplit,
        heni_linked_list_node_t * nodeForSplitOrNull
) HENI_HID_FUNCT_DEF_SUFFIX
{
    heni_linked_list_node_t *   nextNode;

    if (nodeForSplitOrNull == NULL)
    {
        listAfterSplit->first = listToSplit->first;
        listToSplit->first = NULL;
        listAfterSplit->last = listToSplit->last;
        listToSplit->last = NULL;
    }
    else
    {
        nextNode = nodeForSplitOrNull->next;
        listAfterSplit->first = nextNode;
        if (nextNode == NULL)
        {
            HENI_DASSERT(nodeForSplitOrNull == listToSplit->last);
            listAfterSplit->last = NULL;
        }
        else
        {
            listAfterSplit->last = listToSplit->last;
            listToSplit->last = nodeForSplitOrNull;
            nodeForSplitOrNull->next = NULL;
            nextNode->prev = NULL;
        }
    }
}



HENI_HID_FUNCT_DEF_PREFIX void heniLinkedListSplitBeforeNode(
        heni_linked_list_t * listToSplit,
        heni_linked_list_t * listAfterSplit,
        heni_linked_list_node_t * nodeForSplitOrNull
) HENI_HID_FUNCT_DEF_SUFFIX
{
    heni_linked_list_node_t *   prevNode;

    listAfterSplit->first = nodeForSplitOrNull;
    if (nodeForSplitOrNull == NULL)
    {
        listAfterSplit->last = NULL;
    }
    else
    {
        listAfterSplit->last = listToSplit->last;
        prevNode = nodeForSplitOrNull->prev;
        listToSplit->last = prevNode;
        if (prevNode == NULL)
        {
            HENI_DASSERT(nodeForSplitOrNull == listToSplit->first);
            listToSplit->first = NULL;
        }
        else
        {
            nodeForSplitOrNull->prev = NULL;
            prevNode->next = NULL;
        }
    }
}



HENI_API_FUNCT_DEF_PREFIX void heniLinkedListFIterSplitAfterAndAdvance(
        heni_linked_list_fiter_t * iter,
        heni_linked_list_t * tailList
) HENI_API_FUNCT_DEF_SUFFIX
{
    if (iter->currNode == NULL)
    {
        heniLinkedListInit(tailList);
    }
    else
    {
        heniLinkedListSplitAfterNode(iter->list, tailList, iter->currNode);
        iter->currNode = tailList->first;
    }
    iter->list = tailList;
}






HENI_API_FUNCT_DEF_PREFIX void heniLinkedListRIterSplitBeforeWithoutMoving(
        heni_linked_list_riter_t * iter,
        heni_linked_list_t * headList
) HENI_API_FUNCT_DEF_SUFFIX
{
    if (iter->currNode == NULL)
    {
        heniLinkedListInit(headList);
    }
    else
    {
        heniLinkedListSplitAfterNode(iter->list, headList, iter->currNode);
        heniLinkedListSwap(iter->list, headList);
    }
    iter->list = headList;
    HENI_DASSERT(iter->currNode == headList->last);
}



HENI_API_FUNCT_DEF_PREFIX void heniLinkedListRIterSplitAfterAndAdvance(
        heni_linked_list_riter_t * iter,
        heni_linked_list_t * headList
) HENI_API_FUNCT_DEF_SUFFIX
{
    heniLinkedListSplitBeforeNode(iter->list, headList, iter->currNode);
    if (iter->currNode != NULL)
    {
        heniLinkedListSwap(iter->list, headList);
        iter->currNode = headList->last;
    }
    iter->list = headList;
}



HENI_API_FUNCT_DEF_PREFIX void heniLinkedListFIterSetAfterMerge(
        heni_linked_list_fiter_t * iterOrNull,
        heni_linked_list_t * headList,
        heni_linked_list_t * tailList
) HENI_API_FUNCT_DEF_SUFFIX
{
    if (iterOrNull != NULL)
    {
        iterOrNull->list = headList;
        iterOrNull->currNode = headList->last;
    }
    heniLinkedListMerge(headList, tailList);
    heniLinkedListInit(tailList);
}



HENI_API_FUNCT_DEF_PREFIX void heniLinkedListRIterSetAfterMerge(
        heni_linked_list_riter_t * iterOrNull,
        heni_linked_list_t * headList,
        heni_linked_list_t * tailList
) HENI_API_FUNCT_DEF_SUFFIX
{
    if (iterOrNull != NULL)
    {
        iterOrNull->list = headList;
        iterOrNull->currNode = tailList->first;
    }
    heniLinkedListMerge(headList, tailList);
    heniLinkedListInit(tailList);
}

#line 1 "/home/gumol/heni-whip6/nesc/whip6/lib/henikernel/base/src/HENINeighborTable.c"
/*
 * HENI: Hierarchical Embedded Network Infrastructure
 *
 * Copyright (c) 2017, University of Warsaw
 * All rights reserved.
 *
 */
#include "HENINeighborTable.h"



HENI_HID_FUNCT_DEF_PREFIX int_fast8_t heniNeighborTableTIterInitAtOrBeforeNeighbor(
        heni_neighbor_table_titer_t * nbtti,
        heni_neighbor_table_t * nbt,
        uint8_t const * laddrPtr
) HENI_HID_FUNCT_DEF_SUFFIX
{
    int_fast8_t   cmpRes = 1;
    nbtti->nbt = nbt;
    nbtti->bucketIdx = heniNeighborTableGetBucketForLinkAddr(nbt, laddrPtr);
    heniLinkedListFIterInit(
            &nbtti->bucketIter,
            &(nbt->bucketsPtr[nbtti->bucketIdx].bucketList)
    );
    while (heniLinkedListFIterIsActive(&nbtti->bucketIter))
    {
        heni_neighbor_t const *   nbrTmp;
        uint8_t const *           laddrTmp;
        nbrTmp =
                heniNeighborEntryFromBucketListNode(
                        heniLinkedListFIterGetNode(&nbtti->bucketIter)
                );
        laddrTmp = heniNeighborEntryGetLinkAddrConstPtr(nbrTmp);
        cmpRes = heniLinkAddrCmp(laddrTmp, laddrPtr);
        if (cmpRes >= 0)
        {
            break;
        }
        heniLinkedListFIterAdvance(&nbtti->bucketIter);
    }
    return !cmpRes;
}



HENI_HID_FUNCT_DEC_PREFIX void heniNeighborTableTIterSeekNextNeighbor(
        heni_neighbor_table_titer_t * nbtti
) HENI_HID_FUNCT_DEC_SUFFIX
{
    /* All buckets have been tested. */
    if (nbtti->bucketIdx >= nbtti->nbt->bucketsCount)
    {
        return;
    }
    if (heniLinkedListFIterIsActive(&nbtti->bucketIter))
    {
        return;
    }
    for (++nbtti->bucketIdx; nbtti->bucketIdx < nbtti->nbt->bucketsCount; ++nbtti->bucketIdx)
    {
        heniLinkedListFIterInit(
                &nbtti->bucketIter,
                &(nbtti->nbt->bucketsPtr[nbtti->bucketIdx].bucketList)
        );
        if (heniLinkedListFIterIsActive(&nbtti->bucketIter))
        {
            return;
        }
    }
}



HENI_API_FUNCT_DEF_PREFIX void heniNeighborTableInit(
        heni_neighbor_table_t * nbt,
        heni_kernel_t * ker,
        heni_neighbor_table_elem_t * bufPtr,
        size_t bufLen
) HENI_API_FUNCT_DEF_SUFFIX
{
    size_t   bucketIdx;
    HENI_PASSERT(bufPtr != NULL && bufLen > 0);
    nbt->bucketsCount = bufLen;
    nbt->bucketsPtr = bufPtr;
    nbt->ker = ker;
    for (bucketIdx = 0; bucketIdx < bufLen; ++bucketIdx)
    {
        heniLinkedListInit(&(nbt->bucketsPtr[bucketIdx].bucketList));
    }
}



HENI_API_FUNCT_DEF_PREFIX void heniNeighborTableCleanup(
        heni_neighbor_table_t * nbt
) HENI_API_FUNCT_DEF_SUFFIX
{
    /* NOTICE iwanicki 2017-05-23:       */
    /* We do no additional checks here.  */
    nbt->ker = NULL;
    nbt->bucketsPtr = NULL;
    nbt->bucketsCount = 0;
}



HENI_API_FUNCT_DEF_PREFIX void heniNeighborTableAddNonexisting(
        heni_neighbor_table_t * nbt,
        uint8_t const * laddrPtr,
        heni_neighbor_t * nbr
) HENI_API_FUNCT_DEF_SUFFIX
{
    heni_neighbor_table_titer_t   nbtti;

    heniLinkAddrCopy(laddrPtr, heniNeighborEntryGetLinkAddrPtr(nbr));
    HENI_PASSERT(! heniNeighborTableTIterInitAtOrBeforeNeighbor(&nbtti, nbt, laddrPtr));
    heniLinkedListFIterAddBeforeWithoutMoving(
            &nbtti.bucketIter,
            heniNeighborEntryToBucketListNode(nbr)
    );
}



HENI_API_FUNCT_DEF_PREFIX heni_neighbor_t * heniNeighborTableFind(
        heni_neighbor_table_t * nbt,
        uint8_t const * laddrPtr
) HENI_API_FUNCT_DEF_SUFFIX
{
    heni_neighbor_table_titer_t   nbtti;
    if (heniNeighborTableTIterInitAtOrBeforeNeighbor(&nbtti, nbt, laddrPtr))
    {
        return heniNeighborTableTIterGetNeighbor(&nbtti);
    }
    return NULL;
}



HENI_API_FUNCT_DEF_PREFIX void heniNeighborTableRemoveExisting(
        heni_neighbor_table_t * nbt,
        uint8_t const * laddrPtr
) HENI_API_FUNCT_DEF_SUFFIX
{
    heni_neighbor_table_titer_t   nbtti;
    HENI_PASSERT(heniNeighborTableTIterInitAtOrBeforeNeighbor(&nbtti, nbt, laddrPtr));
    heniLinkedListFIterRemoveAndAdvance(&nbtti.bucketIter);
}


HENI_API_FUNCT_DEF_PREFIX void heniNeighborTableTIterInit(
        heni_neighbor_table_titer_t * nbtti,
        heni_neighbor_table_t * nbt
) HENI_API_FUNCT_DEF_SUFFIX
{
    nbtti->nbt = nbt;
    nbtti->bucketIdx = 0;
    if (nbtti->bucketIdx < nbtti->nbt->bucketsCount)
    {
        heniLinkedListFIterInit(
                &nbtti->bucketIter,
                &(nbtti->nbt->bucketsPtr[nbtti->bucketIdx].bucketList)
        );
        heniNeighborTableTIterSeekNextNeighbor(nbtti);
    }
}



HENI_API_FUNCT_DEF_PREFIX void heniNeighborTableTIterCopy(
        heni_neighbor_table_titer_t const * nbttiSrc,
        heni_neighbor_table_titer_t * nbttiDst
) HENI_API_FUNCT_DEF_SUFFIX
{
    nbttiDst->nbt = nbttiSrc->nbt;
    nbttiDst->bucketIdx = nbttiSrc->bucketIdx;
    heniLinkedListFIterCopy(&nbttiSrc->bucketIter, &nbttiDst->bucketIter);
}



HENI_API_FUNCT_DEF_PREFIX void heniNeighborTableTIterToPIter(
        heni_neighbor_table_titer_t const * nbtti,
        heni_neighbor_table_piter_t * nbtpi
) HENI_API_FUNCT_DEF_SUFFIX
{
    nbtpi->nbt = nbtti->nbt;
    if (heniNeighborTableTIterIsActive(nbtti))
    {
        heniLinkAddrCopy(
                heniNeighborEntryGetLinkAddrConstPtr(
                        heniNeighborTableTIterGetNeighbor(nbtti)
                ),
                &(nbtpi->currNeighborLinkAddr.data8[0])
        );
    }
    else
    {
        heniLinkAddrInvalidate(&(nbtpi->currNeighborLinkAddr.data8[0]));
    }
}



HENI_API_FUNCT_DEF_PREFIX void heniNeighborTablePIterToTIter(
        heni_neighbor_table_piter_t const * nbtpi,
        heni_neighbor_table_titer_t * nbtti
) HENI_API_FUNCT_DEF_SUFFIX
{
    if (heniLinkAddrIsValid(&(nbtpi->currNeighborLinkAddr.data8[0])))
    {
        heniNeighborTableTIterInitAtOrBeforeNeighbor(
                nbtti,
                nbtpi->nbt,
                &(nbtpi->currNeighborLinkAddr.data8[0])
        );
    }
    else
    {
        nbtti->nbt = nbtpi->nbt;
        nbtti->bucketIdx = nbtti->nbt->bucketsCount;
    }
}



HENI_API_FUNCT_DEF_PREFIX void heniNeighborTableTIterAdvance(
        heni_neighbor_table_titer_t * nbtti
) HENI_API_FUNCT_DEF_SUFFIX
{
    HENI_DASSERT(heniLinkedListFIterIsActive(&nbtti->bucketIter));
    heniLinkedListFIterAdvance(&nbtti->bucketIter);
    heniNeighborTableTIterSeekNextNeighbor(nbtti);
}



HENI_API_FUNCT_DEF_PREFIX void heniNeighborTableTIterRemoveAndAdvance(
        heni_neighbor_table_titer_t * nbtti
) HENI_API_FUNCT_DEF_SUFFIX
{
    HENI_DASSERT(heniLinkedListFIterIsActive(&nbtti->bucketIter));
    heniLinkedListFIterRemoveAndAdvance(&nbtti->bucketIter);
    heniNeighborTableTIterSeekNextNeighbor(nbtti);
}
#line 1 "/home/gumol/heni-whip6/nesc/whip6/lib/henikernel/base/src/HENIPacket.c"
/*
 * HENI: Hierarchical Embedded Network Infrastructure
 *
 * Copyright (c) 2016, University of Warsaw
 * All rights reserved.
 *
 */
#include "HENIBase.h"
#include "HENIError.h"
#include "HENIFrame.h"
#include "HENIPacket.h"



enum
{
    HENI_PACKET_ADDR_HOP_LIMIT_MAX = (sizeof(heni_packet_hop_limit_t) << 8) - 1,
};


HENI_API_FUNCT_DEF_PREFIX void heniPacketAddrReset(
        heni_packet_addr_t * paddr
) HENI_API_FUNCT_DEF_SUFFIX
{
    memset(paddr, 0x00, sizeof(heni_packet_addr_t));
    heniPacketAddrSetHopLimit(paddr, HENI_PACKET_ADDR_HOP_LIMIT_MAX);
    heniPacketAddrSetInstanceID(paddr, HENI_INSTANCE_ID_INVALID);
}



HENI_HID_FUNCT_DEF_PREFIX void heniPacketTxInfoUpdateWithFrameTxInfo(
        heni_packet_tx_info_t * pinfo,
        heni_frame_tx_info_t const * finfo
) HENI_HID_FUNCT_DEF_SUFFIX
{
    // FIXME: here a different approach, taking into account whether the frame was actually transmitted
    // FIXME: not all packets have to be acked
    if ((finfo->flags & HENI_FRAME_TX_INFO_FLAG_FRAME_TRANSMISSION_ACKED) != 0)
    {
        HENI_DASSERT(finfo->numAttempts > 0);
        ++pinfo->numAckedFrames;
        pinfo->numUnackedFrames += finfo->numAttempts - 1;
    }
    else
    {
        pinfo->status = HENI_PACKET_ROUTING_ERROR_HOP_BY_HOP_ACK_FAILED;
        pinfo->numUnackedFrames += finfo->numAttempts;
    }
}



HENI_HID_FUNCT_DEF_PREFIX void heniPacketRxInfoUpdateWithFrameRxInfo(
        heni_packet_rx_info_t * pinfo,
        heni_frame_rx_info_t const * finfo
) HENI_HID_FUNCT_DEF_SUFFIX
{
    /* There is nothing to be done here as of now. */
}

#line 1 "/home/gumol/heni-whip6/nesc/whip6/lib/henikernel/base/src/HENIScheduler.c"
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

#line 1 "/home/gumol/heni-whip6/nesc/whip6/lib/henikernel/base/src/HENIVectoredIO.c"
/*
 * HENI: Hierarchical Embedded Network Infrastructure
 *
 * Copyright (c) 2016, University of Warsaw
 * All rights reserved.
 *
 */
#include <string.h>
#include "HENIBase.h"
#include "HENIError.h"
#include "HENILinkedList.h"
#include "HENIVectoredIO.h"



HENI_API_FUNCT_DEF_PREFIX size_t heniIOBufListGetCount(
        heni_iobuf_list_t * iol
) HENI_API_FUNCT_DEF_SUFFIX
{
    heni_linked_list_fiter_t   liter;
    size_t                     count = 0;
    
    heniLinkedListFIterInit(&liter, &iol->llist);
    while (heniLinkedListFIterIsActive(&liter))
    {
        ++count;
        heniLinkedListFIterAdvance(&liter);
    }
    return count;
}



HENI_API_FUNCT_DEF_PREFIX size_t heniIOBufListGetCapacity(
        heni_iobuf_list_t * iol
) HENI_API_FUNCT_DEF_SUFFIX
{
    heni_linked_list_fiter_t   liter;
    size_t                     capacity = 0;
    
    heniLinkedListFIterInit(&liter, &iol->llist);
    while (heniLinkedListFIterIsActive(&liter))
    {
        heni_iobuf_list_node_t const *   ionode;
        ionode =
                heniVectoredIOGetIONodeCPtrFromLNodeCPtr(
                        heniLinkedListFIterGetNode(&liter)
                );
        capacity += ionode->iobuf.len;
        heniLinkedListFIterAdvance(&liter);
    }
    return capacity;
}



HENI_API_FUNCT_DEF_PREFIX heni_iobuf_t heniIOBufListFIterGetBuf(
        heni_iobuf_list_fiter_t const * iter
) HENI_API_FUNCT_DEF_SUFFIX
{
    heni_iobuf_t                     res;
    heni_iobuf_list_node_t const *   ioln;
    ioln = heniIOBufListFIterGetNode(iter);
    if (ioln == NULL)
    {
        res.base = NULL;
        res.len = 0;
    }
    else
    {
        HENI_DASSERT(iter->boff < ioln->iobuf.len);
        res.base = (void *)((uint8_t *)ioln->iobuf.base + iter->boff);
        res.len = ioln->iobuf.len - iter->boff;
    }
    return res;
}



HENI_API_FUNCT_DEF_PREFIX size_t heniIOBufListFIterTryAdvance(
        heni_iobuf_list_fiter_t * iter,
        size_t delta
) HENI_API_FUNCT_DEF_SUFFIX
{
    size_t   resDelta = 0;
    while (heniLinkedListFIterIsActive(&iter->liter))
    {
        heni_iobuf_list_node_t const *   ionode;
        size_t                           maxDelta;
        ionode =
                heniVectoredIOGetIONodeCPtrFromLNodeCPtr(
                        heniLinkedListFIterGetNode(&iter->liter)
                );
        HENI_DASSERT(ionode->iobuf.len > iter->boff);
        maxDelta = ionode->iobuf.len - iter->boff;
        if (maxDelta > delta)
        {
            iter->boff += delta;
            return resDelta + delta;
        }
        delta -= maxDelta;
        resDelta += maxDelta;
        iter->boff = 0;
        heniLinkedListFIterAdvance(&iter->liter);
    }
    return resDelta;
}



HENI_API_FUNCT_DEF_PREFIX size_t heniIOBufListFIterTryCopyFromOneBufAndAdvance(
        heni_iobuf_list_fiter_t * iter,
        void const * bufPtr,
        size_t bufLen
) HENI_API_FUNCT_DEF_SUFFIX
{
    size_t   numCopiedBytes = 0;
    while (heniLinkedListFIterIsActive(&iter->liter))
    {
        heni_iobuf_list_node_t const *   ionode;
        size_t                           maxDelta;
        ionode =
                heniVectoredIOGetIONodeCPtrFromLNodeCPtr(
                        heniLinkedListFIterGetNode(&iter->liter)
                );
        HENI_DASSERT(ionode->iobuf.len > iter->boff);
        maxDelta = ionode->iobuf.len - iter->boff;
        if (maxDelta > bufLen)
        {
            if (bufLen > 0)
            {
                memcpy(
                        (uint8_t *)ionode->iobuf.base + iter->boff,
                        (uint8_t const *)bufPtr + numCopiedBytes,
                        bufLen
                );
                iter->boff += bufLen;
                numCopiedBytes += bufLen;
            }
            break;
        }
        memcpy(
                (uint8_t *)ionode->iobuf.base + iter->boff,
                (uint8_t const *)bufPtr + numCopiedBytes,
                maxDelta
        );
        bufLen -= maxDelta;
        numCopiedBytes += maxDelta;
        iter->boff = 0;
        heniLinkedListFIterAdvance(&iter->liter);
    }
    return numCopiedBytes;
}



HENI_API_FUNCT_DEF_PREFIX size_t heniIOBufListFIterTryCopyIntoOneBufAndAdvance(
        heni_iobuf_list_fiter_t * iter,
        void * bufPtr,
        size_t bufLen
) HENI_API_FUNCT_DEF_SUFFIX
{
    size_t   numCopiedBytes = 0;
    while (heniLinkedListFIterIsActive(&iter->liter))
    {
        heni_iobuf_list_node_t const *   ionode;
        size_t                           maxDelta;
        ionode =
                heniVectoredIOGetIONodeCPtrFromLNodeCPtr(
                        heniLinkedListFIterGetNode(&iter->liter)
                );
        HENI_DASSERT(ionode->iobuf.len > iter->boff);
        maxDelta = ionode->iobuf.len - iter->boff;
        if (maxDelta > bufLen)
        {
            if (bufLen > 0)
            {
                memcpy(
                        (uint8_t *)bufPtr + numCopiedBytes,
                        (uint8_t const *)ionode->iobuf.base + iter->boff,
                        bufLen
                );
                iter->boff += bufLen;
                numCopiedBytes += bufLen;
            }
            break;
        }
        memcpy(
                (uint8_t *)bufPtr + numCopiedBytes,
                (uint8_t const *)ionode->iobuf.base + iter->boff,
                maxDelta
        );
        bufLen -= maxDelta;
        numCopiedBytes += maxDelta;
        iter->boff = 0;
        heniLinkedListFIterAdvance(&iter->liter);
    }
    return numCopiedBytes;
}



HENI_API_FUNCT_DEF_PREFIX size_t heniIOBufListFIterTryCopyToAnotherAndAdvanceBoth(
        heni_iobuf_list_fiter_t * fromIter,
        heni_iobuf_list_fiter_t * toIter,
        size_t num
) HENI_API_FUNCT_DEF_SUFFIX
{
    size_t                      numCopiedBytes = 0;
    while (heniLinkedListFIterIsActive(&fromIter->liter))
    {
        heni_iobuf_list_node_t const *   ionode;
        size_t                           maxDelta;
        size_t                           realDelta;
        ionode =
                heniVectoredIOGetIONodeCPtrFromLNodeCPtr(
                        heniLinkedListFIterGetNode(&fromIter->liter)
                );
        HENI_DASSERT(ionode->iobuf.len > fromIter->boff);
        maxDelta = ionode->iobuf.len - fromIter->boff;
        realDelta =
                heniIOBufListFIterTryCopyFromOneBufAndAdvance(
                        toIter,
                        (uint8_t const *)ionode->iobuf.base + fromIter->boff,
                        maxDelta > num ? num : maxDelta
                );
        num -= realDelta;
        numCopiedBytes += realDelta;
        if (realDelta < maxDelta)
        {
            fromIter->boff += realDelta;
            break;
        }
        fromIter->boff = 0;
        heniLinkedListFIterAdvance(&fromIter->liter);
    }
    return numCopiedBytes;
}



HENI_API_FUNCT_DEF_PREFIX int_fast8_t heniIOBufListFIterSplitAt(
        heni_iobuf_list_fiter_t * iter,
        heni_iobuf_list_t * iolTail,
        heni_iobuf_list_node_t * iolExtraNode
) HENI_API_FUNCT_DEF_SUFFIX
{
    int_fast8_t   usedExtra = 0;

    HENI_DASSERT(&iolTail->llist != heniLinkedListFIterGetList(&iter->liter));
    if (heniLinkedListFIterIsActive(&iter->liter))
    {
        heni_linked_list_node_t *   lnode;
        heni_iobuf_list_node_t *    ionode;
        
        lnode = heniLinkedListFIterGetNode(&iter->liter);
        ionode = heniVectoredIOGetIONodePtrFromLNodePtr(lnode);
        HENI_DASSERT(ionode->iobuf.len > iter->boff);
        if (iter->boff == 0)
        {
            heniLinkedListFIterSplitBeforeWithoutMoving(&iter->liter, &iolTail->llist);
        }
        else
        {
            HENI_DASSERT(iolExtraNode != NULL);
            iolExtraNode->iobuf.base = (void *)((uint8_t *)ionode->iobuf.base + iter->boff);
            iolExtraNode->iobuf.len = ionode->iobuf.len - iter->boff;
            ionode->iobuf.len = iter->boff;
            heniLinkedListFIterSplitAfterAndAdvance(&iter->liter, &iolTail->llist);
            heniLinkedListFIterAddBeforeWithoutMoving(&iter->liter, &iolExtraNode->lnode);
            heniLinkedListFIterInit(&iter->liter, &iolTail->llist);
            iter->boff = 0;
            usedExtra = 1;
        }
    }
    else
    {
        heniLinkedListInit(&iolTail->llist);
        heniLinkedListFIterInit(&iter->liter, &iolTail->llist);
        iter->boff = 0;
    }
    return usedExtra;
}



HENI_API_FUNCT_DEF_PREFIX heni_iobuf_list_node_t * heniIOBufListFIterTryConcatenateWithPrevious(
        heni_iobuf_list_fiter_t * iter
) HENI_API_FUNCT_DEF_SUFFIX
{
    heni_linked_list_riter_t   tmpLIter;
    heni_iobuf_list_node_t *   ionodeCurr;
    heni_iobuf_list_node_t *   ionodePrev;

    heniLinkedListFIterToRIter(&iter->liter, &tmpLIter);
    HENI_DASSERT(heniLinkedListRIterIsActive(&tmpLIter));
    ionodeCurr =
            heniVectoredIOGetIONodePtrFromLNodePtr(
                    heniLinkedListRIterGetNode(&tmpLIter)
            );
    heniLinkedListRIterAdvance(&tmpLIter);
    if (! heniLinkedListRIterIsActive(&tmpLIter))
    {
        return NULL;
    }
    ionodePrev =
            heniVectoredIOGetIONodePtrFromLNodePtr(
                    heniLinkedListRIterGetNode(&tmpLIter)
            );
    if (((void *)((uint8_t *)ionodePrev->iobuf.base + ionodePrev->iobuf.len)) !=
            ionodeCurr->iobuf.base)
    {
        return NULL;
    }
    iter->boff += ionodePrev->iobuf.len;
    ionodePrev->iobuf.len += ionodeCurr->iobuf.len;
    heniLinkedListFIterRemoveAndAdvance(&iter->liter);
    heniLinkedListRIterToFIter(&tmpLIter, &iter->liter);
    return ionodeCurr;
}

#line 1 "/home/gumol/heni-whip6/nesc/whip6/lib/henikernel/base/src/HENIZoneTable.c"
/*
 * HENI: Hierarchical Embedded Network Infrastructure
 *
 * Copyright (c) 2017, University of Warsaw
 * All rights reserved.
 *
 */
#include "HENIKernel.h"
#include "HENILabel.h"
#include "HENILinkedList.h"
#include "HENIZoneTable.h"




/**
 * This is a private implementation function.
 *
 * Searches in a HENI zone table row for a next zone
 * starting from the current position and sets the
 * iterator to point at this zone, if it exists.
 * If the iteration process has finished, nothing
 * is done.
 * @param iter The iterator to set.
 * @return Nonzero if the next zone in row has been
 *   found or zero otherwise.
 */
HENI_HID_FUNCT_DEC_PREFIX int_fast8_t heniZoneTableTIterSeekNextZoneInRow(
        heni_zone_table_titer_t * iter
) HENI_HID_FUNCT_DEC_SUFFIX;



HENI_API_FUNCT_DEF_PREFIX void heniZoneTableInit(
        heni_zone_table_t * zt,
        heni_instance_t * inst,
        heni_zone_table_bucket_t * const * bucketPtrsPerRow,
        size_t bucketCountPerRow
) HENI_API_FUNCT_DEF_SUFFIX
{
    heni_level_t   numLevels, levelIdx;

    HENI_DASSERT(inst != NULL);
    numLevels = heniKernelInstanceGetNumLevelsForInstancePtr(inst);
    HENI_PASSERT(numLevels >= 1 && numLevels <= HENI_MAX_LIDS_IN_LABEL);
    zt->bucketPtrsPerRow[0] = &zt->rootZoneBucketBuf;
    heniLinkedListInit(&zt->rootZoneBucketBuf.bucketList);
    for (levelIdx = 1; levelIdx <= numLevels; ++levelIdx)
    {
        size_t   bucketIdx;
        zt->bucketPtrsPerRow[levelIdx] = bucketPtrsPerRow[levelIdx - 1];
        HENI_PASSERT(zt->bucketPtrsPerRow[levelIdx] != NULL);
        for (bucketIdx = 0; bucketIdx < bucketCountPerRow; ++bucketIdx)
        {
            heniLinkedListInit(&(zt->bucketPtrsPerRow[levelIdx][bucketIdx].bucketList));
        }
    }
    for (; levelIdx <= HENI_MAX_LIDS_IN_LABEL; ++levelIdx)
    {
        zt->bucketPtrsPerRow[levelIdx] = NULL;
    }
    zt->bucketCountPerRow = bucketCountPerRow;
    zt->inst = inst;
}



HENI_API_FUNCT_DEF_PREFIX void heniZoneTableAddNonexisting(
        heni_zone_table_t * zt,
        heni_level_t level,
        heni_zone_lid_t lid,
        heni_zone_discr_t discr,
        heni_zone_t * zone
) HENI_API_FUNCT_DEF_SUFFIX
{
    heni_zone_table_titer_t   iter;
    HENI_DASSERT((level == 0 && lid == heniLabelSpecGetZoneLIDInvalid(heniKernelInstanceGetLabelSpecForInstancePtr(zt->inst))) ||
            (level > 0 && heniLabelSpecIsZoneLIDAssignable(lid, heniKernelInstanceGetLabelSpecForInstancePtr(zt->inst))));
    HENI_DASSERT(heniZoneDiscrSpecIsZoneDiscrAssignable(discr, heniKernelInstanceGetNumZoneDiscrBitsForInstancePtr(zt->inst)));
    HENI_PASSERT(! heniZoneTableTIterInitAtOrBeforeZoneWithLevelAndLidAndDiscr(&iter, zt, level, lid, discr));
    heniZoneEntrySetKey(zone, level, lid, discr);
    heniLinkedListFIterAddBeforeWithoutMoving(
            &iter.inrowBucketIter,
            heniZoneEntryToBucketListNode(zone)
    );
}



HENI_API_FUNCT_DEF_PREFIX heni_zone_t * heniZoneTableFind(
        heni_zone_table_t * zt,
        heni_level_t level,
        heni_zone_lid_t lid,
        heni_zone_discr_t discr
) HENI_API_FUNCT_DEF_SUFFIX
{
    heni_zone_table_titer_t   iter;
    /*HENI_DASSERT(heniLabelSpecIsZoneLIDAssignable(lid, heniKernelInstanceGetLabelSpecForInstancePtr(zt->inst)));
    HENI_DASSERT(heniZoneDiscrSpecIsZoneDiscrAssignable(discr, heniKernelInstanceGetNumZoneDiscrBitsForInstancePtr(zt->inst)));*/
    if (heniZoneTableTIterInitAtOrBeforeZoneWithLevelAndLidAndDiscr(&iter, zt, level, lid, discr))
    {
        return heniZoneTableTIterGetZone(&iter);
    }
    return NULL;
}



HENI_API_FUNCT_DEF_PREFIX void heniZoneTableRemoveExisting(
        heni_zone_table_t * zt,
        heni_level_t level,
        heni_zone_lid_t lid,
        heni_zone_discr_t discr
) HENI_API_FUNCT_DEF_SUFFIX
{
    heni_zone_table_titer_t   iter;
    HENI_DASSERT((level == 0 && lid == heniLabelSpecGetZoneLIDInvalid(heniKernelInstanceGetLabelSpecForInstancePtr(zt->inst))) ||
            (level > 0 && heniLabelSpecIsZoneLIDAssignable(lid, heniKernelInstanceGetLabelSpecForInstancePtr(zt->inst))));
    HENI_DASSERT(heniZoneDiscrSpecIsZoneDiscrAssignable(discr, heniKernelInstanceGetNumZoneDiscrBitsForInstancePtr(zt->inst)));
    HENI_PASSERT(heniZoneTableTIterInitAtOrBeforeZoneWithLevelAndLidAndDiscr(&iter, zt, level, lid, discr));
    heniLinkedListFIterRemoveAndAdvance(&iter.inrowBucketIter);
}



HENI_API_FUNCT_DEF_PREFIX void heniZoneTableCleanup(
        heni_zone_table_t * zt
) HENI_API_FUNCT_DEF_SUFFIX
{
    heni_level_t   levelIdx;
    for (levelIdx = 0; levelIdx <= HENI_MAX_LIDS_IN_LABEL; ++levelIdx)
    {
        zt->bucketPtrsPerRow[levelIdx] = NULL;
    }
    zt->bucketCountPerRow = 0;
    zt->inst = NULL;
}



HENI_HID_FUNCT_DEF_PREFIX int_fast8_t heniZoneTableTIterInitAtOrBeforeZoneWithLevelAndLid(
        heni_zone_table_titer_t * iter,
        heni_zone_table_t * zt,
        heni_level_t zoneLevel,
        heni_zone_lid_t zoneLID
) HENI_HID_FUNCT_DEF_SUFFIX
{
    size_t   bucketIdx;
    HENI_DASSERT(zoneLevel <= heniKernelInstanceGetNumLevelsForInstancePtr(zt->inst));
    HENI_DASSERT(zt->bucketCountPerRow > 0);
    HENI_DASSERT(zt->bucketPtrsPerRow[zoneLevel] != NULL);
    bucketIdx = heniZoneTableGetBucketForLevelAndLID(zt, zoneLevel, zoneLID);
    heniLinkedListFIterInit(
            &iter->inrowBucketIter,
            &(zt->bucketPtrsPerRow[zoneLevel][bucketIdx].bucketList)
    );
    iter->inrowBucketIdx = bucketIdx;
    iter->rowLevel = zoneLevel;
    iter->zt = zt;
    while (heniLinkedListFIterIsActive(&iter->inrowBucketIter))
    {
        heni_zone_t const *   zone;
        zone =
                heniZoneEntryFromBucketListNode(
                        heniLinkedListFIterGetNode(&iter->inrowBucketIter)
                );
        if (zone->key.lid >= zoneLID)
        {
            return zone->key.lid == zoneLID ? (int_fast8_t)1 : (int_fast8_t)0;
        }
        heniLinkedListFIterAdvance(&iter->inrowBucketIter);
    }
    return (int_fast8_t)0;
}



HENI_HID_FUNCT_DEF_PREFIX int_fast8_t heniZoneTableTIterInitAtOrBeforeZoneWithLevelAndLidAndDiscr(
        heni_zone_table_titer_t * iter,
        heni_zone_table_t * zt,
        heni_level_t zoneLevel,
        heni_zone_lid_t zoneLID,
        heni_zone_discr_t zoneDiscr
) HENI_HID_FUNCT_DEF_SUFFIX
{
    if (! heniZoneTableTIterInitAtOrBeforeZoneWithLevelAndLid(iter, zt, zoneLevel, zoneLID))
    {
        return 0;
    }
    do
    {
        heni_zone_t const *   zone;
        zone =
                heniZoneEntryFromBucketListNode(
                        heniLinkedListFIterGetNode(&iter->inrowBucketIter)
                );
        if (zone->key.lid > zoneLID)
        {
            return 0;
        }
        else if (zone->key.lid == zoneLID && zone->key.discr >= zoneDiscr)
        {
            return zone->key.discr == zoneDiscr ? (int_fast8_t)1 : (int_fast8_t)0;
        }
        heniLinkedListFIterAdvance(&iter->inrowBucketIter);
    }
    while (heniLinkedListFIterIsActive(&iter->inrowBucketIter));
    return (int_fast8_t)0;
}



HENI_API_FUNCT_DEF_PREFIX void heniZoneTableTIterSetAtLevel(
        heni_zone_table_titer_t * iter,
        heni_zone_table_t * zt,
        heni_level_t zoneLevel
) HENI_API_FUNCT_DEF_SUFFIX
{
    HENI_DASSERT(zoneLevel <= heniKernelInstanceGetNumLevelsForInstancePtr(zt->inst));
    HENI_DASSERT(zt->bucketCountPerRow > 0);
    HENI_DASSERT(zt->bucketPtrsPerRow[zoneLevel] != NULL);
    heniLinkedListFIterInit(
            &iter->inrowBucketIter,
            &(zt->bucketPtrsPerRow[zoneLevel][0].bucketList)
    );
    iter->inrowBucketIdx = 0;
    iter->rowLevel = zoneLevel;
    iter->zt = zt;
    if (! heniZoneTableTIterSeekNextZoneInRow(iter))
    {
        heniZoneTableTIterFinish(iter);
    }
}



HENI_API_FUNCT_DEF_PREFIX void heniZoneTableTIterSetAtLevelAndLID(
        heni_zone_table_titer_t * iter,
        heni_zone_table_t * zt,
        heni_level_t zoneLevel,
        heni_zone_lid_t zoneLID
) HENI_API_FUNCT_DEF_SUFFIX
{
    HENI_DASSERT(zoneLevel <= heniKernelInstanceGetNumLevelsForInstancePtr(zt->inst));
    HENI_DASSERT(zt->bucketCountPerRow > 0);
    HENI_DASSERT(zt->bucketPtrsPerRow[zoneLevel] != NULL);
    HENI_DASSERT(heniLabelSpecIsZoneLIDAssignable(zoneLID, heniKernelInstanceGetLabelSpecForInstancePtr(zt->inst)));
    if (! heniZoneTableTIterInitAtOrBeforeZoneWithLevelAndLid(
            iter, zt, zoneLevel, zoneLID))
    {
        heniZoneTableTIterFinish(iter);
    }
}



HENI_API_FUNCT_DEF_PREFIX void heniZoneTableTIterSetAtLevelAndLIDAndDiscr(
        heni_zone_table_titer_t * iter,
        heni_zone_table_t * zt,
        heni_level_t zoneLevel,
        heni_zone_lid_t zoneLID,
        heni_zone_discr_t zoneDiscr
) HENI_API_FUNCT_DEF_SUFFIX
{
    HENI_DASSERT(zoneLevel <= heniKernelInstanceGetNumLevelsForInstancePtr(zt->inst));
    HENI_DASSERT(zt->bucketCountPerRow > 0);
    HENI_DASSERT(zt->bucketPtrsPerRow[zoneLevel] != NULL);
    HENI_DASSERT(heniLabelSpecIsZoneLIDAssignable(zoneLID, heniKernelInstanceGetLabelSpecForInstancePtr(zt->inst)));
    HENI_DASSERT(heniZoneDiscrSpecIsZoneDiscrAssignable(zoneDiscr, heniKernelInstanceGetNumZoneDiscrBitsForInstancePtr(zt->inst)));
    if (! heniZoneTableTIterInitAtOrBeforeZoneWithLevelAndLidAndDiscr(iter, zt, zoneLevel, zoneLID, zoneDiscr))
    {
        heniZoneTableTIterFinish(iter);
    }
}



HENI_API_FUNCT_DEF_PREFIX void heniZoneTableTIterCopy(
        heni_zone_table_titer_t const * iterSrc,
        heni_zone_table_titer_t * iterDst
) HENI_API_FUNCT_DEF_SUFFIX
{
    heniLinkedListFIterCopy(&iterSrc->inrowBucketIter, &iterDst->inrowBucketIter);
    iterDst->inrowBucketIdx = iterSrc->inrowBucketIdx;
    iterDst->rowLevel = iterSrc->rowLevel;
    iterDst->zt = iterSrc->zt;
}



HENI_API_FUNCT_DEF_PREFIX void heniZoneTableTIterFinish(
        heni_zone_table_titer_t * iter
) HENI_API_FUNCT_DEF_SUFFIX
{
    HENI_DASSERT(iter->zt != NULL);
    HENI_DASSERT(iter->zt->bucketCountPerRow > 0);
    /* NOTICE iwanicki 2017-06-09:                    */
    /* In theory, we do not need the init, but        */
    /* let's leave it here to make the implementation */
    /* more robust.                                   */
    heniLinkedListFIterInit(
            &iter->inrowBucketIter,
            &(iter->zt->bucketPtrsPerRow[0][0].bucketList)
    );
    heniLinkedListFIterFinish(&iter->inrowBucketIter);
    iter->inrowBucketIdx = iter->zt->bucketCountPerRow + 1;
    iter->rowLevel = heniKernelInstanceGetNumLevelsForInstancePtr(iter->zt->inst) + 1;
}



HENI_API_FUNCT_DEF_PREFIX void heniZoneTableTIterAdvancePreservingLevel(
        heni_zone_table_titer_t * iter
) HENI_API_FUNCT_DEF_SUFFIX
{
    HENI_DASSERT(heniZoneTableTIterIsActive(iter));
    HENI_DASSERT(heniLinkedListFIterIsActive(&iter->inrowBucketIter));
    heniLinkedListFIterAdvance(&iter->inrowBucketIter);
    if (! heniZoneTableTIterSeekNextZoneInRow(iter))
    {
        heniZoneTableTIterFinish(iter);
    }
}



HENI_API_FUNCT_DEF_PREFIX void heniZoneTableTIterAdvancePreservingLevelAndLID(
        heni_zone_table_titer_t * iter
) HENI_API_FUNCT_DEF_SUFFIX
{
    heni_zone_t const *   zone;
    heni_zone_lid_t       lid;
    HENI_DASSERT(heniZoneTableTIterIsActive(iter));
    HENI_DASSERT(heniLinkedListFIterIsActive(&iter->inrowBucketIter));
    zone =
            heniZoneEntryFromBucketListNode(
                    heniLinkedListFIterGetNode(&iter->inrowBucketIter)
            );
    lid = heniZoneEntryGetLID(zone);
    heniLinkedListFIterAdvance(&iter->inrowBucketIter);
    if (heniLinkedListFIterIsActive(&iter->inrowBucketIter))
    {
        zone =
                heniZoneEntryFromBucketListNode(
                        heniLinkedListFIterGetNode(&iter->inrowBucketIter)
                );
        if (heniZoneEntryGetLID(zone) == lid)
        {
            return;
        }
    }
    heniZoneTableTIterFinish(iter);
}



HENI_API_FUNCT_DEF_PREFIX void heniZoneTableTIterRemoveAndAdvancePreservingLevel(
        heni_zone_table_titer_t * iter
) HENI_API_FUNCT_DEF_SUFFIX
{
    HENI_DASSERT(heniZoneTableTIterIsActive(iter));
    HENI_DASSERT(heniLinkedListFIterIsActive(&iter->inrowBucketIter));
    heniLinkedListFIterRemoveAndAdvance(&iter->inrowBucketIter);
    if (! heniZoneTableTIterSeekNextZoneInRow(iter))
    {
        heniZoneTableTIterFinish(iter);
    }
}



HENI_API_FUNCT_DEF_PREFIX void heniZoneTableTIterRemoveAndAdvancePreservingLevelAndLID(
        heni_zone_table_titer_t * iter
) HENI_API_FUNCT_DEF_SUFFIX
{
    heni_zone_t const *   zone;
    heni_zone_lid_t       lid;
    HENI_DASSERT(heniZoneTableTIterIsActive(iter));
    HENI_DASSERT(heniLinkedListFIterIsActive(&iter->inrowBucketIter));
    zone =
            heniZoneEntryFromBucketListNode(
                    heniLinkedListFIterGetNode(&iter->inrowBucketIter)
            );
    lid = heniZoneEntryGetLID(zone);
    heniLinkedListFIterRemoveAndAdvance(&iter->inrowBucketIter);
    if (heniLinkedListFIterIsActive(&iter->inrowBucketIter))
    {
        zone =
                heniZoneEntryFromBucketListNode(
                        heniLinkedListFIterGetNode(&iter->inrowBucketIter)
                );
        if (heniZoneEntryGetLID(zone) == lid)
        {
            return;
        }
    }
    heniZoneTableTIterFinish(iter);
}



HENI_HID_FUNCT_DEF_PREFIX int_fast8_t heniZoneTableTIterSeekNextZoneInRow(
        heni_zone_table_titer_t * iter
) HENI_HID_FUNCT_DEF_SUFFIX
{
    heni_zone_table_bucket_t *   rowPtr;
    size_t                       bucketCount;

    HENI_DASSERT(iter->rowLevel <= heniKernelInstanceGetNumLevelsForInstancePtr(iter->zt->inst));
    bucketCount = iter->rowLevel == 0 ? 1 : iter->zt->bucketCountPerRow;
    if (iter->inrowBucketIdx >= bucketCount)
    {
        return (int_fast8_t)0;
    }
    if (heniLinkedListFIterIsActive(&iter->inrowBucketIter))
    {
        return (int_fast8_t)1;
    }
    rowPtr = iter->zt->bucketPtrsPerRow[iter->rowLevel];
    for (++iter->inrowBucketIdx; iter->inrowBucketIdx < bucketCount; ++iter->inrowBucketIdx)
    {
        heniLinkedListFIterInit(
                &iter->inrowBucketIter,
                &(rowPtr[iter->inrowBucketIdx].bucketList)
        );
        if (heniLinkedListFIterIsActive(&iter->inrowBucketIter))
        {
            return (int_fast8_t)1;
        }
    }
    return (int_fast8_t)0;
}



HENI_API_FUNCT_DEF_PREFIX void heniZoneTableTIterToPIter(
        heni_zone_table_titer_t const * titer,
        heni_zone_table_piter_t * piter
) HENI_API_FUNCT_DEF_SUFFIX
{
    HENI_DASSERT(titer->zt != NULL);
    piter->zt = titer->zt;
    if (heniZoneTableTIterIsActive(titer))
    {
        piter->zoneLevel = heniZoneTableTIterGetLevel(titer);
        piter->zoneLID = heniZoneTableTIterGetLID(titer);
        piter->zoneDiscr = heniZoneTableTIterGetDiscr(titer);
    }
    else
    {
        piter->zoneLevel =
                heniKernelInstanceGetNumLevelsForInstancePtr(
                        titer->zt->inst
                ) + 1;
        piter->zoneLID =
                heniLabelSpecGetZoneLIDInvalid(
                        heniKernelInstanceGetLabelSpecForInstancePtr(
                                titer->zt->inst
                        )
                );
        piter->zoneDiscr =
                heniZoneDiscrSpecGetZoneDiscrInvalid(
                        heniKernelInstanceGetNumZoneDiscrBitsForInstancePtr(
                                titer->zt->inst
                        )
                );
    }
}



HENI_API_FUNCT_DEF_PREFIX void heniZoneTablePIterToTIterPreservingLevel(
        heni_zone_table_piter_t const * piter,
        heni_zone_table_titer_t * titer
) HENI_API_FUNCT_DEF_SUFFIX
{
    if (piter->zoneLevel <= heniKernelInstanceGetNumLevelsForInstancePtr(
                piter->zt->inst))
    {
        HENI_DASSERT((piter->zoneLevel > 0 &&
                heniLabelSpecIsZoneLIDAssignable(
                        piter->zoneLID,
                        heniKernelInstanceGetLabelSpecForInstancePtr(
                                piter->zt->inst))) ||
                (piter->zoneLevel == 0 && piter->zoneLID ==
                        heniLabelSpecGetZoneLIDInvalid(
                                heniKernelInstanceGetLabelSpecForInstancePtr(
                                        piter->zt->inst))));
        if (! heniZoneTableTIterInitAtOrBeforeZoneWithLevelAndLidAndDiscr(
                titer, piter->zt, piter->zoneLevel, piter->zoneLID, piter->zoneDiscr))
        {
            if (! heniZoneTableTIterSeekNextZoneInRow(titer))
            {
                heniZoneTableTIterFinish(titer);
            }
        }
    }
    else
    {
        heniZoneTableTIterSetAtLevel(titer, piter->zt, 0);
        heniZoneTableTIterFinish(titer);
    }
}



HENI_API_FUNCT_DEF_PREFIX void heniZoneTablePIterToTIterPreservingLevelAndLID(
        heni_zone_table_piter_t const * piter,
        heni_zone_table_titer_t * titer
) HENI_API_FUNCT_DEF_SUFFIX
{
    heniZoneTablePIterToTIterPreservingLevel(piter, titer);
    if (heniZoneTableTIterIsActive(titer))
    {
        if (piter->zoneLID != heniZoneTableTIterGetLID(titer))
        {
            heniZoneTableTIterFinish(titer);
        }
    }
}



HENI_API_FUNCT_DEF_PREFIX void heniZoneTablePIterToTIterPreservingLevelAndLIDAndDiscr(
        heni_zone_table_piter_t const * piter,
        heni_zone_table_titer_t * titer
) HENI_API_FUNCT_DEF_SUFFIX
{
    heniZoneTablePIterToTIterPreservingLevel(piter, titer);
    if (heniZoneTableTIterIsActive(titer))
    {
        if (piter->zoneLID != heniZoneTableTIterGetLID(titer) ||
                piter->zoneDiscr != heniZoneTableTIterGetDiscr(titer))
        {
            heniZoneTableTIterFinish(titer);
        }
    }
}
#endif /* __LIBHENI_C_GUARD__ */
