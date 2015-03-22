/* ---------------------------------------------------------------------------
 ** This software is in the public domain, furnished "as is", without technical
 ** support, and with no warranty, express or implied, as to its usefulness for
 ** any purpose.
 **
 ** Author: Matthieu Holtz
 ** Year: 2015
 ** -------------------------------------------------------------------------*/

#pragma once

#ifndef TLC59116_H
#define TLC59116_H

#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "I2C.h"

#define		TLC59116_MAX_LED	16		// Amount of LED outputs

#define		ADR_MODE_1			0x00
#define		ADR_MODE_2			0x01
#define		ADR_PWM_0			0x02
#define		ADR_PWM_1			0x03
#define		ADR_PWM_2			0x04
#define		ADR_PWM_3			0x05
#define		ADR_PWM_4			0x06
#define		ADR_PWM_5			0x07
#define		ADR_PWM_6			0x08
#define		ADR_PWM_7			0x09
#define		ADR_PWM_8			0x0A
#define		ADR_PWM_9			0x0B
#define		ADR_PWM_10			0x0C
#define		ADR_PWM_11			0x0D
#define		ADR_PWM_12			0x0E
#define		ADR_PWM_13			0x0F
#define		ADR_PWM_14			0x10
#define		ADR_PWM_15			0x11
#define		ADR_GRPPWM			0x12
#define		ADR_GRPFREQ			0x13
#define		ADR_LEDOUT_0		0x14
#define		ADR_LEDOUT_1		0x15
#define		ADR_LEDOUT_2		0x16
#define		ADR_LEDOUT_3		0x17
#define		ADR_ERRFLAG_1		0x1D
#define		ADR_ERRFLAG_2		0x1E

#define		MASK_NO_AUTO_INCR	0x00	// Bit mask for no auto increment (register address)
#define		MASK_AUTO_INCR		0x80	// Bit mask for using auto increment (register address)

#define		LEDOUT_POS_0		0		// position of 1st bit
#define		LEDOUT_POS_1		2		// position of 1st bit
#define		LEDOUT_POS_2		4		// position of 1st bit
#define		LEDOUT_POS_3		6		// position of 1st bit


class TLC59116 {
	
private:
	// This is an LED output abstraction
	struct LED
	{
		const u_int16_t		adr_ledout;
		const u_int16_t		adr_pwm;
		const u_int8_t		adr_ledout_pos;
		unsigned char *		ledout_Register;
	};
	
private:
	I2C * device;
	unsigned char *				ledout_0Register;
	unsigned char *				ledout_1Register;
	unsigned char *				ledout_2Register;
	unsigned char *				ledout_3Register;
	const struct TLC59116::LED	ledLUT[TLC59116_MAX_LED];
	bool						useGroupDimming;
	
public:
							TLC59116				(std::string _i2cFileName, unsigned char _deviceAddress, bool _useGroupDimming=false);
	virtual					~TLC59116				();
	
	virtual I2C *			getI2CDevice			();
	virtual void			allOn					();
	virtual void			allOff					();
	virtual void			setOn					(const u_int8_t _led);
	virtual void			setOff					(const u_int8_t _led);
	virtual void			setPWMDimming			(const u_int8_t _led, u_int16_t _pwm);
	virtual bool			isUseGroupDimming		() const;
	virtual void			setUseGroupDimming		(const bool _useGroupDimming);
	virtual void			setGroupPWMDimming		(u_int16_t _pwm);
	virtual void			setAllPWMDimming		(u_int16_t _pwm);
	virtual unsigned char	getPWMDimming			(const u_int8_t _led) const;
	virtual unsigned char	getGroupPWMDimming		() const;
	virtual bool			isLEDOn					(const u_int8_t _led) const;
	virtual unsigned char	getErrorFLag1			() const;
	virtual unsigned char	getErrorFLag2			() const;
	
private:
			void			init					();
			void			updateLedOutRegister	(const unsigned char _new_LedOut_Register_Value,
														unsigned char * _LedOut_Register,
														const u_int16_t _LedOut_Register_ADR);
			void			handleSetUseGroupDimming (unsigned char * _LedOut_Register,
														const u_int16_t _LedOut_Register_ADR);
	unsigned char			getMaskOn				(const u_int8_t pos) const;
	unsigned char			getMaskOff				(const u_int8_t pos) const;
};


#endif