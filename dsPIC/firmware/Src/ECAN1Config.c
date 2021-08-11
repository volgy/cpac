/**********************************************************************
* © 2005 Microchip Technology Inc.
*
* FileName:        main.c
* Dependencies:    Header (.h) files if applicable, see below
* Processor:       dsPIC33Fxxxx
* Compiler:        MPLAB® C30 v3.00 or higher
*
* SOFTWARE LICENSE AGREEMENT:
* Microchip Technology Incorporated ("Microchip") retains all ownership and 
* intellectual property rights in the code accompanying this message and in all 
* derivatives hereto.  You may use this code, and any derivatives created by 
* any person or entity by or on your behalf, exclusively with Microchip's
* proprietary products.  Your acceptance and/or use of this code constitutes 
* agreement to the terms and conditions of this notice.
*
* CODE ACCOMPANYING THIS MESSAGE IS SUPPLIED BY MICROCHIP "AS IS".  NO 
* WARRANTIES, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT NOT LIMITED 
* TO, IMPLIED WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY AND FITNESS FOR A 
* PARTICULAR PURPOSE APPLY TO THIS CODE, ITS INTERACTION WITH MICROCHIP'S 
* PRODUCTS, COMBINATION WITH ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION. 
*
* YOU ACKNOWLEDGE AND AGREE THAT, IN NO EVENT, SHALL MICROCHIP BE LIABLE, WHETHER 
* IN CONTRACT, WARRANTY, TORT (INCLUDING NEGLIGENCE OR BREACH OF STATUTORY DUTY), 
* STRICT LIABILITY, INDEMNITY, CONTRIBUTION, OR OTHERWISE, FOR ANY INDIRECT, SPECIAL, 
* PUNITIVE, EXEMPLARY, INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, FOR COST OR EXPENSE OF 
* ANY KIND WHATSOEVER RELATED TO THE CODE, HOWSOEVER CAUSED, EVEN IF MICROCHIP HAS BEEN 
* ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE.  TO THE FULLEST EXTENT 
* ALLOWABLE BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN ANY WAY RELATED TO 
* THIS CODE, SHALL NOT EXCEED THE PRICE YOU PAID DIRECTLY TO MICROCHIP SPECIFICALLY TO 
* HAVE THIS CODE DEVELOPED.
*
* You agree that you are solely responsible for testing the code and 
* determining its suitability.  Microchip has no obligation to modify, test, 
* certify, or support the code.
*
* REVISION HISTORY:
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* Author          	Date      Comments on this revision
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* Vinaya Skanda 	10/18/06  First release of source file
* Vinaya Skanda		07/25/07  Updates from Joe Supinsky and Jatinder Gharoo incorporated
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*
* ADDITIONAL NOTES:
* This code is tested on Explorer-16 board with ECAN PICTail Card.
* The device used is dsPIC33FJ256GP710 controller 
*************************************************************************************************/


#include "p33fxxxx.h"
#include "Externals.h"
#include "ECAN1Config.h"

/* Dma Initialization for ECAN1 Transmission */

void InitDma3(void)
{
    DMA3CONbits.CHEN = 0;
    //read from ram write to peripheral
    DMA3CONbits.DIR = 1;
    /* DMA Addressing Mode: Peripheral Indirect Addressing mode */
    DMA3CONbits.AMODE = 0x2;
    /* Operating Mode: Continuous, Ping-Pong modes disabled */
    DMA3CONbits.MODE = 0x0;
    /* Assign ECAN1 Transmit event for DMA Channel 0 */
    DMA3REQ = 0x0046;
    /* Set Number of DMA Transfer per ECAN message to 8 words */
    DMA3CNT = 7;
    /* Peripheral Address: ECAN1 Transmit Register */
    DMA3PAD = (int) &C1TXD;
    /* Start Address Offset for ECAN1 Message Buffer 0x0000 */
    DMA3STA = __builtin_dmaoffset(ecan1msgBuf);
    /* Channel Enable: Enable*/
    DMA3CONbits.CHEN = 1;
}

/* Dma Initialization for ECAN1 Reception */
void InitDma2(void)
{

    DMACS0 = 0;//clear write collision flags
    DMA2CON = 0x0020;
    DMA2PAD = (int) &C1RXD;//0x0440; /* ECAN 1 (C1RXD) */
    DMA2CNT = 0x0007;
    DMA2REQ = 0x0022; /* ECAN 1 Receive */
    DMA2STA = __builtin_dmaoffset(ecan1msgBuf);
    DMA2CONbits.CHEN = 1;

}

void Ecan1ClkInit(void)
{

    /* Synchronization Jump Width set to 4 TQ */
    C1CFG1bits.SJW = 0x3;
    /* Baud Rate Prescaler */
    C1CFG1bits.BRP = BRP_VAL;
    /* Phase Segment 1 time is 8 TQ */
    C1CFG2bits.SEG1PH = 0x7;
    /* Phase Segment 2 time is set to be programmable */
    C1CFG2bits.SEG2PHTS = 0x1;
    /* Phase Segment 2 time is 6 TQ */
    C1CFG2bits.SEG2PH = 0x5;
    /* Propagation Segment time is 5 TQ */
    C1CFG2bits.PRSEG = 0x4;
    /* Bus line is sampled three times at the sample point */
    C1CFG2bits.SAM = 0x1;
}

void Ecan1Init(void)
{

    /* Request Configuration Mode */
    C1CTRL1bits.REQOP = 4;
    while (C1CTRL1bits.OPMODE != 4);

    Ecan1ClkInit();

    C1FCTRLbits.FSA = 0b01000; /* FIFO Starts at Message Buffer 8 */
    C1FCTRLbits.DMABS = 0b110; /* 32 CAN Message Buffers in DMA RAM */


    /* Enable Window to Access Acceptance Filter Registers */
C1CTRL1bits.WIN=0x1;
///Filter 0 to use Mask0
C1FMSKSEL1bits.F0MSK=0x0;
//Set mask 0 to "don't care" for 4 LSBs
C1RXM0SIDbits.SID = 0b11111110000 ; //SIDs are 11 bits
//Set the SID here.  This shold receive all message from 0x80 to 0x8f
C1RXF0SIDbits.SID = 0x080;
//Set to check only standard IDs
C1RXM0SIDbits.MIDE = 0x1;
//If MIDE = 1, 0=match only SID, 1=match EXID
C1RXF0SIDbits.EXIDE= 0x0;
// Filter 0 writes messages to FIFO
C1BUFPNT1bits.F0BP = 0xf;
// Enable Filter 0
C1FEN1bits.FLTEN0=0x1;
/* Clear Window Bit to Access ECAN Control Registers */
C1CTRL1bits.WIN=0x0;

    /* Enter Normal Mode */
    C1CTRL1bits.REQOP = 0;
    while (C1CTRL1bits.OPMODE != 0);

    /* ECAN transmit/receive message control */

    C1RXFUL1 = C1RXFUL2 = C1RXOVF1 = C1RXOVF2 = 0x0000;
    C1TR01CONbits.TXEN0 = 1; /* ECAN1, Buffer 0 is a Transmit Buffer */
    C1TR01CONbits.TXEN1 = 1; /* ECAN1, Buffer 0 is a Transmit Buffer */
    C1TR23CONbits.TXEN2 = 1; /* ECAN1, Buffer 0 is a Transmit Buffer */
    C1TR23CONbits.TXEN3 = 1; /* ECAN1, Buffer 0 is a Transmit Buffer */
    C1TR45CONbits.TXEN4 = 1; /* ECAN1, Buffer 0 is a Transmit Buffer */
    C1TR45CONbits.TXEN5 = 1; /* ECAN1, Buffer 0 is a Transmit Buffer */
    C1TR67CONbits.TXEN6 = 1; /* ECAN1, Buffer 0 is a Transmit Buffer */
    C1TR67CONbits.TXEN7 = 1; /* ECAN1, Buffer 0 is a Transmit Buffer */

    C1TR01CONbits.TX0PRI = 0b11; /* Message Buffer 0 Priority Level */
    C1TR01CONbits.TX1PRI = 0b11; /* Message Buffer 0 Priority Level */
    C1TR23CONbits.TX2PRI = 0b11; /* Message Buffer 0 Priority Level */
    C1TR23CONbits.TX3PRI = 0b11; /* Message Buffer 0 Priority Level */
    C1TR45CONbits.TX4PRI = 0b11; /* Message Buffer 0 Priority Level */
    C1TR45CONbits.TX5PRI = 0b11; /* Message Buffer 0 Priority Level */
    C1TR67CONbits.TX6PRI = 0b11; /* Message Buffer 0 Priority Level */
    C1TR67CONbits.TX7PRI = 0b11; /* Message Buffer 0 Priority Level */

    InitDma3();
    InitDma2();
}


