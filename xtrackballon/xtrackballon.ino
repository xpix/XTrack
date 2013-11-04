/*  XTrackBalloon
 
    Small firmware to send position data via RTTY.
 
    Created 2013 by XPIX.
*/ 
#include <string.h>
#include <util/crc16.h>

#include <Wire.h>
#include <BMP180.h>
#include <TinyGPS_UBX.h>
#include <AltSoftSerial.h>
#include <simpleThread.h>
#include "FloatToString.h"

#define DEBUG  true
 
// Set your callsign and SSID here. Common values for the SSID are
// (from http://zlhams.wikidot.com/aprs-ssidguide):
//
// - Balloons:  11
// - Cars:       9
// - Home:       0
// - IGate:      5
#define S_CALLSIGN      "FSHAB2"
#define S_CALLSIGN_ID   "11"

// Destination callsign: APRS (with SSID=0) is usually okay.
#define D_CALLSIGN      "APRS"
#define D_CALLSIGN_ID   "0"

// Digipeating paths:
// (read more about digipeating paths here: http://wa8lmf.net/DigiPaths/ )
// The recommended digi path for a balloon is WIDE2-1 or pathless. The default
// is pathless. Uncomment the following two lines for WIDE2-1 path:
#define DIGI_PATH1      "WIDE2"
#define DIGI_PATH1_TTL  "1"

// APRS comment: this goes in the comment portion of the APRS message. You
// might want to keep this short. The longer the packet, the more vulnerable
// it is to noise.
#define APRS_COMMENT    "HAB/102013"

// Intervall in milli seconds to send APRS Data
#define GPS__INTERVAL   2000
#define APRS_INTERVAL   5000

#define CTRL_LED  12    // visual control light
#define RTTY_PIN  13		// Output pin for TX2H or HX1
#define RTTY_BAUD 300   // possible values: 50, 300
#define RTTY_BIT  7     // Change this here 7 or 8 for ASCII-7 / ASCII-8
#define GPSBaud   9600  // baud settings in GPS Device
#define BUZZ_PIN  5     // Buzzer pin

// Resistors divider for the voltage meter (ohms)
#define VMETER_R1       9140
#define VMETER_R2       3300

// Voltage meter analog pin
#define VMETER_PIN      2

// Store the current sea level pressure at your location in Pascals.
float seaLevelPressure = 102825;

// if over 1000m switch on
boolean inthesky = false;

// Add Threads to refresh status informations from GRBL
#define _sT_cnt  _sT_cnt_2    // count of threads(?, $G)
simpleThread_init(_sT_cnt);   // init threads
simpleThread_new_timebased_dynamic  (_sT_P1  , _sT_millis, GPS__INTERVAL, _sT_start ,  getData);	// get Data from GPS
simpleThread_new_timebased_dynamic  (_sT_P2  , _sT_millis, APRS_INTERVAL, _sT_start ,  sendData);	// send Data in air

// init's 
BMP180 barometer;
TinyGPS gps;
AltSoftSerial altserial;  // stable extra serial device on Pin 8,9
 
char datastring[80];
 
void setup() {                
  pinMode(CTRL_LED,OUTPUT);
  pinMode(RTTY_PIN,OUTPUT);

  GPS_setup();

  /* init threads */
  simpleThread_initSetup(_sT_cnt);

  beep(50);

  // We start the serial library to output our messages.
  Serial.begin(9600);
  // We start the I2C on the Arduino for communication with the BMP180 sensor.
  Wire.begin();

  // ----------- Barometer ------------------

  // We create an instance of our BMP180 sensor.
  barometer = BMP180();
  // We check to see if we can connect to the sensor.
  if(barometer.EnsureConnected())
  {
    Serial.println(F("Connected to BMP180.")); // Output we are connected to the computer.
    digitalWrite(CTRL_LED, HIGH);   // Set our LED.
    delay(200);                     // First Flash --> Barometer ok!
    digitalWrite(CTRL_LED, LOW);    // Set our LED.

     // When we have connected, we reset the device to ensure a clean start.
    barometer.SoftReset();
    // Now we initialize the sensor and pull the calibration data.
    barometer.Initialize();
  }

  // ----------- GPS ------------------
  altserial.begin(GPSBaud);
  Serial.println(F("Connect to GPS ..."));
  altserial.println("WakeUp");
  digitalWrite(CTRL_LED, HIGH);   // Set our LED.
  delay(200);                     // Second Flash --> GPS ok!
  digitalWrite(CTRL_LED, LOW);    // Set our LED.
  delay(1000);
}
 
void loop() {
   // Jobs
   simpleThread_run(_sT_priority);
}

int sensors_vin()
{
	analogReference(DEFAULT);      // Ref=5V
	analogRead(VMETER_PIN);        // Disregard the 1st conversion after changing ref (p.256)
	delay(10);                     // This is needed when switching references

	uint16_t adc = analogRead(VMETER_PIN);
	uint16_t mV = 5000L * adc / 1024;
	
	// Vin = mV * R2 / (R1 + R2)
	int vin = (uint32_t)mV * (VMETER_R1 + VMETER_R2) / VMETER_R2;
	return vin;
}

void simpleThread_setup(getData)
{
   // output
}

boolean simpleThread_loop(getData)
{
   // get GPS Data
   GPS_poll();
}

void simpleThread_setup(sendData)
{
   // output
}

boolean simpleThread_loop(sendData)
{
   // complete Datastring with all Data
   setDataString();

   // send this string via RTTY
   rtty_txstring(datastring);
}
