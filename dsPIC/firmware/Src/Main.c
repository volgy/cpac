/*******************************************************
Name: Main.c
Date: 11/06/2017
Created By: Shane King
Description: Main source code file for dsPIC breakout board test.
Test code runs infinite loop that sequentially lights LEDs
and sends data over the CAN bus
Revision History:
rev number: 0  date: 9/1/2013
 *****************************************************/

#include "Defs.h"
#include "Globals.h"
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "I2C.h"
#include "MPU6050.h"

/* These are function prototypes, which define the proper usage
 of functions located in other files */
extern void Ecan1Init(void); // Located in ECAN1Config.c
extern void CanSend(UINT16 ID); // Located in CanSend.c
extern void CanSendAll(void); // Located in CanSend.c
extern void CanSend0x20(void) ;
extern void CanSend0x21(void) ;

extern int CanParse(void);
extern void InitOsc( void); // Located in Init.c
extern void InitGPIO(void); // Located in Init.c
extern void InitGlobs(void); // Located in Init.c
extern void InitEcan1(void); // Located in Init.c
extern void TimersInit(void); // Located in Timers.c
extern void ADCInit(void);
extern void PWMInit(void);

// NEW

//Initializes the QEI Module
void InitQEI(void)
{
QEI1CONbits.QEIM = 0; // Disable QEI Module
QEI1CONbits.CNTERR = 0; // Clear any count errors
QEI1CONbits.QEISIDL = 0; // Continue operation during sleep
QEI1CONbits.SWPAB = 0; // QEA and QEB not swapped
QEI1CONbits.PCDOUT = 0; // Normal I/O pin operation
DFLT1CONbits.CEID = 1; // Count error interrupts disabled
DFLT1CONbits.QEOUT = 1; // Digital filters output enabled for QEn pins
DFLT1CONbits.QECK = 3; // 1:16 clock divide for digital filter for QEn
POS1CNT = 0; // Reset position counter
QEI1CONbits.QEIM = 7; /* X4 mode with the position counter reset by the match 
                       * (MAX1CNT, unused) */
return;
}

int dtemp; // Initializes the temporary duty cycle variable as a global integer

// END NEW

int main(void)
{

    /* Call routines to initialize various functions. */
    InitOsc(); // Located in Init.c
    InitGPIO(); // Located in Init.c
    InitGlobs(); // Located in Init.c
    TimersInit(); // Located in Timers.c
    Ecan1Init(); // Located in ECAN1Config.c
    ADCInit(); // Initialize ADC
    PWMInit(); // Initialize PWM
    I2C_Init();
    
    // NEW
    
    InitQEI(); // Initializes QEI
    
    
    float maxencode=.96; /* Initializes and declares the encoders max pulse per
                          * revolution/100 as a float to be used as the
                          * conversion factor for the encoder position counter*/
    float angpos; // Initializes angular position variable as a float
    dtemp = 0; // Declares temporary duty cycle
    UINT32 PDC7_temp; // Initializes temporary PWM duty cycle
    float d=0; /* Initializes and declares percent duty cycle variable as float 
                * 0=<d=<100 */
    
    // END NEW

    /* Set register bits that control global interrupts.
     * See section 7 of dsPIC datasheet */
    INTCON1bits.NSTDIS = 0; // Disable nested interrupts 
    SRbits.IPL = 0; // Set interrupt priority, see section 7.3

    /* Main infinite loop repeatedly calls three functions, one to
     * send data through the CAN bus (to Simulink), one to receive
     * data through the CAN bus (from Simulink), and one to
     * sequentially light the 5 LEDs. */

    // Initialize data array to send to Simulink
    gData[0]=0;
    gData[1]=0;

    
    // NEW
    
    /* Turns on the green LED and turns off all of the RED LEDs to be used as a
     * check for if the program is running on the board */
    LED1=LEDON;
    LED2=LEDOFF;
    LED3=LEDOFF;
    LED4=LEDOFF;
    LED5=LEDOFF;
    
    if (MPU6050_Init() == MPU6050_OK) {
        LED2 = LEDOFF;
    }
    else {
        LED2 = LEDON;
    }
    
    
    Pin35 = 1;
    i = 0;
    ii = 0;
    solenoid_type = 0;
    prev = 1;
    
    while (1)
    {
        dtemp=(int)POS1CNT; /* Temp duty cycle takes integer value of position 
                             * counter */
        quad_read = dtemp;
        /* Sets upper and lower limits for the temp duty cycle (max at one full 
         * revolution) */
        if (dtemp<0)
        {
            dtemp=0;
        }
        if (dtemp>96)
        {
            dtemp=96;
        }
        
        d=dtemp/maxencode; // Calculates percent duty cycle
        
        // Defines the actual PWM duty cycle based off the percent duty cycle
        //PDC7_temp = ((UINT32)(PTPER)*(INT32)d)/100 ;
        //if (PDC7_temp < 0) PDC7_temp = 0; // Duty cycle lower limit
        //if (PDC7_temp > PTPER) PDC7_temp = PTPER; // Duty cycle upper limit
        //PDC7 (UINT16)PDC7_temp ; // load duty cycle into register

        
        AN_pin27 = ADCRead(8);
        AN_pin28 = ADCRead(9);
        AN_pin29 = ADCRead(10);
        
        angpos=(dtemp/maxencode)*3.60; /* Calculates angular position in degrees
                                        * from encoder position */
        
        if (IFS0bits.T1IF == 1) // checks if timer clock has ticked
        { 
        IFS0bits.T1IF = 0; // reset timer
        
        // Write data to send to Simulink via the CAN bus
        //gData[0] = AN_pin28;
        //gData[1] = AN_pin27;
        //gData[2] = AN_pin29;
        
        MPU6050_Update();
        gData[0] = accelX;
        gData[1] = accelY;
        gData[2] = accelZ;
        
        
        gData[3] = quad_read;


      
        CanSend0x20(); // Outputs data via can, located in CanSend.c
        CanParse();
        
        
        if (solenoid_type == 1){
            if (solenoid_input*prev == 3){
                H1 = 0;
                H2 = 0;
                H3 = 0;
                H4 = 0;
                i = 5;}

            if (i==5){
               ii = ii+1;
                H1 = 1;
                H2 = 0;
                H3 = 0;
                H4 = 1;
                if (ii>20000){
                    i=0;
                    ii = 0;
                    H1 = 0;
                    H2 = 0;
                    H3 = 0;
                    H4 = 0;
                    prev = 0;
                }        
            }

            if (solenoid_input+prev == 2){ //extends, suit disengages
                H1 = 0;
                H2 = 0;
                H3 = 0;
                H4 = 0;
                i = 10;}

            if (i==10){
               ii = ii+1;
                H1 = 0;
                H2 = 1;
                H3 = 1;
                H4 = 0;
                if (ii>5000){
                    i=0;
                    ii = 0;
                    H1 = 0;
                    H2 = 0;
                    H3 = 0;
                    H4 = 0;
                    prev = 1;
                }        
            }
        }
        if (solenoid_type == 0){
            H1 = 0;
            H4 = 0;
            if (solenoid_input == 2){
                PDC7 = 0;
                PDC6 = 0;
                prev = 1;
                ii = 0;
            }
            if (solenoid_input*prev == 3){
                PDC7 = 7992;
                PDC6 = 7992;
                ii = ii+1;
                if (ii>15000){
                    PDC7 = 1000;
                    PDC6 = 7992;
                    ii = 0;
                    prev = 0;
                }
            }    
        }

        }        
        // END NEW
    }
}

