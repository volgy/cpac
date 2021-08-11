/*******************************************************
Name: Defs.h
Date: 9/27/2013
Created By: Don Truex
Comments: Constant definitions for dsPIC breakout board
 * and definition of timer array for gTimer
Revision History:
MG 9/29/2013: Changed definition of LEDs to match order on board
*******************************************************/

/* Note that #ifndef avoids compiler error in case header file
 is called recursively */
#ifndef __DEFS_H__
#define __DEFS_H__
						
#include "p33FJ64GS608.h"
#include "DSPicTypes.h"
#include "Structs.h"

// Conversion constant for A/D converter
// 10 bits equals 3.3 volts: (3.3/2^10)[volts/bit]
#define VOLTSPERBIT 0.00322265625 //3.3/2^10

/* Define indexes in timer array. Each index corresponds
 * to a different timer. This definition defines readable
 * names corresponding to each. Instead of refering to
 * "gTimers[1]" you will refer to "gTimers[TIMER2]", which
 * is the second timer.   */
/* Enumeration of array elements for gTimers array */
enum {TIMER1,TIMER2,CANSEND,MAXTIMERS};

/* Define output port locations of each LED (MG 9/29/2013) */
#define LED1 LATFbits.LATF4  // LED 1 is the green LED
#define LED2 LATFbits.LATF5  // All other LEDs are red
#define LED3 LATFbits.LATF8
#define LED4 LATFbits.LATF7
#define LED5 LATFbits.LATF6
#define Pin52 LATAbits.LATA14
/* Define shorthand terms to turn on/off LEDs.
 * Example usage: LED3=LEDON; // turns on LED 3 */
#define LEDON 0
#define LEDOFF 1
#define Pin35 LATBbits.LATB14  //Pin 35 will be set to output 3v 

#define Pin27 LATBbits.LATB8 
#define Pin29 LATBbits.LATB10 


// H-bridge Outputs
#define H1 LATDbits.LATD2   //Pin 62
#define H2 LATDbits.LATD12  //Pin 64
#define H3 LATDbits.LATD4   //Pin 66 
#define H4 LATDbits.LATD6   //Pin 68


#endif

