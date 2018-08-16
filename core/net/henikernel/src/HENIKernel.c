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
