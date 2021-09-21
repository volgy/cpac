/*
 * Copyright (c) 2021 Vanderbilt University
 * All rights reserved.
 *
 * Permission to use, copy, modify, and distribute this software and its
 * documentation for any purpose, without fee, and without written agreement is
 * hereby granted, provided that the above copyright notice, the following
 * two paragraphs and the author appear in all copies of this software.
 * 
 * IN NO EVENT SHALL THE VANDERBILT UNIVERSITY BE LIABLE TO ANY PARTY FOR
 * DIRECT, INDIRECT, SPECIAL, INCIDENTAL, OR CONSEQUENTIAL DAMAGES ARISING OUT
 * OF THE USE OF THIS SOFTWARE AND ITS DOCUMENTATION, EVEN IF THE VANDERBILT
 * UNIVERSITY HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * 
 * THE VANDERBILT UNIVERSITY SPECIFICALLY DISCLAIMS ANY WARRANTIES,
 * INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY
 * AND FITNESS FOR A PARTICULAR PURPOSE.  THE SOFTWARE PROVIDED HEREUNDER IS
 * ON AN "AS IS" BASIS, AND THE VANDERBILT UNIVERSITY HAS NO OBLIGATION TO
 * PROVIDE MAINTENANCE, SUPPORT, UPDATES, ENHANCEMENTS, OR MODIFICATIONS.
 *
 * @author Peter Volgyesi <peter.volgyesi@vanderbilt.edu>
 */

#include "xc.h"
#include "I2C.h"

void I2C_Init()
{
    // Disabled, master mode, defaults
 	I2C1CONbits.I2CEN = 0;
	I2C1CONbits.I2CSIDL = 0;
	I2C1CONbits.SCLREL = 1;
	I2C1CONbits.IPMIEN = 0;
	I2C1CONbits.A10M = 0;
	I2C1CONbits.DISSLW = 1;
	I2C1CONbits.SMEN = 0;
	I2C1CONbits.GCEN = 0;
	I2C1CONbits.STREN = 0;
	I2C1CONbits.ACKDT = 0;
	I2C1CONbits.ACKEN = 0;
	I2C1CONbits.RCEN = 0;
	I2C1CONbits.PEN = 0;
	I2C1CONbits.RSEN = 0;
	I2C1CONbits.SEN = 0;
	
	// Clearing the receive and transmit buffers, and any pending interrupts
	I2C1RCV = 0x0000;
	I2C1TRN = 0x0000;
    IEC1bits.MI2C1IE = 0;
    IFS1bits.MI2C1IF = 0;
    
    // Baud rate: 400kHz, assuming 40MHz Fcy
    // (FCY/FSCL - FCY/1e7) - 1
    I2C1BRG = 95;       
	
	// Enable I2C master
	I2C1CONbits.I2CEN = 1;
}

void I2C_Idle()
{
    while(I2C1CONbits.SEN || I2C1CONbits.PEN || I2C1CONbits.RCEN || 
            I2C1CONbits.RSEN || I2C1CONbits.ACKEN || I2C1STATbits.TRSTAT);
}

void I2C_Start()
{
	I2C1CONbits.SEN = 1;
	I2C_Idle();
}

void I2C_ReStart()
{
	I2C1CONbits.RSEN = 1;
	I2C_Idle();
}

void I2C_Stop()
{
	I2C1CONbits.PEN = 1;
    I2C_Idle();
}

int I2C_TxByte(UINT8 data)
{
	I2C1TRN = data;
    
	if(I2C1STATbits.IWCOL)
	{
		return I2C_ERR;
	}
	while(I2C1STATbits.TRSTAT);  
    I2C_Idle();
    
    if ( I2C1STATbits.ACKSTAT ) {
        return I2C_ERR;
    }
	
    return I2C_OK;
}

UINT8 I2C_RxByte()
{
	I2C1CONbits.RCEN = 1;
    while(I2C1CONbits.RCEN);
    
    I2C1STATbits.I2COV = 0;
    
    I2C_Idle();
	
    return I2C1RCV;
}

void I2C_RxBytes(size_t length, UINT8* data)
{
    while(length)
    {
        I2C1CONbits.RCEN = 1;
        while(!I2C1STATbits.RBF);
        *data = I2C1RCV;            /* save byte received */
        data++;
        length--;
        if(length == 0)              /* If last char, generate NACK sequence */
        {
            I2C1CONbits.ACKDT = 1;
            I2C1CONbits.ACKEN = 1;
        }
        else                         /* For other chars,generate ACK sequence */
        {
            I2C1CONbits.ACKDT = 0;
            I2C1CONbits.ACKEN = 1;
        }
        while(I2C1CONbits.ACKEN == 1);    /* Wait till ACK/NACK sequence 
                                                 is over */
    }
}