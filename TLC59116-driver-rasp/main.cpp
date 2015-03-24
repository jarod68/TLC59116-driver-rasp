/// -------------------------------------------------------------------------
/// @file main.cpp
/// @brief implementation file for basic test/playing
/// @author Matthieu Holtz
/// @version 0
/// @date 2015
///
/// This software is in the public domain, furnished "as is", without technical
/// support, and with no warranty, express or implied, as to its usefulness for
/// any purpose.
/// -------------------------------------------------------------------------

#include <iostream>
#include <unistd.h>

#include "TLC59116.h"

using namespace std;


int main(int argc, const char * argv[]) {
	
    
    TLC59116 tlc ("/dev/i2c-1", 0x61, true);
	//tlc.allOn();
	bool flag = false;
	
		tlc.setOn(8);
		tlc.setPWMDimming(8, 70);
	
		bool complete = false;
		bool rising=true;
		u_int16_t pwm = 0;
		while (!complete){
			tlc.setGroupPWMDimming(pwm);
			std::cout << "LED 8 "	<< (int)tlc.getPWMDimming(8)		<<std::endl;
			std::cout << "Dim "		<< (int)tlc.getGroupPWMDimming()	<<std::endl;
			std::cout << "E1 "		<< (int)tlc.getErrorFLag1()			<<std::endl;
			std::cout << "E2 "		<< (int)tlc.getErrorFLag2()			<<std::endl;
			if(rising){
				pwm++;
				
				if (pwm == 0xFF){
					rising = false;
				}
				
			}else{
				pwm--;
				if (pwm == 0){
					rising = true;
				}
			}
			
			
			usleep(5500);
			
		}

	

	/*
	u_int8_t l = 0;
	while (true){
		for ( l = 0; l<16; l++){
		
			tlc.setOn(l);
			std::cout << "On " << (int)l <<std::endl;
			sleep(1);
		}
	
		for ( l = 0; l<16; l++){
		
			tlc.setOff(l);
			std::cout << "Off " << (int)l <<std::endl;
			sleep(1);
		}
	}
	*/
	//std::cout << "Open : "<< i2c.openI2C()<< "\n";
	
	
	/*
	unsigned char data[24] = { 0x00, 0x00, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0x00, 0x00, 0xF0, 0x0C, 0x0C, 0x0C, 0x00, 0x00 };
	
	std::cout << "write : "<< i2c.writeReg(0x80, data, 24)<< "\n";
	
	
	unsigned char resp;
	
	i2c.readReg((unsigned char)0x80, &resp);
	std::cout << "Read : "<< (int) resp << "\n";
	
	i2c.readReg((unsigned char)0x82, &resp);
	std::cout << "Read : "<< (int) resp << "\n";
	
	i2c.readReg((unsigned char)0x82, &resp);
	std::cout << "Read : "<< (int) resp << "\n";
	
	//std::cout << "Closed : "<< i2c.closeI2C()<< "\n";
	*/
	
	return 0;
}
