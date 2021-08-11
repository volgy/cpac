/*******************************************************
Name: CanParse.c
Date: 9/27/2013
Created By: Don Truex
Comments: parse routines for CAN messages 
External function: CanParse()
Revision History:
1           10/7/2013           MG
Added description and comments and local data variable to CanParse0x8x().

Description:
    This code provides routines to receive data over
the CAN bus from Simulink (or another CAN interface).
Each CAN message contains 4 16-bit words, and is defined
in the function CanParse0x8y(), where y is between 0 and F.
For example, to send 8 16-bit words, you need to set up
CanParse0x80() and CanParse0x81().

    In order to define a message, you need to edit the
CanParse0x80() function to assign the data in CANdata[4]
to whatever global variables you want to receive from the
CAN bus (i.e., from Simulink). You can define these global
variables in Globals.h.

    In order to add another message (i.e., receive more data),
you need to make two edits:
 
1) Define a new CanParse0x8y() function, which should be exactly
   like CanParse0x80(), but with the data assigned to whatever
   other variables you want to receive from Simulink.
2) Add the case statement corresponding to the new CanParse0x8y() function
   to the CanParseBuf() function below.
3) See example code at bottom of file.

Do not edit the CanParse() function.
*******************************************************/


//#include "p33FJ64GS606.h"
//#include "P2PSpiSlave.h"
#include "p33fxxxx.h"
#include "Can.h"
#include "CanDefs.h"
#include "Defs.h"
#include "Externals.h"

extern void CanSend(uint16 ID, uint16 PID, uint16* pd);

void CanParseBuf(int buf);
int CanParse(void);
void CanParse0x80(unsigned int buf);
void CanParse0x82(UINT16 buf);

#define C1FIRSTRXBUF 8 //0-7 are transmit buffers

#define C1RX1START 8 //first rx buf
#define C1RX1END 15  //last rx buf
#define C1RX1MASK 0xff00

#define C1RX2START 0 //first rx buf
#define C1RX2END 15  //last rx buf
#define C1RX2MASK 0xffff

/* Do not edit the CanParse() function */
int CanParse(void) {
    int i, cnt;
    unsigned int m;

    cnt = 0;
    if (C1RXFUL1 & C1RX1MASK) {
        for (i = C1RX1START; i <= C1RX1END; i++) {
            m = 1 << i;
            if (C1RXFUL1 & m) {
                CanParseBuf(i);
                C1RXFUL1 &= ~m; //clear the flag
                cnt++;
                //return here if you only want to parse
                //1 message per call
                //return cnt;
            }
        }
    }
    if (C1RXFUL2 & C1RX2MASK) {
        for (i = C1RX2START; i <= C1RX2END; i++) {
            m = 1 << i;
            if (C1RXFUL2 & m) {
                CanParseBuf(i + C1RX2START);
                C1RXFUL2 &= ~m; //clear the flag
                //return here if you only want to parse
                //1 message per call
                //return cnt;
            }
        }
    }

    return cnt;
}

void CanParseBuf(int buf) {
    unsigned int id;
    id = ecan1msgBuf[buf][0] >> 2;
    switch (id) {
        case 0x80:
            CanParse0x80(buf);
            break;
            /* Add additional case statements for each additional
             message, from 0x80 through 0x8F, and corresponding CanParse0x8y()
             function below for each additional message. Example:
        case 0x81:
            CanParse0x81(buf);
            break; */
        // case 0x82:
          //  CanParse0x82(buf);
          //  break;
        default:
            break;
    }
}


void CanParse0x80(UINT16 buf) {

    /* Data in each CAN message is 4 16-bit words. First
     part of message is identifier and format, so actual
     data part of message is in elements 3-6 of the buffer array.*/

    // Define integer array of message data (note: all data unsigned)
//    UINT16 CANdata[4] ;

    /* This function provides examples for parsing CAN data from
     one CAN message into two 8-bit bytes, one 16-bit word,
     and one 32-bit word. */

    /* Note that these variables are defined locally for this example,
     but in general you will need to assign the data to global variables
     so it exists outside of this function. */

    UINT8 c1,c2; // define two 8-bit bytes
    INT16 i; // define a 16-bit word
    INT32 li; // define a 32-bit word

    // Read data from CAN message
    CANdata[0] = ecan1msgBuf[buf][3] ; 
    CANdata[1] = ecan1msgBuf[buf][4] ; 
    CANdata[2] = ecan1msgBuf[buf][5] ; 
    CANdata[3] = ecan1msgBuf[buf][6] ; 
    solenoid_input = CANdata[0];
    // Parse the first word into two 8-bit bytes
    c1 = (UINT8)CANdata[0];
    c2 = (UINT8)(CANdata[0] >> 8);

    /* IMPORTANT: Need to make sure data is defined in Simulink according
     to the way it is type cast below.*/

    // Second word is a 16-bit int
    i = (INT16)CANdata[1]; // Type cast to a signed integer.
    
    // Third and fourth words are low and high portions of 32 bit int
    li = (INT32)CANdata[2]; // low 16 bits
    li = li | ((INT32)CANdata[3]) << 16; // high 16 bits (shift and OR)
    /* These are type cast to signed long integer. */
}

///* Example code to read a position command from Simulink. You can
//   use this code by uncommenting it and replacing the CanParse0x80()
//   the function above with this function. You would also need to define
//   the position_command variable in Globals.h instead of here. */
//
//void CanParse0x80(UINT16 buf) {
//
//    // Define integer array of message data (note: all data unsigned)
//    UINT16 CANdata[4] ;
//
//    INT32 position_command; // This should actually be defined in Globals.h
//
//    /* IMPORTANT: Need to make sure data is defined in Simulink as a
//     long integer.*/
//
//    // Read data from CAN message
//    CANdata[0] = ecan1msgBuf[buf][3] ;
//    CANdata[1] = ecan1msgBuf[buf][4] ;
//    // Note that other two words in message are not used here.
//
//    // Assign low and high portions of 32 bit int
//    position_command = (INT32)CANdata[0]; // low 16 bits
//    position_command = position_command | ((INT32)CANdata[1]) << 16; // high 16 bits (shift and OR)
//}
//
//// End example code.

