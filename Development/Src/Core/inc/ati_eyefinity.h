///
///  Copyright (c) 2008 - 2012 Advanced Micro Devices, Inc.

///  THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF ANY KIND,
///  EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED
///  WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A PARTICULAR PURPOSE.

/// \file ati_eyefinity.h
#pragma once

#include <windows.h>
#include "adl/adl_sdk.h"
#include <stdio.h>

extern int setAdapterDisplaysToEyefinity(int iAdapterIndex, int iRows, int iColumns, int iDisplayMapIndexes[], int iNumOfDisplays, int iSLSRearrange);
extern int setBezelOffsets(int iAdapterIndex, int iHbezel, int iVbezel);
extern int setResolution(int iAdapterIndex, int iXRes, int iYRes);
extern int setPrimaryAdapter(int iAdapterIndex);
extern int setAdapterDisplaysToDisable(int iAdapterIndex);
extern int setAdapterDisplaysToClone(int iAdapterIndex, int iDisplayIndexes[], int iDisplaysCount);
extern int disableAdapterEyefinityMapping(int iAdapterIndex);
extern int printDisplayIndexes();
extern int getDisplayIndexesofOneGPU(int iCurrentAdapterIndex, int *lpAdpaterIndexes, int *lpDisplayIndexes, int *lpCount);
extern int CanSetPrimary(int iAdapterIndex, int iCurrentPrimaryAdapterIndex);
extern int initializeADL();
extern void deinitializeADL();
extern void printSyntax();