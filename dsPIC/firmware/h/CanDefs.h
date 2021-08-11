/*******************************************************
Name: CanDefs.h
Date: 1/05/2010
Created By: Don Truex
Comments: CAN module #define
Revision History:

*******************************************************/

#ifndef _CAN_DEFS_H
#define _CAN_DEFS_H

#ifndef uint16
	#define uint16 unsigned int
#endif

//The Standard Identifier (SID) Field contains 11 bits and makes up the entire Arbitration Field
//within a standard CAN message, Figure 4-1. This field is typically responsible for identifying the
//message contents and also assigning priority to the message. For this project, the SID has been
//divided into 3 subsections as suggested in the UNB Protocol, and are labeled: Priority Identifier
//(PID) Field, Mode Identifier (MID) Field, and the Node Identifier (NID), Figure 4-2.
#define ACKMSB 0x0100
#define ACKLSB 0x0001

//Priority identifier
#define PRIORITY_HI 0x0 //00  // high priority
#define PRIORITY_MED 0x200 //01 // medium priority
#define PRIORITY_LOW 0x400 //10 // low priority
//Mode Identifier
#define MID_FROM 0 //the NID contains the value of the node that sent the message.
#define MID_TO 0x100 //the NID contains the value of the node that the message is destined

//Accept messages from these NIDs 
//table 4-15
//MID = 0
#define NIDScnCmdFeed1 0x018
#define NIDLmnSensorFeed1 0x048
#define NIDLmnSensorFeed2 0x049
#define NIDLmnPackedMsg 0x04f

//These messages will be sent to (MID = 1) or 
//from (MID = 0) this NID
//Table 4-1
#define NIDVU_HCNCommandID 0x20
#define NIDVU_HCNDataFeed1 0x28
#define NIDVU_HCNDataFeed2 0x29
#define NIDVU_HCNDataFeed3 0x2a
#define NIDVU_HCNDataFeed4 0x2b
#define NIDVU_HCNDataFeed5 0x2c
#define NIDVU_HCNDataFeed6 0x2d
#define NIDVU_HCNDataFeed7 0x2e
#define NIDVU_HCNPackedMessage 0x2f

//transmit buffer index
#define VU_HCNDataFeed1Buf 0
#define VU_HCNDataFeed2Buf 1
#define VU_HCNDataFeed3Buf 2
#define VU_HCNDataFeed4Buf 3
#define VU_HCNDataFeed5Buf 4
#define VU_HCNDataFeed6Buf 5
#define VU_HCNDataFeed7Buf 6
#define VU_HCNCommandBuf 7

//transmit request register
#define VU_HCNDataFeed1BufTXEnable C1TR01CONbits.TXREQ0
#define VU_HCNDataFeed2BufTXEnable C1TR01CONbits.TXREQ1
#define VU_HCNDataFeed3BufTXEnable C1TR23CONbits.TXREQ2
#define VU_HCNDataFeed4BufTXEnable C1TR23CONbits.TXREQ3
#define VU_HCNDataFeed5BufTXEnable C1TR45CONbits.TXREQ4
#define VU_HCNDataFeed6BufTXEnable C1TR45CONbits.TXREQ5
#define VU_HCNDataFeed7BufTXEnable C1TR67CONbits.TXREQ6
#define VU_HCNCommandBufTXEnable C1TR67CONbits.TXREQ7
		
//parameter IDs from table 4-4
#define PIDKneeAngleSampleRate 0x0010
#define PIDKneeVelocitySampleRate 0x0011
#define PIDToeLoadSampleRate 0x0012
#define PIDHeelLoadSampleRate 0x0013
#define PIDSagitalPlaneMovementSampleRate 0x0014
#define PIDActivityModeSampleRate 0x0015
#define PIDGaitPhaseSampleRate 0x0016
#define PIDTorqueProfileSampleRate 0x0017

//system function codes from table 4-12
#define SFCResponseMask 0x80 //or this with the request code to indicate a response
#define SFCBindRequest	0x01
#define SFCGetDeviceInfo	0x02
#define SFCGetDeviceParameter	0x03
#define SFCSetDeviceParameter	0x04
#define SFCStart 0x50 // begin transmitting data over CAN port
#define SFCStop 0x51

#define SFCXferComplete 0x40 //Transmit the gP2P.in.p to the Main board via SPI
#define SFCMainCmd 0x41 //the next byte is a command intended for the main processor 
						// It will be parsed into gP2P.out.p.cmd
						//Valid commands are defined in P2PSpi.h


// hardware manipulation macros
#define LED_GREEN 		PORTAbits.RA8
#define LED_RED   		PORTBbits.RB4
#define TEST_PIN_RA0  	PORTAbits.RA0
#define LED_GREEN_ON 	PORTAbits.RA8 = 0
#define LED_RED_ON   	PORTBbits.RB4 = 0
#define TEST_PIN_HI  	PORTAbits.RA0 = 1
#define LED_GREEN_OFF 	PORTAbits.RA8 = 1
#define LED_RED_OFF   	PORTBbits.RB4 = 1
#define TEST_PIN_LOW  	PORTAbits.RA0 = 0


#endif

