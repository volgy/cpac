/*******************************************************
Name: CanSend.c
Date: 1/05/2010
Created By: Don Truex
Comments: Build and Transmit CAN messages
External function: CanSend()
Revision History:
rev number  date 		by
1           10/1/2013           MG
Added description and comments, in addition to local variables
to CanSend0x2x() functions to simplify editing.

Description:
    This code provides routines to send information over
the CAN bus to Simulink (or to another CAN interface).
Each CAN message contains 4 16-bit words, and is defined
in the function CanSend0x2x(), where x is between 0 and F.
For example, to send 8 16-bit words, you need to set up
CanSend0x20() and CanSend0x21(). A single message can be sent
with the message-specific CanSend0x2x() function, with the
argument-specified CanSend(message_ID) function, or all
messages can be sent at a specified rate with the CanSendAll()
function, where the rate is specified in that function. Note
that the user must define the order of each message in
CanSend0x2x(), must define a new message for each additional
4 word message, must add each CanSend0x2x() function to
CanSendAll(), and must define the rate of data transmission
in CanSendAll().
*******************************************************/

#include "p33fxxxx.h"
#include "Can.h"
#include "CanDefs.h"
#include "Externals.h"

extern void ecan1WriteTxMsgBufId(unsigned int buf, long txIdentifier, unsigned int ide, unsigned int remoteTransmit);
extern void ecan1WriteTxMsgBufData(unsigned int buf, unsigned int dataLength, unsigned int data1, unsigned int data2, unsigned int data3, unsigned int data4);

//extern P2P_CAN_SLAVE_DATA gP2P,gP2PParse;

void CanSend(uint16 ID);
void CanSend0x20(void);
void CanSend0x21(void);
//void CanSend0x22(void);
//void CanSend0x23(void);

void CanSendAll(void);

/* The user must define the data rate in CanSendAll(), and must also
 include the CanSend0x2x() corresponding to each message to be sent. */

void CanSendAll(void)
{
    if(gTimers[CANSEND] == 0)
    {
        /* This line defines the rate at which the CAN message
         is sent. */
        gTimers[CANSEND] = 4;   // Send message every 4 msec (250 Hz)
/* The user must enter the line for each CanSend0x2x message here. */
        CanSend0x20(); // Currently sending only message ID 0x20
        CanSend0x21();
        // Any additional messages entered here (e.g., CanSend0x21();)
    }
}

/* This function sends the CAN message as given by its function
 * argument (e.g., CanSend(0x20) sends the CAN message with the
 * message ID 20), as defined below. The user must add a case
 * statement for each corresponding message
 * (e.g., for a message 22, add the case 0x22). */

void CanSend(uint16 ID)
{
    switch (ID & 0x00ff)
    {
        case 0x20:
            CanSend0x20();
            break;
        case 0x21:
            CanSend0x21();
            break;
        default:
            break;
    }

}

/* Each CAN message is an array containing 4 16-bit unsigned integers.
   The user must define the information contained in each
   message as in the function below. The user can (in this
   file) define up to 16 messages, 0x20 through 0x2F. */

/* Signed numbers should be scaled and offset prior to sending
 over the CAN bus. */

void CanSend0x20(void)
{
    // Define message ID corresponding to function
    UINT16 MessageID = 0x20 ; // message ID
    // Length of CAN message in bytes. Max length is 8. If sending
    // less than 8, still need to have 6 arguments in ecan1WriteTXMsgBufData()
    UINT16 Message_length = 8 ; // send 4 bytes (= 2 words)
    // UINT16 data[4] ;
    // Define UINT16 data[2] in Globals.h and Externals.h

    /* Define message to be sent. The elements of the message
       should be global variables, which can be defined in Globals.h.
     * A 32-bit word can be sent by first sending the high 16 bits,
     * then the low 16 bits, then reassembling on the other end. As as
     * example, see gRunTime below. */
    while (VU_HCNDataFeed1BufTXEnable == 1)
        VU_HCNDataFeed1BufTXEnable = 0; // abort the request.
    ecan1WriteTxMsgBufId(VU_HCNDataFeed1Buf, PRIORITY_HI | MID_FROM | MessageID, 0, 0);
// 8 -> 4 and 2 and 3 should be 0
    ecan1WriteTxMsgBufData(VU_HCNDataFeed1Buf, Message_length,
            gData[0],gData[1],gData[2],gData[3]);

      VU_HCNDataFeed1BufTXEnable = 1;

}
void CanSend0x21(void)
{
    // Define message ID corresponding to function
    UINT16 MessageID = 0x21 ; // message ID
    // Length of CAN message in bytes. Max length is 8. If sending
    // less than 8, still need to have 6 arguments in ecan1WriteTXMsgBufData()
    UINT16 Message_length = 4 ; // send 4 bytes (= 2 words)
    // UINT16 data[4] ;
    // Define UINT16 data[2] in Globals.h and Externals.h

    /* Define message to be sent. The elements of the message
       should be global variables, which can be defined in Globals.h.
     * A 32-bit word can be sent by first sending the high 16 bits,
     * then the low 16 bits, then reassembling on the other end. As as
     * example, see gRunTime below. */
    while (VU_HCNDataFeed1BufTXEnable == 1)
        VU_HCNDataFeed1BufTXEnable = 0; // abort the request.
    ecan1WriteTxMsgBufId(VU_HCNDataFeed1Buf, PRIORITY_HI | MID_FROM | MessageID, 0, 0);
// 8 -> 4 and 2 and 3 should be 0
    ecan1WriteTxMsgBufData(VU_HCNDataFeed1Buf, Message_length,
            gData[0],gData[1],gData[2],0);

      VU_HCNDataFeed1BufTXEnable = 1;

}












//void CanSend0x21(void)
//{
//    // Define message ID corresponding to function
//    UINT16 MessageID = 0x21 ; // message ID
//    // Length of CAN message in bytes. Max length is 8. If sending
//    // less than 8, still need to have 6 arguments in ecan1WriteTXMsgBufData()
//    UINT16 Message_length = 8 ; // length of message in bytes, max 8
//    UINT16 CANmessage[4] ;
//
//    /* Define message to be sent. The elements of the message
//       should be global variables, which can be defined in Globals.h */
//    CANmessage[0] = 500; // Sending test numbers back in message 21
//    CANmessage[1] = 600;
//    CANmessage[2] = 700;
//    CANmessage[3] = 800;
//
//    while (VU_HCNDataFeed1BufTXEnable == 1)
//        VU_HCNDataFeed1BufTXEnable = 0; // abort the request.
//    ecan1WriteTxMsgBufId(VU_HCNDataFeed1Buf, PRIORITY_HI | MID_FROM | MessageID, 0, 0);
//    ecan1WriteTxMsgBufData(VU_HCNDataFeed1Buf, Message_length,
//            CANmessage[0],
//            CANmessage[1],
//            CANmessage[2],
//            CANmessage[3]);
//
//      VU_HCNDataFeed1BufTXEnable = 1;
//
//}










