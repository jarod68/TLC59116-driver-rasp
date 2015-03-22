/* ---------------------------------------------------------------------------
 ** This software is in the public domain, furnished "as is", without technical
 ** support, and with no warranty, express or implied, as to its usefulness for
 ** any purpose.
 **
 ** Author: Matthieu Holtz
 ** Year: 2015
 ** -------------------------------------------------------------------------*/

#include "TLC59116.h"

TLC59116	::	TLC59116	(std::string _i2cFileName, unsigned char _deviceAddress, bool _useGroupDimming)
							:
							device				(new I2C(_i2cFileName, _deviceAddress)),
							ledout_0Register	(new unsigned char),
							ledout_1Register	(new unsigned char),
							ledout_2Register	(new unsigned char),
							ledout_3Register	(new unsigned char),
							ledLUT				(
									{
									{ADR_LEDOUT_0, ADR_PWM_0,	LEDOUT_POS_0,	ledout_0Register},
									{ADR_LEDOUT_0, ADR_PWM_1,	LEDOUT_POS_1,	ledout_0Register},
									{ADR_LEDOUT_0, ADR_PWM_2,	LEDOUT_POS_2,	ledout_0Register},
									{ADR_LEDOUT_0, ADR_PWM_3,	LEDOUT_POS_3,	ledout_0Register},
									{ADR_LEDOUT_1, ADR_PWM_4,	LEDOUT_POS_0,	ledout_1Register},
									{ADR_LEDOUT_1, ADR_PWM_5,	LEDOUT_POS_1,	ledout_1Register},
									{ADR_LEDOUT_1, ADR_PWM_6,	LEDOUT_POS_2,	ledout_1Register},
									{ADR_LEDOUT_1, ADR_PWM_7,	LEDOUT_POS_3,	ledout_1Register},
									{ADR_LEDOUT_2, ADR_PWM_8,	LEDOUT_POS_0,	ledout_2Register},
									{ADR_LEDOUT_2, ADR_PWM_9,	LEDOUT_POS_1,	ledout_2Register},
									{ADR_LEDOUT_2, ADR_PWM_10,	LEDOUT_POS_2,	ledout_2Register},
									{ADR_LEDOUT_2, ADR_PWM_11,	LEDOUT_POS_3,	ledout_2Register},
									{ADR_LEDOUT_3, ADR_PWM_12,	LEDOUT_POS_0,	ledout_3Register},
									{ADR_LEDOUT_3, ADR_PWM_13,	LEDOUT_POS_1,	ledout_3Register},
									{ADR_LEDOUT_3, ADR_PWM_14,	LEDOUT_POS_2,	ledout_3Register},
									{ADR_LEDOUT_3, ADR_PWM_15,	LEDOUT_POS_3,	ledout_3Register}
									}
												),
							useGroupDimming(_useGroupDimming)
{
	
	*ledout_0Register	=	0x00;
	*ledout_1Register	=	0x00;
	*ledout_2Register	=	0x00;
	*ledout_3Register	=	0x00;

	this->init();
}

TLC59116	::	~TLC59116	()
{
	delete device;
	delete ledout_0Register;
	delete ledout_1Register;
	delete ledout_2Register;
	delete ledout_3Register;
}

void TLC59116	::	init	()
{
	const u_int16_t dataSize = 24;
	unsigned char data[dataSize];

	data[0] = 0x00; // set MODE_1 : no auto increment, oscillator is ON,  no I2C sub-addresses
	data[1] = 0x00; // set MODE_2 : enable error flags, group control set to dimming
	
	for (int i=2; i< 18; i++) // set all PWM_i values to maximum
		data[i] = 0xFF;
	
	data[18] = 0xFF; // set GRPPWM with maximum
	data[19] = 0x00; // set GRPFREQ with 0, this is deactivated in MODE_2
	
	data[20] = *ledout_0Register;
	data[21] = *ledout_1Register;
	data[22] = *ledout_2Register;
	data[23] = *ledout_3Register;
	
	device->writeReg(MASK_AUTO_INCR | ADR_MODE_1, data, dataSize);
}

I2C * TLC59116	::	getI2CDevice	()
{
	return device;
}

void TLC59116	::	allOn			()
{
	const u_int16_t dataSize = 4;
	unsigned char data[dataSize];
	
	*ledout_0Register = 0xAA;
	*ledout_1Register = 0xAA;
	*ledout_2Register = 0xAA;
	*ledout_3Register = 0xAA;
	
	data[0] = *ledout_0Register;
	data[1] = *ledout_1Register;
	data[2] = *ledout_2Register;
	data[3] = *ledout_3Register;
	
	device->writeReg(MASK_AUTO_INCR | ADR_LEDOUT_0, data, dataSize);

}

void TLC59116	::	allOff			()
{
	
	const u_int16_t dataSize = 4;
	unsigned char data[dataSize];
	
	*ledout_0Register = 0x00;
	*ledout_1Register = 0x00;
	*ledout_2Register = 0x00;
	*ledout_3Register = 0x00;
	
	data[0] = *ledout_0Register;
	data[1] = *ledout_1Register;
	data[2] = *ledout_2Register;
	data[3] = *ledout_3Register;
	
	device->writeReg(MASK_AUTO_INCR | ADR_LEDOUT_0, data, dataSize);
	
}

void TLC59116	::	setOn			(const u_int8_t _led)
{

	
	TLC59116::LED led = ledLUT[_led];
	
	switch (led.adr_ledout_pos) {
			
		case 0:
			updateLedOutRegister((*led.ledout_Register) | getMaskOn(LEDOUT_POS_0), led.ledout_Register, led.adr_ledout);
			break;
			
		case 1:
			updateLedOutRegister((*led.ledout_Register) | getMaskOn(LEDOUT_POS_1), led.ledout_Register, led.adr_ledout);
			break;
			
		case 2:
			updateLedOutRegister((*led.ledout_Register) | getMaskOn(LEDOUT_POS_2), led.ledout_Register, led.adr_ledout);
			break;
			
		case 3:
			updateLedOutRegister((*led.ledout_Register) | getMaskOn(LEDOUT_POS_3), led.ledout_Register, led.adr_ledout);
			break;
			
		default:
			break;
	}
}

void TLC59116	::	setOff			(const u_int8_t _led)
{
	TLC59116::LED led = ledLUT[_led];
	
	switch (led.adr_ledout_pos) {
			
		case 0:
			updateLedOutRegister((*led.ledout_Register) & getMaskOff(LEDOUT_POS_0), led.ledout_Register, led.adr_ledout);
			break;
			
		case 1:
			updateLedOutRegister((*led.ledout_Register) & getMaskOff(LEDOUT_POS_1), led.ledout_Register, led.adr_ledout);
			break;
			
		case 2:
			updateLedOutRegister((*led.ledout_Register) & getMaskOff(LEDOUT_POS_2), led.ledout_Register, led.adr_ledout);
			break;
			
		case 3:
			updateLedOutRegister((*led.ledout_Register) & getMaskOff(LEDOUT_POS_3), led.ledout_Register, led.adr_ledout);
			break;
			
		default:
			break;
	}
}

void TLC59116	::	setPWMDimming	(const u_int8_t _led, u_int16_t _pwm)
{
	TLC59116::LED led = ledLUT[_led];
	device->writeReg(MASK_NO_AUTO_INCR | led.adr_pwm, _pwm);
}

bool TLC59116	::	isUseGroupDimming		() const
{
	return useGroupDimming;
}

void TLC59116	::	setUseGroupDimming	(const bool _useGroupDimming)
{
	if (this->isUseGroupDimming() == _useGroupDimming)
		return;
	
	useGroupDimming = _useGroupDimming;
	
	this->handleSetUseGroupDimming(ledout_0Register, ADR_LEDOUT_0);
	this->handleSetUseGroupDimming(ledout_1Register, ADR_LEDOUT_1);
	this->handleSetUseGroupDimming(ledout_2Register, ADR_LEDOUT_2);
	this->handleSetUseGroupDimming(ledout_3Register, ADR_LEDOUT_3);
	
}

void TLC59116	::	setGroupPWMDimming (u_int16_t _pwm)
{
	device->writeReg(MASK_NO_AUTO_INCR | ADR_GRPPWM, _pwm);
}

void TLC59116	::	setAllPWMDimming (u_int16_t _pwm)
{
	const u_int16_t dataSize = 18;
	unsigned char data[dataSize];
	
	
	for (int i=0; i< 18; i++) // set all PWM_i values to maximum
		data[i] = _pwm;
	
	device->writeReg(MASK_AUTO_INCR | ADR_PWM_0, data, dataSize);
}

unsigned char	TLC59116	::	getPWMDimming (const u_int8_t _led) const
{
	TLC59116::LED led = ledLUT[_led];
	unsigned char data = 0x00;
	
	device->readReg(led.adr_pwm, &data);
	
	return data;
}

unsigned char	TLC59116	::	getGroupPWMDimming () const
{
	unsigned char data = 0x00;
	
	device->readReg(ADR_GRPPWM, &data);
	
	return data;
}

bool			TLC59116	::	isLEDOn	(const u_int8_t _led) const
{
	return false; //TODO: need implementation
}

unsigned char	TLC59116	::	getErrorFLag1 () const
{
	unsigned char data = 0x00;
	
	device->readReg(ADR_ERRFLAG_1, &data);
	
	return data;
}

unsigned char	TLC59116	::	getErrorFLag2 () const
{
	unsigned char data = 0x00;
	
	device->readReg(ADR_ERRFLAG_2, &data);
	
	return data;
}

void TLC59116	::	handleSetUseGroupDimming (unsigned char * _LedOut_Register, const u_int16_t _LedOut_Register_ADR)
{
	unsigned char newLedOutRegister = *_LedOut_Register;
	
	unsigned char led_0 = newLedOutRegister & 0x03;
	unsigned char led_1 = newLedOutRegister & 0x0C;
	unsigned char led_2 = newLedOutRegister & 0x30;
	unsigned char led_3 = newLedOutRegister & 0xC0;
	
	if (led_0 != 0x00){
		newLedOutRegister |= getMaskOn(LEDOUT_POS_0);
	}
	
	if (led_1 != 0x00){
		newLedOutRegister |= getMaskOn(LEDOUT_POS_1);
	}
	
	if (led_2 != 0x00){
		newLedOutRegister |= getMaskOn(LEDOUT_POS_2);
	}
	
	if (led_3 != 0x00){
		newLedOutRegister |= getMaskOn(LEDOUT_POS_3);
	}
	
	if (newLedOutRegister != (*_LedOut_Register))
	{
		updateLedOutRegister (newLedOutRegister, _LedOut_Register, _LedOut_Register_ADR);
	}
	
}

unsigned char		TLC59116	::	getMaskOn				(const u_int8_t _pos) const
{
	if (_pos == LEDOUT_POS_0){
		
		if (useGroupDimming)
			return 0x03;
		
		return 0x02;
		
	}else if (_pos == LEDOUT_POS_1){
		
		if (useGroupDimming)
			return 0x0C;
		
		return 0x08;

	}else if (_pos == LEDOUT_POS_2){
		
		if (useGroupDimming)
			return 0x30;
		
		return 0x20;
		
	}else if (_pos == LEDOUT_POS_3){
		
		if (useGroupDimming)
			return 0xC0;
		
		return 0x80;
	}
	
	return 0x00;
}

unsigned char		TLC59116	::	getMaskOff				(const u_int8_t _pos) const
{
	if (_pos == LEDOUT_POS_0)
		return 0xFC;
		
	else if (_pos == LEDOUT_POS_1)
		return 0xF3;
		
	else if (_pos == LEDOUT_POS_2)
		return 0xCF;
	
	else if (_pos == LEDOUT_POS_3)
		return 0x3F;
		
	
	return 0x00;
}

void TLC59116	::	updateLedOutRegister	(const unsigned char _new_LedOut_Register_Value, unsigned char * _LedOut_Register, const u_int16_t _LedOut_Register_ADR)
{
	
	*_LedOut_Register = _new_LedOut_Register_Value;
	device->writeReg(MASK_NO_AUTO_INCR | _LedOut_Register_ADR, *_LedOut_Register);
}
