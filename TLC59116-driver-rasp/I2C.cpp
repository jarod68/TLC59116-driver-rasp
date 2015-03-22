/* ---------------------------------------------------------------------------
 ** This software is in the public domain, furnished "as is", without technical
 ** support, and with no warranty, express or implied, as to its usefulness for
 ** any purpose.
 **
 ** Author: Matthieu Holtz
 ** Year: 2015
 ** -------------------------------------------------------------------------*/

#include "I2C.h"


I2C		::		I2C		(std::string _i2cFileName, unsigned char _deviceAddress)
						:
						i2cFileName(_i2cFileName),
						deviceAddress(_deviceAddress),
						i2cDescriptor(-1)
{
    
}

I2C		::		~I2C		()
{
    
}

int16_t I2C	::	openI2C		()
{
    this->i2cDescriptor = open(i2cFileName.c_str(), O_RDWR);
    if(this->i2cDescriptor < 0){
        perror("Could not open file (1)");
        exit(1);
    }
    
    return i2cDescriptor;
}

int16_t I2C	::	closeI2C	()
{
    int16_t retVal = -1;
    retVal = close(this->i2cDescriptor);
    if(retVal < 0){
        perror("Could not close file (1)");
        exit(1);
    }
    return retVal;
}


int16_t I2C	::	writeReg	(const unsigned char reg_addr, unsigned char data)
{
	return this->writeReg(reg_addr, &data, 1);
}

int16_t I2C	::	writeReg	(const unsigned char reg_addr, unsigned char * data, const int16_t data_size)
{
	const int16_t buff_size = data_size + 1;
	unsigned char buff[buff_size];
	int16_t retVal = -1;
	struct i2c_rdwr_ioctl_data packets;
	struct i2c_msg messages[1];
 
	buff[0] = reg_addr;
	memcpy(&buff[1], data, data_size);

	//buff[1] = data;
 
	messages[0].addr = deviceAddress;
	messages[0].flags = 0;
	messages[0].len = buff_size;
	messages[0].buf = buff;
 
	packets.msgs = messages;
	packets.nmsgs = 1;
 
	this->openI2C();
	retVal = ioctl(this->i2cDescriptor, I2C_RDWR, &packets);
	
	if(retVal < 0)
		perror("Write to I2C Device failed");
 
	this->closeI2C();
	
	return retVal;
}

int16_t I2C	::	readReg		(const unsigned char reg_addr, unsigned char * data){
 
	unsigned char *inbuff, outbuff;
	int16_t retVal = -1;
	struct i2c_rdwr_ioctl_data packets;
	struct i2c_msg messages[2];
 
	outbuff = reg_addr;
	messages[0].addr = deviceAddress;
	messages[0].flags= 0;
	messages[0].len = sizeof(outbuff);
	messages[0].buf = &outbuff;
 
	inbuff = data;
	messages[1].addr = deviceAddress;
	messages[1].flags = I2C_M_RD;
	messages[1].len = sizeof(*inbuff);
	messages[1].buf = inbuff;
 
	packets.msgs = messages;
	packets.nmsgs = 2;
	this->openI2C();
	retVal = ioctl(this->i2cDescriptor, I2C_RDWR, &packets);
	
	if(retVal < 0)
		perror("Read from I2C Device failed");
	
	this->closeI2C();
	return retVal;
}
