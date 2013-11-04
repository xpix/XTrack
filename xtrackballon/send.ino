void setDataString(){
	float altitude, currentTemperature;
	if(barometer.IsConnected)
	{
		// Retrive the current altitude (in meters). Current Sea Level Pressure is required for this.
		altitude = barometer.GetAltitude(seaLevelPressure);

		if( altitude > 1000)
		inthesky = true;

		// Retrive the current temperature in degrees celcius.
		currentTemperature = barometer.GetTemperature();
	}

	// Beep!!!
	if(inthesky and altitude < 200)
	beep(200);

	char buffer[25];
	datastring[0] = '\0';
	
	// DJ7OO-11>APRS,WIDE2-1:/113801h4957.60N/00811.98EO000/000/A=000895/Ti=58/Te=107/V=8439 APRS Additional Text*CRC

	// Header: DJ7OO-11>APRS,WIDE2-1:
	strcat(datastring, S_CALLSIGN);
	strcat(datastring, "-");
	strcat(datastring, S_CALLSIGN_ID);
	strcat(datastring, ">");
	strcat(datastring, DIGI_PATH1);
	strcat(datastring, "-");
	strcat(datastring, DIGI_PATH1_TTL);

	strcat(datastring, ":/");

	// Time 112021h
   int year;
   byte month, day, hour, minute, second, hundredths;
   unsigned long age;
   gps.crack_time(&hour, &minute, &second, &age);
   snprintf(buffer, 7, "%02d%02d%02d ", hour, minute, second);
	strcat(datastring, buffer);
	strcat(datastring, "h");

   float flat, flon;
   gps.f_get_position(&flat, &flon, &age);


	// Position 49.386227,8.59385
	strcat(datastring, floatToString(buffer, flat, 6));
	strcat(datastring, "N/");

	strcat(datastring, floatToString(buffer, flon, 6));
	strcat(datastring, "E");

	strcat(datastring, "O");	// Symbol: O=balloon, -=QTH
        snprintf(buffer, 4, "%03d", (int)(gps.f_course()));
	strcat(datastring, buffer);	// Course in degrees

	strcat(datastring, "/");
	snprintf(buffer, 4, "%03d", (int)(gps.f_speed_kmph()));
	strcat(datastring, buffer);	// Speed in km/h

	// Barometer Altitude in feets
	strcat(datastring, "/A=");
	strcat(datastring, floatToString(buffer, gps.f_altitude(), 2));

	// Barometer temperature inside
	strcat(datastring, "/TI=");
	strcat(datastring, floatToString(buffer, currentTemperature, 2));

	// Vin
	strcat(datastring, "/V=");
	snprintf(buffer, 6, "%d", sensors_vin());
	strcat(datastring, buffer);	// Course in degrees

	// Comment
	strcat(datastring, " ");
	strcat(datastring, APRS_COMMENT);	// Course in degrees
	
	unsigned int CHECKSUM = gps_CRC16_checksum(datastring);  // Calculates the checksum for this datastring
	char checksum_str[6];
	sprintf(checksum_str, "*%04X\n", CHECKSUM);
	strcat(datastring, checksum_str);

	Serial.print(datastring);
}


void beep(unsigned char delayms){
	analogWrite(BUZZ_PIN, 20);      // Almost any value can be used except 0 and 255
	// experiment to get the best tone
	delay(delayms);                 // wait for a delayms ms
	analogWrite(BUZZ_PIN, 0);       // 0 turns it off
	delay(delayms);                 // wait for a delayms ms
}
