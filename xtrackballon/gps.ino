byte gps_hour, gps_minute, gps_second;
long gps_lat, gps_lon;
unsigned long gps_fix_age;


// no need to store these in the RAM anyway
static char str_buffer[25];
prog_char GPSstr_poll[] PROGMEM = "$PUBX,00*33";
prog_char GPSstr_setup1[] PROGMEM = "$PUBX,40,ZDA,0,0,0,0*44";
prog_char GPSstr_setup2[] PROGMEM = "$PUBX,40,GLL,0,0,0,0*5C";
prog_char GPSstr_setup3[] PROGMEM = "$PUBX,40,VTG,0,0,0,0*5E";
prog_char GPSstr_setup4[] PROGMEM = "$PUBX,40,GSV,0,0,0,0*59";
prog_char GPSstr_setup5[] PROGMEM = "$PUBX,40,GSA,0,0,0,0*4E";
prog_char GPSstr_setup6[] PROGMEM = "$PUBX,40,GGA,0,0,0,0*5A";
prog_char GPSstr_setup7[] PROGMEM = "$PUBX,40,RMC,0,0,0,0*47";
PROGMEM const char *str_table[] = {
  GPSstr_poll, GPSstr_setup1, GPSstr_setup2, GPSstr_setup3, 
  GPSstr_setup4, GPSstr_setup5, GPSstr_setup6, GPSstr_setup7
};

void GPS_setup() {
  altserial.begin(9600);
  // switch baudrate to 4800 bps
  //altserial.println("$PUBX,41,1,0007,0003,4800,0*13"); 
  //altserial.begin(4800);
  //altserial.flush();
  
  delay(500);
  
  // turn off all NMEA sentences for the uBlox GPS module
  // ZDA, GLL, VTG, GSV, GSA, GGA, RMC
  altserial.println(strcpy_P(str_buffer, (char*)pgm_read_word(&str_table[1])));
  altserial.println(strcpy_P(str_buffer, (char*)pgm_read_word(&str_table[2])));
  altserial.println(strcpy_P(str_buffer, (char*)pgm_read_word(&str_table[3])));
  altserial.println(strcpy_P(str_buffer, (char*)pgm_read_word(&str_table[4])));
  altserial.println(strcpy_P(str_buffer, (char*)pgm_read_word(&str_table[5])));
  altserial.println(strcpy_P(str_buffer, (char*)pgm_read_word(&str_table[6])));
  altserial.println(strcpy_P(str_buffer, (char*)pgm_read_word(&str_table[7])));

  delay(500);
}

// request uBlox to give fresh data
boolean GPS_poll() {
  //altserial.println("$PUBX,00*33");
  altserial.println(strcpy_P(str_buffer, (char*)pgm_read_word(&str_table[0])));
  delay(300);
  unsigned long starttime = millis();
  while (true) {
    if (altserial.available()) {
      char c = altserial.read();
      #if DEBUG
        Serial.print(c);
      #endif
      if (gps.encode(c))
        return true;
    }
    // that's it, can't wait any longer
    // i have short attention span..
    if (millis() - starttime > 1000) {
      #if DEBUG
        Serial.println("timeout");
      #endif
      break;
    }
  }
  return false;
}
