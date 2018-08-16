/*
 * HENI: Hierarchical Embedded Network Infrastructure
 *
 * Copyright (c) 2016, University of Warsaw
 * All rights reserved.
 *
 */
#ifndef __HENI_DEBUG_H__
#define __HENI_DEBUG_H__

#include "HENIBase.h"

/**
 * @file
 * HENI: Debugging functionality.
 * @author Konrad Iwanicki <iwanicki@mimuw.edu.pl>
 */

#ifdef HENI_DEBUG
#define HENI_DLOGS() heniDebugStartMsg()
#define HENI_DLOGC(msg...) heniDebugContinueMsg(msg)
#define HENI_DLOGF() heniDebugFinishMsg()
#else
#define HENI_DLOGS()
#define HENI_DLOGC(msg...)
#define HENI_DLOGF()
#endif

#define HENI_DLOG(msg...) do { HENI_DLOGS(); HENI_DLOGC(msg); HENI_DLOGF(); } while (0)


/**
 * Starts sending a message to a debug channel.
 */
HENI_EXT_FUNCT_DEC_PREFIX void heniDebugStartMsg() HENI_EXT_FUNCT_DEC_SUFFIX;

/**
 * Continues sending a message to a debug channel.
 * @param msg The message to be sent to a debug channel.
 */
HENI_EXT_FUNCT_DEC_PREFIX void heniDebugContinueMsg(char const * dbgMsg, ...) HENI_EXT_FUNCT_DEC_SUFFIX;

/**
 * Finishes sending a message to a debug channel.
 */
HENI_EXT_FUNCT_DEC_PREFIX void heniDebugFinishMsg() HENI_EXT_FUNCT_DEC_SUFFIX;


#endif /* __HENI_DEBUG_H__ */

