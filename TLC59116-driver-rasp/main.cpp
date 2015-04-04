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
#include <cstdlib>
#include <getopt.h>

#include "TLC59116.h"

using namespace std;

#ifdef TEST
int main(int argc, const char * argv[])
{
	
    
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

	
	
	return EXIT_SUCCESS;
}
#else

string i2cBus ("");
unsigned char i2cAdr		= 0x00;

int16_t led					= -1;
u_int16_t pwm				= -1;
u_int16_t groupPwm			= -1;

bool on						= false;
bool off					= false;
bool setPWM					= false;
bool getPWM					= false;
bool setUseGroup			= false;
bool setGroupPWM			= false;
bool getGroupPWM			= false;
bool allOn					= false;
bool allOff					= false;
bool setAllPWM				= false;
bool reset					= false;
bool help					= false;

static struct option long_options[] = {
										{"led",				required_argument,	0,  0 },
										{"on",				no_argument,		0,  0 },
										{"off",				no_argument,		0,  0 },
										{"set-pwm",			required_argument,	0,  0 },
										{"get-pwm",			no_argument,		0,  0 },
										{"usegroup",		no_argument,		0,  0 },
										{"set-group-pwm",   required_argument,	0,  0 },
										{"get-group-pwm",   required_argument,	0,  0 },
										{"i2c-bus",			required_argument,	0,  0 },
										{"i2c-adr",			required_argument,	0,  0 },
										{"all-on",			no_argument,		0,  0 },
										{"all-off",			no_argument,		0,  0 },
										{"set-all-pwm",		required_argument,	0,  0 },
										{"reset",			no_argument,		0,  0 },
										{"help",			no_argument,		0,  0 },
										{0,					0,					0,  0 }
};

#define ARG_LED__INDEX				0
#define ARG_ON__INDEX				1
#define ARG_OFF__INDEX				2
#define ARG_SET_PWM__INDEX			3
#define ARG_GET_PWM__INDEX			4
#define ARG_USEGROUP__INDEX			5
#define ARG_SET_GROUP_PWM__INDEX	6
#define ARG_GET_GROUP_PWM__INDEX	7
#define ARG_I2C_BUS__INDEX			8
#define ARG_I2C_ADR__INDEX			9
#define ARG_ALL_ON__INDEX			10
#define ARG_ALL_OFF__INDEX			11
#define ARG_SET_ALL_PWM__INDEX		12
#define ARG_RESET__INDEX			13
#define ARG_HELP__INDEX				14

void printHelp()
{
	std::cerr << "Usage:" << std::endl;
	exit(EXIT_FAILURE);
}

int parseArguments (int argc, char ** argv)
{
	int c;
	int digit_optind = 0;
	
	while (1) {
		int this_option_optind = optind ? optind : 1;
		int option_index = 0;
		
		c = getopt_long(argc, argv, "h?", long_options, &option_index);
		
		if (c == -1)
			break;
		
		if (c == 0)
		{
			switch (option_index) {
				case ARG_LED__INDEX: //led
					if(led != -1)
					{
						std::cerr << "You can only set option "<< long_options[ARG_LED__INDEX].name << " one time" << std::endl;
						return EXIT_FAILURE;
					}
					led=(int16_t) atoi(optarg);
					
					break;
					
				case ARG_ON__INDEX: //on
					if(on)
					{
						std::cerr << "You can only set option "<< long_options[ARG_ON__INDEX].name << " one time" << std::endl;
						return EXIT_FAILURE;
					}
					if(off)
					{
						std::cerr << "You can't use " << long_options[ARG_ON__INDEX].name <<" with " << long_options[ARG_OFF__INDEX].name << std::endl;
						return EXIT_FAILURE;
					}
					on=true;
					
					break;
					
				case ARG_OFF__INDEX: //off
					if(off)
					{
						std::cerr << "You can only set option "<< long_options[ARG_OFF__INDEX].name << " one time" << std::endl;
						return EXIT_FAILURE;
					}
					if(on)
					{
						std::cerr << "You can't use " << long_options[ARG_ON__INDEX].name <<" with " << long_options[ARG_OFF__INDEX].name << std::endl;
						return EXIT_FAILURE;
					}
					off=true;
					break;
					
				case ARG_SET_PWM__INDEX: //set-pwm
					if(setPWM)
					{
						std::cerr << "You can only set option "<< long_options[ARG_SET_PWM__INDEX].name << " one time" << std::endl;
						return EXIT_FAILURE;
					}
					setPWM = true;
					pwm = (int16_t)atoi(optarg);
					break;
					
				case ARG_GET_PWM__INDEX: //get-pwm
					if(getPWM)
					{
						std::cerr << "You can only set option "<< long_options[ARG_GET_PWM__INDEX].name << " one time" << std::endl;
						return EXIT_FAILURE;
					}
					getPWM = true;
					
					break;
				case ARG_USEGROUP__INDEX: //usegroup
					if(setUseGroup)
					{
						std::cerr << "You can only apply option "<< long_options[ARG_USEGROUP__INDEX].name << " one time" << std::endl;
						return EXIT_FAILURE;
					}
					setUseGroup = true;
					break;
				case ARG_SET_GROUP_PWM__INDEX: //set-group-pwm
					if(setGroupPWM)
					{
						std::cerr << "You can only set option "<< long_options[ARG_SET_GROUP_PWM__INDEX].name << " one time" << std::endl;
						return EXIT_FAILURE;
					}
					setGroupPWM = true;
					groupPwm = (int16_t)atoi(optarg);
					break;
				case ARG_GET_GROUP_PWM__INDEX: //get-group-pwm
					if(getGroupPWM)
					{
						std::cerr << "You can only set option "<< long_options[ARG_GET_GROUP_PWM__INDEX].name << " one time" << std::endl;
						return EXIT_FAILURE;
					}
					getGroupPWM = true;
					break;
					
				case ARG_I2C_BUS__INDEX: //i2c-bus
					if(i2cBus != "")
					{
						std::cerr << "You can only set option "<< long_options[ARG_I2C_BUS__INDEX].name << " one time" << std::endl;
						return EXIT_FAILURE;
					}
					i2cBus = string(optarg);
					std::cout <<"i2cBus set " << i2cBus << std::endl;
					break;
				case ARG_I2C_ADR__INDEX: //i2c-adr
					
					if(i2cAdr != 0x00)
					{
						std::cerr << "You can only set option " << long_options[ARG_I2C_ADR__INDEX].name << " one time" << std::endl;
						return EXIT_FAILURE;
					}
					
					i2cAdr = (unsigned char) strtol(optarg, NULL, 0);
					std::cout <<"i2cAdr set "<< optarg << std::endl;
					break;
				case ARG_ALL_ON__INDEX: //all-on
					
					if(allOn)
					{
						std::cerr << "You can only set option "<< long_options[ARG_ALL_ON__INDEX].name << " one time" << std::endl;
						return EXIT_FAILURE;
					}
					if(allOff)
					{
						std::cerr << "You can't use " << long_options[ARG_ALL_ON__INDEX].name <<" with " << long_options[ARG_ALL_OFF__INDEX].name << std::endl;
						return EXIT_FAILURE;
					}
					allOn=true;
					break;
				case ARG_ALL_OFF__INDEX: //all-off
					
					if(allOff)
					{
						std::cerr << "You can only set option "<< long_options[ARG_ALL_OFF__INDEX].name << " one time" << std::endl;
						return EXIT_FAILURE;
					}
					
					if(allOn)
					{
						std::cerr << "You can't use " << long_options[ARG_ALL_ON__INDEX].name <<" with " << long_options[ARG_ALL_OFF__INDEX].name << std::endl;
						return EXIT_FAILURE;
					}
					
					allOff = true;
					break;
					
				case ARG_SET_ALL_PWM__INDEX: //set-all-pwm
					
					if(setAllPWM)
					{
						std::cerr << "You can only set option "<< long_options[ARG_SET_ALL_PWM__INDEX].name << " one time" << std::endl;
						return EXIT_FAILURE;
					}
					
					setAllPWM = true;
					pwm = (int16_t)atoi(optarg);
					break;
					
				case ARG_RESET__INDEX: //reset
					
					if(reset)
					{
						std::cerr << "You can only apply option "<< long_options[ARG_RESET__INDEX].name << " one time" << std::endl;
						return EXIT_FAILURE;
					}
					
					reset=true;
					break;
					
				case ARG_HELP__INDEX: //help
					printHelp();
					break;
					
				default:
					break;
			}
			
		}else if (c == '?')
		{
			printHelp();
			
		}else if (c == 'h')
		{
			printHelp();
		}
		else{
			std::cout << "?? getopt returned character code "<< c << std::endl;
			printHelp();
		}
		
	}
	
	if (optind < argc) {
		
		std::cout <<"non-option ARGV-elements: ";
		
		while (optind < argc)
			std::cout << argv[optind++];
		
		std::cout << std::endl;
	}

	return EXIT_SUCCESS;
	
}

int main(int argc, char ** argv)
{
	
	if(parseArguments(argc, argv) == EXIT_FAILURE)
		return EXIT_FAILURE;
	
	
	if (i2cBus == "" || i2cAdr == 0x00 )
	{
		std::cout <<long_options[ARG_I2C_ADR__INDEX].name <<" and "<<long_options[ARG_I2C_BUS__INDEX].name << " need to be set with correct values" << std::endl;
		return EXIT_FAILURE;

	}
	
	TLC59116 tlc (i2cBus, i2cAdr, setUseGroup, reset);
	
	if (led >-1 && led <16)
	{
		if(on){
			tlc.setOn(led);
		}
		if(off)
		{
			tlc.setOff(led);
		}
		if(setPWM){
			tlc.setPWMDimming(led, pwm);
		}
		if(getPWM){
			std::cout <<"get-pwm="<< tlc.getPWMDimming(led)<<std::endl;
		}
	}

	if(setGroupPWM)
	{
		tlc.setGroupPWMDimming(groupPwm);
	}
	if(getGroupPWM){
		std::cout <<"get-group-pwm="<< tlc.getGroupPWMDimming()<<std::endl;
	}
	if(setAllPWM){
		tlc.setAllPWMDimming(pwm);
	}
	if(allOn)
	{
		tlc.allOn();
	}
	if(allOff)
	{
		tlc.allOff();
	}
	
	return EXIT_SUCCESS;
}


#endif
