/*
 * HENI: Hierarchical Embedded Network Infrastructure
 *
 * Copyright (c) 2016, University of Warsaw
 * All rights reserved.
 *
 */
#ifndef __HENI_KERNEL_H__
#define __HENI_KERNEL_H__

#include "HENIBase.h"
#include "HENIError.h"
#include "HENILinkedList.h"
#include "HENIPacket.h"
#include "HENIScheduler.h"
#include "HENIVectoredIO.h"


/**
 * @file
 * HENI: Accessors for the HENI kernel state.
 * @author Konrad Iwanicki <iwanicki@mimuw.edu.pl>
 */

/* ---------------------------------------------------------------------- *
 *                                                                        *
 *                          Kernel-related types                          *
 *                                                                        *
 * ---------------------------------------------------------------------- */





/* ---------------------------------------------------------------------- *
 *                                                                        *
 *                            Basic kernel API                            *
 *                                                                        *
 * ---------------------------------------------------------------------- */

/**
 * Schedules execution of a given function that
 * can be run as deferred computations, so-called
 * task, within a HENI kernel.
 * @param ker The HENI kernel.
 * @param taskName The name of the task function.
 * @return Nonzero if the function has been scheduled
 *   or zero if the function has already been scheduled.
 */
#define HENI_KERNEL_TASK_POST(ker, taskName) \
        heniKernelScheduleTask(ker, HENI_KERNEL_TASK_GET_UID(taskName))


/**
 * Initializes an entire HENI kernel.
 * @param ker The kernel.
 * @return Zero if the initialization was successful
 *   or a negative value denoting the number of
 *   the initialization step that failed.
 */
HENI_API_FUNCT_DEC_PREFIX int_fast8_t heniKernelInit(
        heni_kernel_t * ker
) HENI_API_FUNCT_DEC_SUFFIX;

/**
 * Allocates a new HENI instance for
 * a given HENI kernel.
 * @param ker The HENI kernel.
 * @param iid The instance ID.
 * @return A pointer to the allocated instance or
 *   NULL if there has been no memory to allocate
 *   the instance.
 */
HENI_EXT_FUNCT_DEC_PREFIX heni_instance_t * heniKernelInstanceAlloc(
        heni_kernel_t * ker,
        heni_instance_id_t iid
) HENI_EXT_FUNCT_DEC_SUFFIX;

/**
 * Frees the memory for an existing HENI instance
 * that has been allocated for a given HENI
 * kernel.
 * @param ker The HENI kernel.
 * @param iid The instance ID.
 * @param inst The HENI instance.
 */
HENI_EXT_FUNCT_DEC_PREFIX void heniKernelInstanceFree(
        heni_kernel_t * ker,
        heni_instance_id_t iid,
        heni_instance_t * inst
) HENI_EXT_FUNCT_DEC_SUFFIX;

/**
 * Starts a HENI instance.
 * @param ker The HENI kernel.
 * @param iid The ID of the instance.
 * @return Zero on success, one if the instance is
 *   already running, or a negative value denoting the
 *   number of the initialization step that failed.
 */
HENI_API_FUNCT_DEC_PREFIX int_fast8_t heniKernelInstanceStart(
        heni_kernel_t * ker,
        heni_instance_id_t iid
) HENI_API_FUNCT_DEC_SUFFIX;

/**
 * Checks if a HENI instance with a given ID is running.
 * @param ker The HENI kernel.
 * @param iid The ID of the instance.
 * @return Nonzero if the instance with a given ID is running
 *   or zero otherwise.
 */
HENI_API_FUNCT_DEC_PREFIX int_fast8_t heniKernelInstanceIsRunning(
        heni_kernel_t const * ker,
        heni_instance_id_t iid
) HENI_API_FUNCT_DEC_SUFFIX;

/**
 * Checks if a HENI instance with a given ID is stopping.
 * @param ker The HENI kernel.
 * @param iid The ID of the instance.
 * @return Nonzero if the instance with a given ID is running
 *   or zero otherwise.
 */
HENI_API_FUNCT_DEC_PREFIX int_fast8_t heniKernelInstanceIsStopping(
        heni_kernel_t const * ker,
        heni_instance_id_t iid
) HENI_API_FUNCT_DEC_SUFFIX;

/**
 * Schedules execution of deferred computations
 * within a HENI kernel. The computations are
 * performed from within a notification function,
 * heniKernelResumeComputation.
 * Implementers must guarantee that notification
 * function will not to be invoked from within
 * this function. The kernel guarantees, in turn,
 * that this function will not be invoked again
 * before the notification.
 * @param ker The HENI kernel.
 */
HENI_EXT_FUNCT_DEC_PREFIX void heniKernelPostponeComputations(
        heni_kernel_t * ker
) HENI_EXT_FUNCT_DEC_SUFFIX;

/**
 * An internal control task for a HENI kernel.
 * @param ker The HENI kernel.
 */
HENI_HID_FUNCT_DEC_PREFIX void heniKernelControlTask(
        heni_kernel_t * ker
) HENI_HID_FUNCT_DEC_SUFFIX;

/**
 * An internal task responsible for processing
 * outgoing packets by a HENI kernel.
 * @param ker The HENI kernel.
 */
HENI_HID_FUNCT_DEC_PREFIX void heniKernelProcessOutgoingPacketsTask(
        heni_kernel_t * ker
) HENI_HID_FUNCT_DEC_SUFFIX;

/**
 * An internal task responsible for processing
 * incoming packets by a HENI kernel.
 * @param ker The HENI kernel.
 */
HENI_HID_FUNCT_DEC_PREFIX void heniKernelProcessIncomingPacketsTask(
        heni_kernel_t * ker
) HENI_HID_FUNCT_DEC_SUFFIX;

/**
 * An internal task responsible for routing
 * packets by a HENI kernel.
 * @param ker The HENI kernel.
 */
HENI_HID_FUNCT_DEC_PREFIX void heniKernelRoutePacketsTask(
        heni_kernel_t * ker
) HENI_HID_FUNCT_DEC_SUFFIX;

/**
 * A notification function for executing postponed
 * computations within a HENI kernel.
 * @param ker The HENI kernel.
 */
HENI_API_FUNCT_DEC_PREFIX void heniKernelResumeComputations(
        heni_kernel_t * ker
) HENI_API_FUNCT_DEC_SUFFIX;

/**
 * Stops a HENI instance.
 * When the instance has fully stopped,
 * a notification function is invoked.
 * It is guaranteed that the notification will
 * not be invoked until this function has finished.
 * @param ker The HENI kernel.
 * @param iid The ID of the instance.
 */
HENI_API_FUNCT_DEC_PREFIX void heniKernelInstanceStopTrigger(
        heni_kernel_t * ker,
        heni_instance_id_t iid
) HENI_API_FUNCT_DEC_SUFFIX;

/**
 * Notifies that a HENI instance has fully stopped.
 * @param ker The HENI kernel.
 * @param iid The ID of the instance.
 */
HENI_EXT_FUNCT_DEC_PREFIX void heniKernelInstanceStopDone(
        heni_kernel_t * ker,
        heni_instance_id_t iid
) HENI_EXT_FUNCT_DEC_SUFFIX;

/**
 * Stops all HENI instances.
 * When all the instances have fully stopped,
 * a notification function is invoked.
 * It is guaranteed that the notification will
 * not be invoked until this function has finished.
 * @param ker The HENI kernel.
 */
HENI_API_FUNCT_DEC_PREFIX void heniKernelInstanceStopAllTrigger(
        heni_kernel_t * ker
) HENI_API_FUNCT_DEC_SUFFIX;

/**
 * Notifies that all HENI instances have fully stopped.
 * @param ker The HENI kernel.
 */
HENI_EXT_FUNCT_DEC_PREFIX void heniKernelInstanceStopAllDone(
        heni_kernel_t * ker
) HENI_EXT_FUNCT_DEC_SUFFIX;

/**
 * Cleans up a HENI kernel.
 * All HENI instances in the kernel must be stopped.
 * @param ker The kernel.
 */
HENI_API_FUNCT_DEC_PREFIX void heniKernelCleanup(
        heni_kernel_t * ker
) HENI_API_FUNCT_DEC_SUFFIX;

/**
 * Starts transmitting a HENI frame.
 * A transmission finish notification event
 * is guaranteed to be invoked if the function
 * returns zero.
 * @param ker The HENI kernel.
 * @param faddr The addressing information for
 *   the frame. This object remains valid only
 *   until this function returns. If the
 *   implementer needs to use it later, its
 *   contents must be copied.
 * @param fpld The payload of the frame. This
 *   object will be maintained by the HENI kernel
 *   until the transmission process has finished.
 *   Until then, however, its ownership passes to
 *   the implementer of this function. In
 *   particular, the implementer may modify the
 *   object arbitrarily.
 * @return Zero if the operation has succeeded
 *   or a negative value denoting the step that
 *   has failed. In the latter case, the transmission
 *   of the frame will not be completed and the
 *   appropriate completion signal will not be
 *   triggered.
 */
HENI_EXT_FUNCT_DEC_PREFIX int_fast8_t heniKernelFrameSendStart(
        heni_kernel_t * ker,
        heni_frame_addr_t const * faddr,
        heni_iobuf_list_t * fpld
) HENI_EXT_FUNCT_DEC_SUFFIX;

/**
 * Notifies that a transmission of a HENI frame
 * has finished.
 * @param ker The HENI kernel.
 * @param fpld The payload of the frame. This
 *   This object must be the same (both pointer-
 *   and content-wise) as the object passed when
 *   the transmission was initiated. Its ownership
 *   returns back to the HENI kernel.
 * @param fsts The status of transmitting the frame.
 *   The object remains valid only until this
 *   function returns.
 */
HENI_API_FUNCT_DEC_PREFIX void heniKernelFrameSendFinish(
        heni_kernel_t * ker,
        heni_iobuf_list_t * fpld,
        heni_frame_tx_info_t const * fsts
) HENI_API_FUNCT_DEC_SUFFIX;

/**
 * Starts receiving a HENI frame by the HENI kernel.
 * A reception finish notification function
 * is guaranteed to be invoked if the function
 * returns zero.
 * @param ker The HENI kernel.
 * @param faddr The addressing information for
 *   the frame. This object remains valid only
 *   until this function returns.
 * @param fpld The payload of the frame. The ownership
 *   of this object is transferred to the HENI kernel
 *   until the reception process has finished.
 *   In particular, the kernel may modify the
 *   object arbitrarily but guarantees that ultimately
 *   the object will return to its initial state.
 * @param fsts The status of receiving the frame.
 *   The object remains valid only until this
 *   function returns.
 * @return Zero if the operation has succeeded
 *   or a negative value denoting the step that
 *   has failed. In the latter case, the reception
 *   of the frame will not be completed and the
 *   appropriate completion function will not be
 *   invoked.
 */
HENI_API_FUNCT_DEC_PREFIX int_fast8_t heniKernelFrameReceiveStart(
        heni_kernel_t * ker,
        heni_frame_addr_t const * faddr,
        heni_iobuf_list_t * fpld,
        heni_frame_rx_info_t const * fsts
) HENI_API_FUNCT_DEC_SUFFIX;

/**
 * Notifies that a reception of a HENI frame by
 * a HENI kernel has finished.
 * @param ker The HENI kernel.
 * @param fpld The payload of the frame. This
 *   This object must be the same (both pointer-
 *   and content-wise) as the object passed when
 *   the reception was initiated. Its ownership
 *   returns back to the implementer.
 */
HENI_EXT_FUNCT_DEC_PREFIX void heniKernelFrameReceiveFinish(
        heni_kernel_t * ker,
        heni_iobuf_list_t * fpld
) HENI_EXT_FUNCT_DEC_SUFFIX;

/**
 * Starts sending a HENI packet.
 * The transmission notification event
 * is guaranteed to be invoked. The notification
 * may be invoked even if this function has
 * not yet returned.
 * @param ker The HENI kernel.
 * @param paddr The addressing information for
 *   the packet. This object will be copied
 *   to the HENI kernel.
 * @param ppld The payload of the packet. The
 *   ownership of this object will be taken over
 *   by the HENI kernel upon success.
 * @return Zero if the operation has succeeded
 *   or a negative value denoting the step that
 *   has failed. In the latter case, the transmission
 *   of the packet will not be completed and the
 *   appropriate completion signal will not be
 *   triggered.
 */
HENI_API_FUNCT_DEC_PREFIX int_fast8_t heniPacketSendStart(
        heni_kernel_t * ker,
        heni_packet_addr_t const * paddr,
        heni_iobuf_list_t * ppld
) HENI_API_FUNCT_DEC_SUFFIX;

/**
 * Notifies that sending of a HENI packet
 * has finished.
 * @param ker The HENI kernel.
 * @param paddr The addressing information for
 *   the packet. This object is valid only within
 *   the function.
 * @param ppld The payload of the packet. This
 *   This object is the same (both pointer-
 *   and content-wise) as the object passed when
 *   the sending was initiated. Its ownership
 *   is returned back by the HENI kernel.
 * @param psts The status of transmitting the packet.
 *   The object remains valid only until this
 *   function returns.
 */
HENI_EXT_FUNCT_DEC_PREFIX void heniPacketSendFinish(
        heni_kernel_t * ker,
        heni_packet_addr_t const * paddr,
        heni_iobuf_list_t * ppld,
        heni_packet_tx_info_t const * psts
) HENI_EXT_FUNCT_DEC_SUFFIX;

/**
 * Starts receiving a HENI packet.
 * The recipient of the event must gurantee that
 * the reception finish notification will be invoked
 * after the function has returned zero.
 * @param ker The HENI kernel.
 * @param paddr The addressing information for
 *   the packet. This object remains valid only
 *   until this function returns.
 * @param ppld The payload of the packet. The ownership
 *   of this object is transferred from the HENI kernel
 *   to the implementer of this function
 *   until the reception process has finished.
 *   In particular, the implementer may modify the
 *   object arbitrarily but guarantees that ultimately
 *   the object will return to its initial state.
 * @param psts The status of receiving the packet.
 *   The object remains valid only until this
 *   function returns.
 */
HENI_EXT_FUNCT_DEC_PREFIX int_fast8_t heniPacketReceiveStart(
        heni_kernel_t * ker,
        heni_packet_addr_t const * paddr,
        heni_iobuf_list_t * ppld,
        heni_packet_rx_info_t const * psts
) HENI_EXT_FUNCT_DEC_SUFFIX;

/**
 * Notifies the HENI kernel that a reception of
 * a HENI packet by has finished.
 * @param ker The HENI kernel.
 * @param ppld The payload of the packet. This
 *   This object must be the same (both pointer-
 *   and content-wise) as the object passed when
 *   the reception was initiated. Its ownership
 *   returns back to the HENI kernel.
 */
HENI_API_FUNCT_DEC_PREFIX void heniPacketReceiveFinish(
        heni_kernel_t * ker,
        heni_iobuf_list_t * ppld
) HENI_API_FUNCT_DEC_SUFFIX;



/* ---------------------------------------------------------------------- *
 *                                                                        *
 *                          Packet-allocation API                         *
 *                                                                        *
 * ---------------------------------------------------------------------- */

/**
 * Allocates a new HENI packet.
 * @param ker The HENI kernel.
 * @param iid The ID of the instance the
 *   packet is bound to.
 * @return A pointer to the allocated packet or NULL
 *   if there has been no memory to allocate the packet.
 */
HENI_EXT_FUNCT_DEC_PREFIX heni_packet_t * heniPacketAlloc(
        heni_kernel_t * ker,
        heni_instance_id_t iid
) HENI_EXT_FUNCT_DEC_SUFFIX;

/**
 * Frees a HENI packet allocated for
 * a given instance within a given kernel.
 * @param ker The HENI kernel.
 * @param iid The ID of the instance the
 *   packet is bound to.
 * @param packet The packet to be freed.
 */
HENI_EXT_FUNCT_DEC_PREFIX void heniPacketFree(
        heni_kernel_t * ker,
        heni_instance_id_t iid,
        heni_packet_t * packet
) HENI_EXT_FUNCT_DEC_SUFFIX;



/* ---------------------------------------------------------------------- *
 *                                                                        *
 *                        Kernel object accessors                         *
 *                                                                        *
 * ---------------------------------------------------------------------- */

/**
 * Returns the task scheduler associated with
 * a given HENI kernel.
 * @param ker The HENI kernel.
 * @return The scheduler associated with the kernel.
 */
HENI_INL_FUNCT_DEC_PREFIX heni_kernel_task_scheduler_t * heniKernelAccessorsGetSchedulerForKernel(
        heni_kernel_t * ker
) HENI_INL_FUNCT_DEC_SUFFIX;

/**
 * Returns the HENI kernel for which
 * a given task scheduler operates.
 * @param The scheduler.
 * @return ker The HENI kernel owning with the scheduler.
 */
HENI_INL_FUNCT_DEC_PREFIX heni_kernel_t * heniKernelAccessorsGetKernelForScheduler(
        heni_kernel_task_scheduler_t * ker
) HENI_INL_FUNCT_DEC_SUFFIX;



/* ---------------------------------------------------------------------- *
 *                                                                        *
 *                    Kernel instance object accessors                    *
 *                                                                        *
 * ---------------------------------------------------------------------- */

#include "detail/HENIKernelDetail.h"

#endif /* __HENI_KERNEL_H__ */

