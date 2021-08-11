/*******************************************************
Name: Globals.h
Date: 5/25/2010
Created By: Don Truex
Comments: Define global variables
Revision History:

*******************************************************/
#ifndef __GLOBALS_H__
#define __GLOBALS_H__
#include "Defs.h"

UINT16 gTimers[MAXTIMERS]; // timer array ticks at 1 ms (see Timers.c)
// Note: the gTimer array is used for relatively slow events
// e.g., lighting LEDs
UINT32 gRunTime; // counts total runtime between resets in ms
UINT16 gData[4] ; // data array for CanSend0x20()
UINT16 CANdata[4];
ECAN1MSGBUF ecan1msgBuf __attribute__((space(dma),aligned(ECAN1_MSG_BUF_LENGTH*16)));
INT16 quad_read;
INT16 AN_pin27;
INT16 AN_pin28;
INT16 AN_pin29;
INT16 solenoid_input;
UINT16 i;
UINT32 ii;
UINT16 prev;
UINT16 solenoid_type;
#endif
