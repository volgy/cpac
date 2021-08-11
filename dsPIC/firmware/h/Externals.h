/*******************************************************
Name: Externals.h
Date: 5/25/2010
Created By: Don Truex
Comments:  Define external variables. External and global are
 essentially the same thing, but only main.c includes
 global variables, so all other source files that use those global
 variables include them as external variables.
Revision History:
rev number  date 		by

*****************************************************/
#ifndef __EXTERNALS_H__
#define __EXTERNALS_H__
#include "Defs.h"
extern UINT16 gTimers[MAXTIMERS];
extern UINT32 gRunTime;
extern UINT16 gData[4] ; // CANmessage array for CanSend0x20()
extern UINT16 CANdata[4];
extern INT16 quad_read;
extern INT16 solenoid_input;

extern ECAN1MSGBUF ecan1msgBuf __attribute__((space(dma),aligned(ECAN1_MSG_BUF_LENGTH*16)));
#endif
