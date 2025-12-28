//==============================================================================
//
// Title:		Frequency.h
// Purpose:		A short description of the interface.
//
// Created on:	11/5/2020 at 5:01:06 PM by .
// Copyright:	. All Rights Reserved.
//
//==============================================================================

#ifndef __Frequency_H__
#define __Frequency_H__

#ifdef __cplusplus
    extern "C" {
#endif

//==============================================================================
// Include files

#include "cvidef.h"

//==============================================================================
// Constants
		
#define USER_EVENT         1000
#define MYEVENT_CLOSE      (USER_EVENT+1)

//==============================================================================
// Types

//==============================================================================
// External variables
extern double sampleRate;
extern int npoints;
extern double *waveData;
//==============================================================================
// Global functions

int Declare_Your_Functions_Here (int x);

#ifdef __cplusplus
    }
#endif

#endif  /* ndef __Frequency_H__ */
