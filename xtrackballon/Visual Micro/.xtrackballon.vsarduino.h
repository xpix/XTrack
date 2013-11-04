/* 
	Editor: http://www.visualmicro.com
	        arduino debugger, visual micro +, free forum and wiki
	
	Hardware: Arduino Uno, Platform=avr, Package=arduino
*/

#define __AVR_ATmega328P__
#define _VMDEBUG 1
#define ARDUINO 101
#define ARDUINO_MAIN
#define F_CPU 16000000L
#define __AVR__
#define __cplusplus
extern "C" void __cxa_pure_virtual() {;}

//
//
void getGPSData();
int sensors_vin();
void simpleThread_setup(sendData);
boolean simpleThread_loop(sendData);
void rtty_txstring (char * string);
void rtty_txbyte (char c);
void rtty_txbit (int bit);
uint16_t gps_CRC16_checksum (char *string);
void setDataString();
float meters_to_feet(float m);
void beep(unsigned char delayms);

#include "C:\Users\xpix\Dropbox\arduino-1.0.5\hardware\arduino\variants\standard\pins_arduino.h" 
#include "C:\Users\xpix\Dropbox\arduino-1.0.5\hardware\arduino\cores\arduino\arduino.h"
#include "C:\Users\xpix\Dropbox\GitHub\XTrack\xtrackballon\xtrackballon.ino"
#include "C:\Users\xpix\Dropbox\GitHub\XTrack\xtrackballon\FloatToString.h"
#include "C:\Users\xpix\Dropbox\GitHub\XTrack\xtrackballon\rtty.ino"
#include "C:\Users\xpix\Dropbox\GitHub\XTrack\xtrackballon\send.ino"
