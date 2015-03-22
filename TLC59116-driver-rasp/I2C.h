/* ---------------------------------------------------------------------------
 ** This software is in the public domain, furnished "as is", without technical
 ** support, and with no warranty, express or implied, as to its usefulness for
 ** any purpose.
 **
 ** Author: Matthieu Holtz
 ** Year: 2015
 ** -------------------------------------------------------------------------*/

#pragma once

#ifndef I2C_H
#define I2C_H

#include <string>
#include <stdio.h>
#include <linux/i2c.h>
#include <linux/i2c-dev.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <string.h>

class I2C {
	
private:
    std::string		i2cFileName;
    unsigned char	deviceAddress; // i2c device address
	int16_t			i2cDescriptor; // i2c device descriptor
    
public:
			I2C		(std::string _i2cFileName, unsigned char _deviceAddress);
    virtual ~I2C	();
    
	virtual int16_t writeReg	(const unsigned char reg_addr, unsigned char data);
	virtual int16_t writeReg	(const unsigned char reg_addr, unsigned char * data,const int16_t data_size);
	virtual int16_t readReg		(const unsigned char reg_addr, unsigned char * data);
	
private:
	virtual int16_t openI2C		();
	virtual int16_t closeI2C	();
};


#endif
