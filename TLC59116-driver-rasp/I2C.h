/// -------------------------------------------------------------------------
/// @file I2C.h
/// @brief header file for the I2C class.
/// @author Matthieu Holtz
/// @version 0
/// @date 2015
///
/// This software is in the public domain, furnished "as is", without technical
/// support, and with no warranty, express or implied, as to its usefulness for
/// any purpose.
/// -------------------------------------------------------------------------

#pragma once

#ifndef I2C_H
#define I2C_H


#include <linux/i2c.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>

#include <string.h>
#include <string>
#include <stdio.h>

/// @class I2C
/// @brief this is the abstraction for an I2C device
/// It indends to work on a Linux plateform with I2C support and offers the write and read function on the bus
///
class I2C {
	
private:
    std::string		i2cFileName;	///< the filename of the I2C device controller in Linux OS
    unsigned char	deviceAddress;	///< i2c device address on the bus
	int16_t			i2cDescriptor;	///< i2c device descriptor
    
public:
	/// @brief Constructor with I2C controller filename and device address on the bus
	///
	/// @param _i2cFileName		: the linux I2C device filename i.e. '/dev/i2c-1'
	/// @param _deviceAddress	: the device address on the I2C bus
			I2C		(std::string _i2cFileName, unsigned char _deviceAddress);
	
	/// @brief Destructor
    virtual ~I2C	();
	
	/// @brief Write the a byte data in the specified register address
	///
	/// @param reg_addr	: the shift register address
	/// @param data		: the single byte data to write
	/// @return the POSIX error code
	virtual int16_t writeReg	(const unsigned char reg_addr, unsigned char data);
	
	/// @brief Write the a byte array data in the specified register address
	///
	/// @param reg_addr	: the shift register address
	/// @param data		: the byte array to write
	/// @param data_size: the byte array size
	/// @return the POSIX error code
	virtual int16_t writeReg	(const unsigned char reg_addr, unsigned char * data,const int16_t data_size);
	
	/// @brief Read a single byte data in a shift register
	///
	/// @param reg_addr	: the shift register address
	/// @param data		: the byte array buffer to store the data
	/// @return the POSIX error code
	virtual int16_t readReg		(const unsigned char reg_addr, unsigned char * data);
	
private:
	
	/// @brief Open the I2C file descriptor
	virtual int16_t openI2C		();
	
	/// @brief Close the I2C file descriptor
	virtual int16_t closeI2C	();
};


#endif
