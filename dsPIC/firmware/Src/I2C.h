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

#ifndef I2C_H
#define	I2C_H

#include <stddef.h>
#include <xc.h> 
#include "DSPicTypes.h"

#define I2C_OK      (0)
#define I2C_ERR     (1)

#define I2C_WRITE   (0x00)
#define I2C_READ   (0x01)

void I2C_Init();
void I2C_Idle();
void I2C_Start();
void I2C_ReStart();
void I2C_Stop();
int I2C_TxByte(UINT8 data);
UINT8 I2C_RxByte();
void I2C_RxBytes(size_t length, UINT8* data);

#endif	/* I2C_H */

