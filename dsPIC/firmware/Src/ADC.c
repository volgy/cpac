/*--------------------------------------------------------
----------------------------------------------------------
Name: ADC.c
Date: 9/27/2013
Created By: Don Truex
Revision History:
    MG 10/10/2013, Added ADCRead() function,
    disabled interrupt driven sampling function.

Description: Provides functions for initialization and
usage of software-triggered A/D converters 0-3 on dsPIC.

Function list:
ADCInit()          Initializes A/D converters
ADCRead()          Function to read from single A/D channel

References:
Section 22 of the dsPIC datasheet DS70591E
----------------------------------------------------------
--------------------------------------------------------*/
#include "p33fxxxx.h"
#include "Externals.h"
#include "CanDefs.h"

void ADCInit(void);
INT16 ADCRead(INT16 chan);

/*------------------------------------------------------
NAME
    ADCInit()

SYNOPSIS
    void ADCInit(void)

DESCRIPTION
Initializes A/D register pairs 0 and 1. Pair 0 corresponds 
to ADC0 and ADC1, while pair 1 corresponds to ADC2 and ADC3.
This function should be called once prior to using the ADC.
--------------------------------------------------------*/

void ADCInit(void)
{
    INT16 i;

    ADCONbits.FORM = 0; // Output in Integer Format
    ADCONbits.EIE = 0; // Enable Early Interrupt is disabled
    ADCONbits.ORDER = 0; // Normal Order of conversion
    ADCONbits.SEQSAMP = 0; // Simultaneous sampling
    ADCONbits.ASYNCSAMP = 1; // Sample continuously until trigger
    ADCONbits.SLOWCLK = 0; // High Frequency Clock input

    ADCONbits.ADCS = 0b011; // Clock divider selection 80/4 = 20Meg
    //ADCONbits.ADCS = 0b111; // Clock divider selection 80/8 = 10Meg

    ADCONbits.GSWTRG = 1 ; // Set global software trigger (see p. 315)
    
    //set channels 0 through 3 as analog input channels
    ADPCFG = 0xfff0; //1 = digital, 0 = analog
    ADPCFG2 = 0xffff; //all digital

    ADCONbits.ADON = 1; // Enable ADC module
    
    ADPCFGbits.PCFG8 = 0; //Added by PRS 4/23 to read in AN8
    ADPCFGbits.PCFG9 = 0; //Added by PRS 4/23 to read in AN9
    ADPCFGbits.PCFG10 = 0; //Added by PRS 4/23 to read in AN10    

    for (i = 0; i < 10000; i++); //wait for adc module to stabilize after enabling
    
    ADCPC0bits.TRGSRC0 = 0b00001; // Set P0 for software trigger (see p. 322)
    ADCPC0bits.TRGSRC1 = 0b00001; // Set P1 for software trigger (see p. 321)
    
    
    ADCPC2bits.TRGSRC4 = 0b00001; // Trigger for AN8+AN9 for accelerometer
    ADCPC2bits.TRGSRC5 = 0b00001; // Trigger for AN10 for accelerometer

}

/*------------------------------------------------------
NAME
    ADCRead()

SYNOPSIS
    UINT16 ADCRead(UINT16 chan)

DESCRIPTION
Perform A/D conversion for specified channel (between 0 and 3).
Returned value is an unsigned integer where 0 volts
corresponds to 0 in the A/D register and 3.3 volts corresponds
to 2^10 (1024), since the dsPIC has 10-bit A/D converters.
The A/D will saturate at levels below 0 or above 3.3 v.

EXAMPLE USAGE

 UINT16 chan = 0;
 UINT16 data[5000] ;
 UINT16 i ;

 // Read 5 seconds of data at 1 kHz
 // Assumes TMR1 set with period of 1 ms
 for (i=0 ; i<5000 ; i++) {
	if (IFS0bits.T1IF == 1) { // TMR1 has ticked
            IFS0bits.T1IF = 0; // reset TMR1
            data[i] = ADCRead(chan);
        }
 } // end for
 
 * --------------------------------------------------------*/

INT16 ADCRead(INT16 chan)
{
INT16 data ;

switch (chan)
{
    case 0:
        ADCPC0bits.SWTRG0 = 1 ; // Trigger A/D conversion on pair 0
        while (ADSTATbits.P0RDY != 1) { // Wait for conversion to finish
        }
        data = ADCBUF0 ; // Read ADC0
        ADSTATbits.P0RDY = 0 ; //Reset pair 0 ready flag
        //probably don't need to reset
        //what's difference between ADCPC0bits.PEND0 and ADSTATbits.P0RDY?
        break ;

    case 1:
        ADCPC0bits.SWTRG0 = 1 ; // Trigger A/D conversion on pair 0
        while (ADSTATbits.P0RDY != 1) { // Wait for conversion to finish
        }
        data = ADCBUF1 ; // Read ADC1
        ADSTATbits.P0RDY = 0 ; //Reset pair 0 ready flag
        break ;

    case 2:
        ADCPC0bits.SWTRG1 = 1 ; // Trigger A/D conversion on pair 1
        while (ADSTATbits.P1RDY != 1) { // Wait for conversion to finish
        }
        data = ADCBUF2 ; // Read ADC2
        ADSTATbits.P1RDY = 0 ; //Reset pair 1 ready flag
        break ;

    case 3:
        ADCPC0bits.SWTRG1 = 1 ; // Trigger A/D conversion on pair 1
        while (ADSTATbits.P1RDY != 1) { // Wait for conversion to finish
        }
        data = ADCBUF0 ; // Read ADC3
        ADSTATbits.P1RDY = 0 ; //Reset pair 1 ready flag
        break ;
        
    case 8:        
        ADCPC2bits.SWTRG4 = 1 ; // Trigger A/D conversion 
        while (ADSTATbits.P4RDY != 1) { // Wait for conversion to finish
        }
        data = ADCBUF8 ; // Read ADC8
        ADSTATbits.P4RDY = 0 ; //Reset pair 1 ready flag
        break ;
        
    case 9:
        ADCPC2bits.SWTRG4 = 1 ; // Trigger A/D conversion 
        while (ADSTATbits.P4RDY != 1) { // Wait for conversion to finish
        }
        data = ADCBUF9 ; // Read ADC9
        ADSTATbits.P4RDY = 0 ; //Reset pair 1 ready flag
        break ;
        
    case 10:        
        ADCPC2bits.SWTRG5 = 1 ; // Trigger A/D conversion 
        while (ADSTATbits.P5RDY != 1) { // Wait for conversion to finish
        }
        data = ADCBUF10 ; // Read ADC10
        ADSTATbits.P5RDY = 0 ; //Reset pair 1 ready flag
        break ;
        
    default:
        break ;
}

return(data) ;

} /* End Read_ADC() */


