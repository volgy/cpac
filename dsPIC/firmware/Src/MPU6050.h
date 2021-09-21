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

#ifndef MPU6050_H
#define	MPU6050_H

#include <xc.h> 
#include "DSPicTypes.h"

#define MPU6050_OK      (0)
#define MPU6050_ERR     (1)

#define MPU6050_ADDR      (0x68)
// #define MPU6050_ADDR      (0x69)

#define MPU6050_REG_WHO_AM_I    (0x75)
#define MPU6050_REG_ACCEL_XOUT_H    (0x3B)
#define MPU6050_REG_PWR_MGMT_1      (0x6B)


extern INT16 accelX, accelY, accelZ;
extern INT16 gyroX, gyroY, gyroZ;


int MPU6050_Init();
void MPU6050_Update();

#endif	/* MPU6050_H */

