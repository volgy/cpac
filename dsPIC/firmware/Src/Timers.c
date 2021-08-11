/*******************************************************
Name: Timers.c
Date: 9/27/2013
Created By: Don Truex
Revision History:
rev number  date 		by
1           9/29/2013           MG

Description:
Initialize and start timers. Timers 1 through 3 can be set by the
user for user-defined purposes. Timer 4 is set at 1 kHz,
and is used in an interrupt service routine below to
increment the run-time counter every ms, and to decrement the
counter array in gTimer[] every 1 ms.

In order to set the "clock rate" of Timers 1 through 3, the user
must set the period and prescale for each timer, where the frequency
of each timer is given by:

    freq = clk/(period*prescale)

where the prescale is set to 1, 8, 64, or 256, and is essentially
a range selector, and the period is selected as:

    period = clk/(freq*prescale)

where clk is 40 MHz.

Example usage: Set Timer 1 to "tick" at 5 kHz (i.e., every 0.2 ms)
    // option 1: period = 40e6/(5000*64) = 125
    // option 2: period = 40e6/(5000*8) = 1000
    // option 3: period = 40e6/(5000*1) = 8000
    // period is UINT16, so must be between 0 and 64k. Somewhere is the
    // middle is a good idea.
    // select option 3:
    T1CONbits.TCKPS = 0b00; // Select prescaler=1 (see page 220)
    PR1 = 8000; // Load the period value for 5000 Hz corresponding to
                // prescaler = 1

In order to check when timer has "ticked", you need to poll the
corresponding timer interrupt flag, which will become 1 when the timer
has completed the period count. For Timer 1:

	if (IFS0bits.T1IF == 1) { // timer clocked has ticked
            IFS0bits.T1IF = 0; // reset timer
            // Perform whatever function is to be performed here
        }

TMR4 triggers the interrupt service routines (below) that increments
the runtime counter and decrements the timer array gTimer[]
defined in Globals.h, Externals.h, and Defs.h.

Function list:
TimersInit()    Initialize timers 1-4
_T4Interrupt()  ISR that decrements gTimers on TMR3

References:
Section 13 of the dsPIC datasheet DS70591E
 *****************************************************/

#include "Defs.h"
#include "Externals.h"

/* Function prototype */
void TimersInit(void);

void TimersInit(void)
{
        /******************************
            Set up timer 1
        ******************************/
        //40 Meg Clock
        //use timer 1 to trigger AN0 conversion at 10 kHz.
	T1CONbits.TON = 0; // Disable Timer
	T1CONbits.TCS = 0; // Select internal instruction cycle clock
	T1CONbits.TGATE = 0; // disable Gated Timer mode

        // Timer count rate is set by the combination of the prescalar
        // (T1CONbits.TCKPS) and the period (PR1) as follows:
        // Timer rate: freq = clk/(period*prescale) where clk=40MHz
        // freq = 40e6/500/8 = 10 kHz (i.e., set prescaler=8 and period=5000)
	//T1CONbits.TCKPS = 0b00; // Select prescaler=1 (see page 220)
	T1CONbits.TCKPS = 0b01; // Select prescaler=8 (see page 220)
	//T1CONbits.TCKPS = 0b10; // Select prescaler=64 (see page 220)
	//T1CONbits.TCKPS = 0b11; // Select prescaler=256 (see page 220)
	TMR1 = 0x00; // Clear timer register
	PR1 = 500; // Load the period value for 10 kHz
        /* Poll this bit (T1 interrupt flag) for sampling clock tick. Flag
           is set to 1 for every clock tick. User should reset to 0 after
           each clock tick. */
        IFS0bits.T1IF = 0; // Clear Interrupt Flag
	IEC0bits.T1IE = 0; // Disable interrupt (no ISR)
	T1CONbits.TON = 1; // Start Timer

        /******************************
            Set up timer 2
        ******************************/
	T2CONbits.TON = 0; // Disable Timer
	T2CONbits.TCS = 0; // Select internal instruction cycle clock
	T2CONbits.TGATE = 0; // disable Gated Timer mode

        // Timer count rate is set by the combination of the prescalar
        // (T2CONbits.TCKPS) and the period (PR2) as follows:
        // Timer rate: freq = clk/(period*prescale) where clk=40MHz
        // freq = 40e6/12500/64 = 50 Hz (i.e., set prescaler=8 and period=12500)
	//T2CONbits.TCKPS = 0b00; // Select prescaler=1 (see page 220)
	//T2CONbits.TCKPS = 0b01; // Select prescaler=8 (see page 220)
	T2CONbits.TCKPS = 0b10; // Select prescaler=64 (see page 220)
	//T2CONbits.TCKPS = 0b11; // Select prescaler=256 (see page 220)

        TMR2 = 0x00; // Clear timer register
	PR2 = 12500; // Load the period value for 50 Hz
        /* Poll this bit (T2 interrupt flag) for sampling clock tick. Flag
           is set to 1 for every clock tick. User should reset to 0 after
           each clock tick. */
        IFS0bits.T2IF = 0; // Clear Interrupt Flag
	IEC0bits.T2IE = 0; // Disable interrupt (no ISR)
	T2CONbits.TON = 1; // Start Timer

        /******************************
            Set up timer 3
        ******************************/

        T3CONbits.TON = 0; // Disable Timer
	T3CONbits.TCS = 0; // Select internal instruction cycle clock
	T3CONbits.TGATE = 0; // disable Gated Timer mode

        // Timer count rate is set by the combination of the prescalar
        // (T3CONbits.TCKPS) and the period (PR4) as follows:
        // Timer rate: freq = clk/(period*prescale) where clk=40MHz
        // freq = 40e6/5000/8 = 1 kHz (i.e., set prescaler=8 and period=5000)
	//T3CONbits.TCKPS = 0b00; // Select prescaler=1 (see page 220)
	T3CONbits.TCKPS = 0b01; // Select prescaler=8 (see page 220)
	//T3CONbits.TCKPS = 0b10; // Select prescaler=64 (see page 220)
	//T3CONbits.TCKPS = 0b11; // Select prescaler=256 (see page 220)

	TMR3 = 0x00; // Clear timer register
        // freq = 40e6/500/8 = 10 kHz
        PR3 = 500; // Load the period value for 10 kHz
	// IPC2bits.T3IP = 0x04; // Set Interrupt Priority Level
        /* Poll this bit (T3 interrupt flag) for sampling clock tick. Flag
           is set to 1 for every clock tick. User should reset to 0 after
           each clock tick. */
        IFS0bits.T3IF = 0; // Clear Interrupt Flag
	IEC0bits.T3IE = 0; // Disable interrupt (i.e., no ISR)
        // Timer 3 not currently started. Set bit to 1 to start.
	T3CONbits.TON = 0; // Set to 1 to start timer 3

        /******************************
            Set up timer 4
        ******************************/
	// use timer 4 and ISR below to increment runtime and
        // decrement timer array
	T4CONbits.TON = 0; // Disable Timer
	T4CONbits.TCS = 0; // Select internal instruction cycle clock
	T4CONbits.TGATE = 0; // disable Gated Timer mode
        // Timer count rate is set by the combination of the prescalar
        // (T4CONbits.TCKPS) and the period (PR4) as follows:
        // Timer rate: freq = clk/(period*prescale) where clk=40MHz
        // freq = 40e6/5000/8 = 1 kHz (i.e., set prescaler=8 and period=5000)
	//T4CONbits.TCKPS = 0b00; // Select prescaler=1 (see page 220)
	T4CONbits.TCKPS = 0b01; // Select prescaler=8 (see page 220)
	//T4CONbits.TCKPS = 0b10; // Select prescaler=64 (see page 220)
	//T4CONbits.TCKPS = 0b11; // Select prescaler=256 (see page 220)
	TMR4 = 0x00; // Clear timer register
        PR4 = 5000; // Load TMR4 period of 5000
     	IPC6bits.T4IP = 0x04; // Set Interrupt Priority Level (since TMR4
        // triggers an ISR
	IFS1bits.T4IF = 0; // Clear interrupt flag.
	IEC1bits.T4IE = 1; // Enable interrupt (TMR4 triggers ISR)
        T4CONbits.TON = 1; // Set to 1 to start Timer

}

/* Interrupt service routine to decrement msec timer array. Note that
 * the underscores denote an ISR, and name is assigned by Microchip.
 * The processor will call _T4Interrupt() at the rate of TMR4 as initialized
 * above (i.e., at 1 ms). When the ISR is called, it will increment the
 * run-time counter gRunTime, and decrement each element of the
 * gTimers[] array */

void __attribute__((__interrupt__, no_auto_psv)) _T4Interrupt(void)
{
    int i;

    // increment total run time counter. Total run time is
    // gRunTime/1000 (s). gRunTime wraps around at 2^32/1000
    // =~50 days.
    gRunTime++;

    /* This decrements all counters in the gTimers array, as defined
     in Defs.h, with the total number of timers defined by MAXTIMERS */
    for (i = 0; i < MAXTIMERS; i++)
    {
        if (gTimers[i] > 0)
            gTimers[i]--;
    }
    IFS1bits.T4IF = 0; // Clear Timer4 Interrupt Flag
}

