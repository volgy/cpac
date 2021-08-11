/*******************************************************
Name: DSPicTypes.h
Date: 10/04/2010
Created By: Don Truex
Comments: 
Revision History:

*******************************************************/

#ifndef DSPIC_TYPES_H
#define DSPIC_TYPES_H

typedef  unsigned char UINT8;
typedef  signed char INT8;
typedef  unsigned int UINT16;
typedef  signed int INT16;
typedef  unsigned long UINT32;
typedef  signed long INT32;
typedef  unsigned long long UINT64;
typedef  signed long long INT64;
typedef  float FLOAT32;
typedef  long double FLOAT64;

#define  ECAN1_MSG_BUF_LENGTH 	32
typedef UINT16 ECAN1MSGBUF [ECAN1_MSG_BUF_LENGTH][8];
#endif





