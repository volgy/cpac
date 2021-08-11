/*******************************************************
Name: ADC_Read.c
Date: 9/27/2013
Created By: Don Truex
Comments: Function for A/D conversion
Revision History:
*********************************************************/
#include "p33fxxxx.h"
#include "Externals.h"
#include "CanDefs.h"

// change function name to avoid duplicate names

void ADCInit(void);

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

    //set an0 and an2 as analog channels
    ADPCFG = 0xfffa; //1 = digital, 0 = analog
    ADPCFG2 = 0xffff; //all digital

    ADCONbits.ADON = 1; // Enable ADC module

    for (i = 0; i < 10000; i++); //wait for adc module to stabalize after enabling

    IPC27bits.ADCP0IP = 0x07; // Set ADC Pair 0 Interrupt Priority (Level 1)
    IFS6bits.ADCP0IF = 0; // Clear ADC Pair 0 Interrupt Flag
    IEC6bits.ADCP0IE = 1;

    IPC27bits.ADCP1IP = 0x04; // Set ADC Pair 1 Interrupt Priority (Level 2)
    IFS6bits.ADCP1IF = 0; // Clear ADC Pair 1 Interrupt Flag
    IEC6bits.ADCP1IE = 1; // Enable ADC Pair 1 Interrupt

    //Set trigger sources
    ADCPC0bits.TRGSRC0 = 0b11111; // ANO TMR2 period match
    ADCPC0bits.TRGSRC1 = 0b01100; //AN1 TMR1 period match

    /* Set these to 1 if you want interrupt-based sampling of 
     ADC 0 and 1, in which case the PIC will call the following two
     ISRs at the sampling rate established by TMR1 and TMR2. */
//    IEC6bits.ADCP0IE = 1; // Enable ADC Pair 0 Interrupt
//    IEC6bits.ADCP1IE = 1; // Enable ADC Pair 1 Interrupt
    
     /*Set these to 0 to perform a manual read of these two ADCs. */

    // Pair 0 is channels 0 and 1
    // Pari 1 is channels 2 and 3

    IEC6bits.ADCP0IE = 0; // Disable ADC Pair 0 Interrupt
    IEC6bits.ADCP1IE = 0; // Disable ADC Pair 1 Interrupt
}

/* Function to read from ADC channel 0 or 1 */

INT16 Read_ADC(INT16 chan)
{
INT16 data ;
// figure this out
switch (chan) {
    case: 0
        if(IFS6bits.ADCP0IF == 1) { // ADC0 ready to read
        data = ADCBUF0 ; // Read ADC0
        IFS6bits.ADCP0IF = 0; // Reset ADC0 ready flag
        }
        break;
    case: 2
        if(IFS6bits.ADCP1IF == 1) { // ADC1 ready to read
        data = ADCBUF2 ; // Read ADC1
        IFS6bits.ADCP1IF = 0; // Reset ADC1 ready flag
        }
        break;
    default:
        break ;
}

return(data) ;
} /* End Read_ADC() */


void __attribute__((__interrupt__, no_auto_psv)) _ADCP0Interrupt (void)
{
    PORTEbits.RE5 = 1; //toggle this bit to confirm operation using scope
    if(VU_HCNDataFeed2BufTXEnable == 0)
    {
        ecan1msgBuf[VU_HCNDataFeed2Buf][3] = ADCBUF0;
        VU_HCNDataFeed2BufTXEnable = 1;
    }

    // this == 1 means ADC register ready to read
    // set back to 0 after reading
    IFS6bits.ADCP0IF = 0; // Clear ADC Pair 0 Interrupt Flag

    PORTEbits.RE5 = 0; //toggle this bit to confirm operation using scope
}

void __attribute__((__interrupt__, no_auto_psv)) _ADCP1Interrupt (void)
{
    PORTEbits.RE6 = 1; //toggle this bit to confirm operation using scope
    if(VU_HCNDataFeed3BufTXEnable == 0)
    {
        ecan1msgBuf[VU_HCNDataFeed3Buf][3] = ADCBUF2;
        VU_HCNDataFeed3BufTXEnable = 1;
    }
   IFS6bits.ADCP1IF = 0; // Clear ADC Pair 1 Interrupt Flag
   PORTEbits.RE6 = 0; //toggle this bit to confirm operation using scope
}

// ADC buf 0-4 wired to header