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

#include <stddef.h>
#include "xc.h"
#include "I2C.h"
#include "MPU6050.h"


INT16 accelX, accelY, accelZ;
INT16 gyroX, gyroY, gyroZ;



static void _mpu_write_reg(UINT8 addr, UINT8 data) {
    I2C_Start();
    I2C_TxByte(MPU6050_ADDR << 1 | I2C_WRITE);
    I2C_TxByte(addr);
    I2C_TxByte(data);
    I2C_Stop();
}

static UINT8 _mpu_read_reg(UINT8 addr) {
    UINT8 data;
    
    I2C_Start();
    I2C_TxByte(MPU6050_ADDR << 1 | I2C_WRITE);
    I2C_TxByte(addr);
    I2C_ReStart();
    I2C_TxByte(MPU6050_ADDR << 1 | I2C_READ);
    data = I2C_RxByte();
    I2C_Stop();
    return data;
}

static void _mpu_read_file(UINT8 start_addr, size_t length, UINT8 *data) {
    
    if (length <= 0) {
        return;
    }
    
    I2C_Start();
    I2C_TxByte(MPU6050_ADDR << 1 | I2C_WRITE);
    I2C_TxByte(start_addr);
    I2C_ReStart();
    I2C_TxByte(MPU6050_ADDR << 1 | I2C_READ);
    I2C_RxBytes(length, data);
    I2C_Stop();
}


int MPU6050_Init()
{       
    if (_mpu_read_reg(MPU6050_REG_WHO_AM_I) !=  (MPU6050_ADDR & 0xFE)) {
        return MPU6050_ERR;
    }
    
    _mpu_write_reg(MPU6050_REG_PWR_MGMT_1, 0); // wake-up, using internal osc
    
    return MPU6050_OK;
}


void MPU6050_Update()
{
    UINT8   buffer[14];
    
    _mpu_read_file(MPU6050_REG_ACCEL_XOUT_H, sizeof(buffer), buffer);
    accelX = (buffer[0] << 8) | buffer[1];
    accelY = (buffer[2] << 8) | buffer[3];
    accelZ = (buffer[4] << 8) | buffer[5];
    
    gyroX = (buffer[8] << 8) | buffer[9];
    gyroY = (buffer[10] << 8) | buffer[11];
    gyroZ = (buffer[12] << 8) | buffer[13];
}

