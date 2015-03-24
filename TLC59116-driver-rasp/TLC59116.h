/// -------------------------------------------------------------------------
/// @file TLC59116.h
/// @brief header file for the TLC59116 class.
/// @author Matthieu Holtz
/// @version 0
/// @date 2015
///
/// This software is in the public domain, furnished "as is", without technical
/// support, and with no warranty, express or implied, as to its usefulness for
/// any purpose.
/// -------------------------------------------------------------------------

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

#define		MASK_NO_AUTO_INCR	0x00	///< Bit mask for no auto increment (register address)
#define		MASK_AUTO_INCR		0x80	///< Bit mask for using auto increment (register address)

#define		LEDOUT_POS_0		0		///< position of 1st bit
#define		LEDOUT_POS_1		2		///< position of 1st bit
#define		LEDOUT_POS_2		4		///< position of 1st bit
#define		LEDOUT_POS_3		6		///< position of 1st bit

/// @class TLC59116
/// @brief this is the abstraction for the Texas Instrument TLC59116 I2C LED controller chip.
/// It indends to work on a Linux plateform with I2C support and offers the basics functions of the TLC59116.
///
class TLC59116 {
	
private:
	/// @struct LED
	/// @brief this structure is an abstraction for an LED controlled with the TLC59116
	struct LED
	{
		const u_int16_t		adr_ledout;			///< hex address of the LEDOUT register where the LED output is controlled
		const u_int16_t		adr_pwm;			///< hex address of the PWM register wich holds the PWM dimming of this output
		const u_int8_t		adr_ledout_pos;		///< position (0, 2, 4, 6) in the register of the 1st bit controlling the output state in LEDOUT register. (The LED is controlled with 2 bit of LEDOUT register)
		unsigned char *		ledout_Register;	///< LEDOUT register (this is a pointer to an image of the real register)
	};
	
private:
	I2C * device;									///< this is the TLC I2C device
	unsigned char *				ledout_0Register;	///< image of the LEDOUT0 register
	unsigned char *				ledout_1Register;	///< image of the LEDOUT1 register
	unsigned char *				ledout_2Register;	///< image of the LEDOUT2 register
	unsigned char *				ledout_3Register;	///< image of the LEDOUT3 register
	const struct TLC59116::LED	ledLUT[TLC59116_MAX_LED]; ///< Look up table with all LED
	bool						useGroupDimming;	//< define wether or not we use the group dimming function
	
public:
	/// @brief Constructor with I2C controller filename and TLC address on the bus. In addition you can enable the group dimming
	///
	/// @param _i2cFileName		: the linux I2C device filename
	/// @param _deviceAddress	: the TLC address on the I2C bus
	/// @param _useGroupDimming : to enable or not the group dimming function (default set to false)
							TLC59116				(std::string _i2cFileName, unsigned char _deviceAddress, bool _useGroupDimming=false);
	
	/// @brief Destructor
	virtual					~TLC59116				();
	
	/// @brief Getter for the I2C device, if you need to directly access the TLC through the I2C bus abstraction
	///
	/// @return the I2C abstraction
	virtual I2C *			getI2CDevice			();
	
	/// @brief Turns all LED output ON
	virtual void			allOn					();
	
	/// @brief Turns all LED output OFF
	virtual void			allOff					();
	
	/// @brief Set the specified LED ON
	///
	/// @param _led		: the LED index (from 0 to 15) to switch ON
	virtual void			setOn					(const u_int8_t _led);
	
	/// @brief Set the specified LED OFF
	///
	/// @param _led		: the LED index (from 0 to 15) to switch OFF
	virtual void			setOff					(const u_int8_t _led);
	
	/// @brief Set the specified LED dimming value with the PWM value
	///
	/// @param _led		: the LED index (from 0 to 15) to set the PWM
	/// @param _pwm		: the PWM value to set (0 to 255)
	virtual void			setPWMDimming			(const u_int8_t _led, u_int16_t _pwm);
	
	/// @brief Returns true of the group dimming function is now activated
	///
	/// @return true if dimming function is ON, false otherwise
	virtual bool			isUseGroupDimming		() const;
	
	/// @brief Set the group dimming function to ON or OFF depending on the _useGroupDimming parameter and imediately update all outputs
	///
	/// @param _useGroupDimming		: true to activate the dimming function, false to deactivate
	virtual void			setUseGroupDimming		(const bool _useGroupDimming);
	
	/// @brief Set the group dimming value with the PWM value. You need first to turn on the group dimming function
	///
	/// @param _pwm		: the group PWM value to set (0 to 255)
	virtual void			setGroupPWMDimming		(u_int16_t _pwm);
	
	/// @brief Set the dimming value with the PWM value for all 16 LED output
	///
	/// @param _pwm		: the PWM value to set (0 to 255)
	virtual void			setAllPWMDimming		(u_int16_t _pwm);
	
	/// @brief Returns current value of the PWM for the specified LED output
	///
	/// @param _led		: led index (from 0 to 15) from which we want the current PWM dimming value
	/// @return the current PWM dimming value (0 to 255)
	virtual unsigned char	getPWMDimming			(const u_int8_t _led) const;
	
	/// @brief Returns current value of the group PWM
	///
	/// @return the current group PWM dimming value (0 to 255)
	virtual unsigned char	getGroupPWMDimming		() const;
	
	/// @brief Returns wether or not the specified LED is ON or OFF
	///
	/// @param _led		: led index (from 0 to 15) from which we want know the current state ON/OFF
	/// @return true if LED is ON, false is OFF
	virtual bool			isLEDOn					(const u_int8_t _led) const;
	
	/// @brief Returns Error flag 1 register
	///
	/// @return raw value of the register
	virtual unsigned char	getErrorFLag1			() const;
	
	/// @brief Returns Error flag 2 register
	///
	/// @return raw value of the register
	virtual unsigned char	getErrorFLag2			() const;
	
private:
	
	/// @brief Internal initialization of the registers. This method is called from the constructor
			void			init					();
	
	/// @brief Internal handle of the LEDOUT_x registers when turning ON or OFF a LED output
			void			updateLedOutRegister	(const unsigned char _new_LedOut_Register_Value,
														unsigned char * _LedOut_Register,
														const u_int16_t _LedOut_Register_ADR);
	
	/// @brief Internal handle of the group dimming switch ON or OFF in order to update the LEDOUT_x registers
			void			handleSetUseGroupDimming (unsigned char * _LedOut_Register,
	
													  const u_int16_t _LedOut_Register_ADR);
	
	/// @brief Returns the current mask to use in LEDOUT register to turn ON the LED at the specified position. The mask vary is the group pwm dimming function is ON or OFF
	///
	/// @param pos		: the position of the 1st bit of the LED in LEDOUT
	/// @return the mask to apply on LEDOUT_x register to switch ON the LED
	unsigned char			getMaskOn				(const u_int8_t pos) const;
	
	/// @brief Returns the current mask to use in LEDOUT register to turn OFF the LED at the specified position. The mask vary is the group pwm dimming function is ON or OFF
	///
	/// @param pos		: the position of the 1st bit of the LED in LEDOUT
	/// @return the mask to apply on LEDOUT_x register to switch OFF the LED
	unsigned char			getMaskOff				(const u_int8_t pos) const;
};


#endif