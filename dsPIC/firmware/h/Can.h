/*******************************************************
Name: Can.h
Date: 11/11/2009
Created By: Don Truex
Comments:  Contains data structs and definitions for 
	CAN serial bus communication.
Revision History:

*******************************************************/
#ifndef _CAN_H_
#define _CAN_H_
#define CAN_MSG_DATA	0x01 // message type 
#define CAN_MSG_RTR		0x02 // data or RTR
#define CAN_FRAME_EXT	0x03 // Frame type
#define CAN_FRAME_STD	0x04 // extended or standard

/* CAN Baud Rate Configuration 		*/
#define FCAN  	40000000 //FCAN == Fcy == Fosc/2 40Mhz is max
#define BITRATE 1000000  
#define NTQ 	20		// 20 Time Quanta in a Bit Time
#define BRP_VAL		((FCAN/(2*NTQ*BITRATE))-1)

/* CAN Message Buffer Configuration */
//#define  ECAN1_MSG_BUF_LENGTH 	32
//typedef unsigned int ECAN1MSGBUF [ECAN1_MSG_BUF_LENGTH][8];
//extern ECAN1MSGBUF  ecan1msgBuf __attribute__((space(dma)));

/* message structure in RAM */
typedef struct{
	/* keep track of the buffer status */
	unsigned char buffer_status;
	/* RTR message or data message */
	unsigned char message_type;
	/* frame type extended or standard */
	unsigned char frame_type;
	/* buffer being used to reference the message */
	unsigned char buffer;
	/* 29 bit id max of 0x1FFF FFFF 
	*  11 bit id max of 0x7FF */
	unsigned long id; 
	/* message data */
	unsigned char data[8];	
	/* received message data length */
	unsigned char data_length;
}mID;


/* structure used to queue the FIFO messages in RAM */
typedef struct{
	/* keep track of the FIFO status */
	unsigned char FIFO_status;
	/* the structure of each message */ 
	mID FIFO[7];
}FIFO;

typedef struct{
	char cmd;
	int i1;
	int i2;
	int i3;
	int i4;
	int i5;
	int i6;
	int i7;
	int i8;
	unsigned int crc;
}CAN_PARAMS;

typedef struct{
	CAN_PARAMS in;
	CAN_PARAMS out;
}CAN_DATA;

#endif


