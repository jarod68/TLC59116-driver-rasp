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
										{0,					0,					0,  0 }
};

void printHelp()
{
	
}

int parseArguments (int argc, char ** argv)
{
	int c;
	int digit_optind = 0;
	
	while (1) {
		int this_option_optind = optind ? optind : 1;
		int option_index = 0;
		
		c = getopt_long(argc, argv, "", long_options, &option_index);
		if (c == -1)
			
			break;
		if (c == 0)
		{
			switch (option_index) {
				case 0: //led
					if(led != -1)
					{
						std::cerr << "You can only set option --led one time" << std::endl;
						return EXIT_FAILURE;
					}
					led=(int16_t) atoi(optarg);
					
					break;
					
				case 1: //on
					if(on == true)
					{
						std::cerr << "You can only set option --on one time" << std::endl;
						return EXIT_FAILURE;
					}
					on=true;
					
					break;
					
				case 2: //off
					if(off == true)
					{
						std::cerr << "You can only set option --off one time" << std::endl;
						return EXIT_FAILURE;
					}
					off=true;
					break;
					
				case 3: //set-pwm
					if(setPWM == true)
					{
						std::cerr << "You can only set option --set-pwm one time" << std::endl;
						return EXIT_FAILURE;
					}
					if(getPWM == true)
					{
						std::cerr << "You can't use --get-pwm with --set-pwm" << std::endl;
						return EXIT_FAILURE;
					}
					setPWM = true;
					pwm = (int16_t)atoi(optarg);
					break;
					
				case 4: //get-pwm
					if(getPWM == true)
					{
						std::cerr << "You can only set option --get-pwm one time" << std::endl;
						return EXIT_FAILURE;
					}
					if(setPWM == true)
					{
						std::cerr << "You can't use --get-pwm with --set-pwm" << std::endl;
						return EXIT_FAILURE;
					}
					getPWM = true;
					
					break;
				case 5: //usegroup
					if(setUseGroup == true)
					{
						std::cerr << "You can only apply option --usegroup one time" << std::endl;
						return EXIT_FAILURE;
					}
					setUseGroup = true;
					break;
				case 6: //set-group-pwm
					if(setGroupPWM == true)
					{
						std::cerr << "You can only set option --set-group-pwm one time" << std::endl;
						return EXIT_FAILURE;
					}
					setGroupPWM = true;
					groupPwm = (int16_t)atoi(optarg);
					break;
				case 7: //get-group-pwm
					if(getGroupPWM == true)
					{
						std::cerr << "You can only set option --get-group-pwm one time" << std::endl;
						return EXIT_FAILURE;
					}
					getGroupPWM = true;
					break;
					
				case 8: //i2c-bus
					if(i2cBus != "")
					{
						std::cerr << "You can only set option --i2c-bus one time" << std::endl;
						return EXIT_FAILURE;
					}
					i2cBus = string(optarg);
					std::cout <<"i2cBus set " << optarg << std::endl;
					break;
				case 9: //i2c-adr
					
					if(i2cAdr != 0x00)
					{
						std::cerr << "You can only set option --i2c-adr one time" << std::endl;
						return EXIT_FAILURE;
					}
					
					i2cAdr = (unsigned char) strtol(optarg, NULL, 0);
					std::cout <<"i2cAdr set "<< optarg << std::endl;
					break;
				case 10: //all-on
					
					if(allOn == true)
					{
						std::cerr << "You can only set option --all-on one time" << std::endl;
						return EXIT_FAILURE;
					}
					
					allOn=true;
					break;
				case 11: //all-off
					
					if(allOff == true )
					{
						std::cerr << "You can only set option --all-off one time" << std::endl;
						return EXIT_FAILURE;
					}
					
					allOff = true;
					break;
					
				case 12: //set-all-pwm
					
					if(setAllPWM == true)
					{
						std::cerr << "You can only set option --set-all-pwm one time" << std::endl;
						return EXIT_FAILURE;
					}
					
					setAllPWM = true;
					pwm = (int16_t)atoi(optarg);
					break;
					
				case 13: //reset
					
					if(reset == true)
					{
						std::cerr << "You can only apply option --reset one time" << std::endl;
						return EXIT_FAILURE;
					}
					
					reset=true;
					break;
					
				default:
					break;
			}
			
		}else if (c =='?')
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
		std::cout <<"--i2c-adr and --i2c-bus need to be set with correct values" << std::endl;
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
