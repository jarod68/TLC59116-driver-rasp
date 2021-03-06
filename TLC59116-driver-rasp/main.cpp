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

int mutualArgError(std::string arg1, std::string arg2, std::string optionalText="")
{
	std::cerr << "You can't use " << arg1 <<" with " << arg2 << " " << optionalText << std::endl;
	return EXIT_FAILURE;
}

int multipleTimeArgError(std::string arg, std::string optionalText="")
{
	std::cerr << "You can only set option "<< arg << " one time " << optionalText << std::endl;
	return EXIT_FAILURE;
}

void printHelp()
{
	std::cout << "Usage:" << std::endl;
	std::cout << "tlc [OPTION]... --i2c-bus=I2C_BUS_FILE --i2c-adr=I2C_DEVICE_ADDRESS" << std::endl;
	std::cout << "Description:" << std::endl;
	std::cout << "This program is a driver for the TI TLC59116 I2C 16 output LED controller." << std::endl;
	std::cout << std::endl;
	std::cout << "[Options] are" << std::endl;
	std::cout << '\t' << "--led=VALUE : specified the led you want to control" << std::endl;
	std::cout << '\t' << "--on : turn the specifed led (--led) to ON" << std::endl;
	std::cout << '\t' << "--off : turn the specified led (--led) to OFF" << std::endl;
	std::cout << '\t' << "--set-pwm=VALUE : defined the PWM value [0-255] to use with the specified LED (--led)" << std::endl;
	std::cout << '\t' << "--get-pwm : print the current PWM value [0-255] in use by the specified LED (--led)" << std::endl;
	std::cout << '\t' << "--usegroup : use or not the group dimming (additional PWM signal)" << std::endl;
	std::cout << '\t' << "--get-group-pwm : print the current PWM value [0-255] in use for the group dimming" << std::endl;
	std::cout << '\t' << "--set-group-pwm=VALUE : defined the PWM value [0-255] to use with the group dimming" << std::endl;
	std::cout << '\t' << "--all-on : turn all LED ON" << std::endl;
	std::cout << '\t' << "--all-off : turn all LED OFF" << std::endl;
	std::cout << '\t' << "--set-all-pwm=VALUE : set all individual PWM values [0-255] with the specified value" << std::endl;
	std::cout << '\t' << "--reset : reset the controller, all LED OFF, no group dimming, and all PWM (inclusive the group dimming) are set to 255" << std::endl;
	std::cout << '\t' << "--help : print this help message" << std::endl;
	
	exit(EXIT_FAILURE);
}

int parseArguments (int argc, char ** argv)
{
	int c;
	//int digit_optind = 0;
	
	while (1) {
		//int this_option_optind = optind ? optind : 1;
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
						return multipleTimeArgError(long_options[ARG_LED__INDEX].name);
					}
					led=(int16_t) atoi(optarg);
					
					break;
					
				case ARG_ON__INDEX: //on
					if(on)
					{
						return multipleTimeArgError(long_options[ARG_ON__INDEX].name);
					}
					if(off)
					{
						return mutualArgError(long_options[ARG_ON__INDEX].name,long_options[ARG_OFF__INDEX].name);
					}
					on=true;
					
					break;
					
				case ARG_OFF__INDEX: //off
					if(off)
					{
						return multipleTimeArgError(long_options[ARG_OFF__INDEX].name);					}
					if(on)
					{
						return mutualArgError(long_options[ARG_ON__INDEX].name,long_options[ARG_OFF__INDEX].name);
					}
					off=true;
					break;
					
				case ARG_SET_PWM__INDEX: //set-pwm
					if(setPWM)
					{
						return multipleTimeArgError(long_options[ARG_SET_PWM__INDEX].name);
					}
					setPWM = true;
					pwm = (int16_t)atoi(optarg);
					break;
					
				case ARG_GET_PWM__INDEX: //get-pwm
					if(getPWM)
					{
						return multipleTimeArgError(long_options[ARG_GET_PWM__INDEX].name);
					}
					getPWM = true;
					
					break;
				case ARG_USEGROUP__INDEX: //usegroup
					if(setUseGroup)
					{
						return multipleTimeArgError(long_options[ARG_USEGROUP__INDEX].name);
					}
					setUseGroup = true;
					break;
				case ARG_SET_GROUP_PWM__INDEX: //set-group-pwm
					if(setGroupPWM)
					{
						return multipleTimeArgError(long_options[ARG_SET_GROUP_PWM__INDEX].name);
					}
					setGroupPWM = true;
					groupPwm = (int16_t)atoi(optarg);
					break;
				case ARG_GET_GROUP_PWM__INDEX: //get-group-pwm
					if(getGroupPWM)
					{
						return multipleTimeArgError(long_options[ARG_GET_GROUP_PWM__INDEX].name);
					}
					getGroupPWM = true;
					break;
					
				case ARG_I2C_BUS__INDEX: //i2c-bus
					if(i2cBus != "")
					{
						return multipleTimeArgError(long_options[ARG_I2C_BUS__INDEX].name);
					}
					i2cBus = string(optarg);
					std::cout <<"i2cBus set " << i2cBus << std::endl;
					break;
				case ARG_I2C_ADR__INDEX: //i2c-adr
					
					if(i2cAdr != 0x00)
					{
						return multipleTimeArgError(long_options[ARG_I2C_ADR__INDEX].name);
					}
					
					i2cAdr = (unsigned char) strtol(optarg, NULL, 0);
					std::cout <<"i2cAdr set "<< optarg << std::endl;
					break;
				case ARG_ALL_ON__INDEX: //all-on
					
					if(allOn)
					{
						return multipleTimeArgError(long_options[ARG_ALL_ON__INDEX].name);
					}
					if(allOff)
					{
						return mutualArgError(long_options[ARG_ALL_ON__INDEX].name,long_options[ARG_ALL_OFF__INDEX].name);
					}
					allOn=true;
					break;
				case ARG_ALL_OFF__INDEX: //all-off
					
					if(allOff)
					{
						return multipleTimeArgError(long_options[ARG_ALL_OFF__INDEX].name);
					}
					
					if(allOn)
					{
						return mutualArgError(long_options[ARG_ALL_ON__INDEX].name,long_options[ARG_ALL_OFF__INDEX].name);
					}
					
					allOff = true;
					break;
					
				case ARG_SET_ALL_PWM__INDEX: //set-all-pwm
					
					if(setAllPWM)
					{
						return multipleTimeArgError(long_options[ARG_SET_ALL_PWM__INDEX].name);
					}
					
					setAllPWM = true;
					pwm = (int16_t)atoi(optarg);
					break;
					
				case ARG_RESET__INDEX: //reset
					
					if(reset)
					{
						return multipleTimeArgError(long_options[ARG_RESET__INDEX].name);
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
