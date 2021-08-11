/*******************************************************
Name: PWM.c
Date: 9/27/2013
Created By: Don Truex
Revision History:
MG 10/10/2013 Modified and commented
MG 10/30/2013 Corrected error in PTPER formula
 
Description:
This file initializes the PWM 7H on the dsPIC,
sets the PWM frequency, and sets the initial output to 0.
In order to set the PWM frequency, define PTPER = 128e6*8/(f*p)
where f is freq in Hz and p is a prescalar (see below), where
p is selected such that PTPER is a large 16-bit word
(e.g., between 10k and 20k). Strictly speaking PTPER must be between
0x0010 and 0xFFF8. In order to output a PWM waveform with duty cycle d,
you can calculate the output using:

PDC7 = (PTPER*d)/100;

where PDC7 is a 16-bit pulse width register (defined by the compiler)
and the duty cycle d should be defined as a 16-bit unsigned integer:
UINT16 d ; // 0>=d>=100
 
In order to calculate the number to load into the pulse width
register, you need to type cast into a temporary 32-bit unsigned
integer (so the product can exceed 64k), then saturate between
0 and the PTPER (0 and 100% DC), then load into the PDC7 register.
Example code:
 
      PDC7_temp = ((UINT32)(PTPER)*(INT32)d)/100 ;
      if (PDC7_temp < 0) PDC7_temp = 0;
      if (PDC7_temp > PTPER) PDC7_temp = PTPER;
      PDC7 = (UINT16)PDC7_temp ; // load DC into register

References:
See section 43 High-speed PWM datasheet (document no. 70323E-1)

*********************************************************/
#include "p33fxxxx.h"
#include "Externals.h"

void PWMInit(void);

void PWMInit(void)
{
	// Setup for the PWM clock to use the Primary Oscillator as the REFCLK */
	//((OSC * 16) / APSTSCLR) = (7.37 * 16) / 1 = 117.92 MHz */
//	ACLKCONbits.ASRCSEL = 1; // Primary Oscillator is the Clock Source */
//	ACLKCONbits.FRCSEL = 0; // Input clock source is determined by ASRCSEL bit setting */
//	ACLKCONbits.SELACLK = 1; // Auxiliary Oscillator provides the clock source */
//	ACLKCONbits.APSTSCLR = 7; // Divide Auxiliary clock by 1 */
//	ACLKCONbits.ENAPLL = 1; // Enable Auxiliary PLL */
	ACLKCON = 0xe780;
	while(ACLKCONbits.APLLCK != 1); // Wait for Auxiliary PLL to Lock */
        /**************************/
      	/* Set PWM frequency here */
        /**************************/
        // Equation 43.4 in DS70323E where aux clock is 128e6 Hz
        // PTPER = 128e6*8/(f*p) - 8 ;
        // where p is prescalar and f is PWM freq in Hz
        // Note that PTPER is a 16-bit word (0x0010<=PTPER<=0xFFF8)
        // which is 16<=PTPER<=65528
        // Note: Lowest PWM frequency at PTPER = 0xFFF8 (65528)
        // f(lowest) ~= 128e6*8/(65528*64) = 244 Hz

        // Use f=1kHz for the LED brightness
        // For f=1kHz and p=64, PTPER=(128e6)(8)/((1000)(64))-8 = 15992
        
        /* Select PWM prescaler (see page 235 of datasheet) */
        // Prescalar p=1
	// PTCON2bits.PCLKDIV = 0;
        // Prescalar p=2
	// PTCON2bits.PCLKDIV = 1;
	// Prescalar p=4 
	// PTCON2bits.PCLKDIV = 2;
	// Prescalar p=8 
	// PTCON2bits.PCLKDIV = 3;
        // Prescalar p=16 
	// PTCON2bits.PCLKDIV = 4;
        // Prescalar p=32 
	// PTCON2bits.PCLKDIV = 5;
        // Prescalar p=64 
	PTCON2bits.PCLKDIV = 6;

        #define PWMPERIOD 7992//1kHz 

	PTPER = PWMPERIOD; //Primary period
	//setup PWM 7H 
	PWMCON7 = 0x0000;
	IOCON7bits.PENH = 1;  //enable PWM H output
	IOCON7bits.PENL = 0;  //disable PWM L output.
	IOCON7bits.PMOD = 0b11; //hi low are independent
	LEBDLY7bits.LEB = 100;
	DTR7 = 100; //
    
    // Setup PWM 6H
    PWMCON6 = 0x0000;
    IOCON6bits.PENH = 1; //enable PWM H output
    IOCON6bits.PENL = 0; //disable PWM L output
    IOCON6bits.PMOD = 0b11; //hi low are independent
    LEBDLY6bits.LEB = 100;
    DTR6 = 100;
    
        /* Set the duty cycle of the PWM output here */
	PDC7 = PTPER*0; // Initialize to 0% duty cycle
    PDC6 = PTPER*0; // Initialize to 0% duty cycle
    
	PTCONbits.PTEN = 1; // Enable output
}
