/*******************************************************
Name: Init.c
Date: 11/08/2017
Created By: Shane King
Comments: Initialization of dsPIC breakout board
Revision History:

*******************************************************/
#include "Defs.h"
#include "Externals.h"

/* These are preprocessor directives to initialize features in
 the dsPIC. These are words written to special registers in the
 flash memory that determine processor options (see section 24 of 
 the dsPIC datasheet DS70591E) */

// This is the start up osc.
_FOSCSEL(FNOSC_FRCPLL  & IESO_ON);
// Clock Switching is enabled and Fail Safe Clock Monitor is disabled
// OSC2 Pin Function: OSC2 is GPIO
// Primary Oscillator Mode: Internal RC Osc
_FOSC(FCKSM_CSECME & OSCIOFNC_OFF & POSCMD_NONE);
_FICD( ICS_PGD1 & JTAGEN_OFF);
_FWDT(FWDTEN_OFF);// Watchdog Timer disabled

void InitGlobs(void)
{
   // initialize program run time at power up to 0
    gRunTime = 0;

    // for loop to init timers
}

void InitOsc(void)
{
    // Configure Oscillator to operate the device at 40Mhz instruction cycle
    // See section 9 of dsPIC datasheet (DS70591E).
    // Fosc= Fin*M/(N1*N2),
    // Fosc= 7.37M*65/(2*3)=79.84MHz 80Mhz
    // Fcy=Fosc/2
    PLLFBD = 63; // M  = PLLDIV+2 = 50 9 bits
    CLKDIVbits.PLLPRE = 1; // N1 = PLLPRE + 2 = 3
    CLKDIVbits.PLLPOST = 0; // N2 = 2*(PLLPOST + 1) = 2

    // Disable Watch Dog Timer. See section 24 of datasheet (DS70591E).
    RCONbits.SWDTEN = 0;

    // Wait for PLL to stablize.
    // See section 9 of dsPIC datasheet (DS70591E).
    while (OSCCONbits.LOCK != 1)
    {
    };
}

/* Initialize general purpose I/O ports */
void InitGPIO(void)
{
    // Set the digital pins as input or output
    // 0 is output, 1 is input
    // PORT refers to the I/O pins, TRIS refers to the tri-state
    // register, which sets the corresponding pins to input or output.
    // See section 11 of the datasheet (DS70591E) for description
    // of initialization, and see page 10 of datasheet for correspondence
    // between ports and pins.
    TRISA = 0x0000;
    TRISB = 0x0005; //RB0 and RB2 are analog inputs
    PORTC = 0x0000;
    TRISC = 0x0000;
    PORTD = 0x0000;
    TRISD = 0x0000;
    PORTE = 0x0000;
    TRISE = 0x0000;
    PORTF = 0x0000;
    TRISF = 0x0001;
    PORTG = 0x0000;
    TRISG = 0x0000;
    
    //NEW
    
    TRISDbits.TRISD0=1; // Defines QEB1 as an input
    TRISDbits.TRISD11=1; // Defines QEA1 as an input
    TRISAbits.TRISA14=0; // pin 52 output
    // END NEW
    
    // Accelerometer
    TRISBbits.TRISB14 = 0; // Pin 35 is output for powering Accelerometer
    TRISBbits.TRISB8 = 1; // Pin 29 is input for Z data
    TRISBbits.TRISB9 = 1; // Pin 28 is input for X data
    TRISBbits.TRISB10 = 1; // Pin 27 is input for Y data
    
    // Output signals for controlling H bridge
    TRISDbits.TRISD2 = 0;  //Pin 62
    TRISDbits.TRISD12 = 0; //Pin 64
    TRISDbits.TRISD4 = 0;  //Pin 66
    TRISDbits.TRISD6 = 0; //Pin 68
    
}
