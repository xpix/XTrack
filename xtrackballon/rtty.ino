/* ===============================================
 * Project: XTrackBallon, rtty
 * ===============================================
 * Autor:     Frank (xpix) Herrmann
 * Email:     xpixer@gmail.com
 * License:   all Free
 * Last edit: 30.09.2013
 * RTTY code from Rob Harrison Icarus Project.
   http://ukhas.org.uk
 */
void rtty_txstring (char * string)
{

  /* Simple function to sent a char at a time to
   	** rtty_txbyte function.
   	** NB Each char is one byte (8 Bits)
   	*/

  char c;

  c = *string++;

  while ( c != '\0')
  {
    rtty_txbyte (c);
    c = *string++;
  }
}


void rtty_txbyte (char c)
{
  /* Simple function to sent each bit of a char to
   	** rtty_txbit function.
   	** NB The bits are sent Least Significant Bit first
   	**
   	** All chars should be preceded with a 0 and
   	** proceded with a 1. 0 = Start bit; 1 = Stop bit
   	**
   	*/

  int i;

  rtty_txbit (0); // Start bit

  // Send bits for for char LSB first	

  for (i=0;i<RTTY_BIT;i++) // Change this here 7 or 8 for ASCII-7 / ASCII-8
  {
    if (c & 1) rtty_txbit(1);

    else rtty_txbit(0);	

    c = c >> 1;

  }

  rtty_txbit (1); // Stop bit
  rtty_txbit (1); // Stop bit
}

void rtty_txbit (int bit)
{
  if (bit)
  {
    // high
    digitalWrite(RTTY_PIN, HIGH);
  }
  else
  {
    // low
    digitalWrite(RTTY_PIN, LOW);
  }

  #if RTTY_BAUD == 300  
   delayMicroseconds(3370); // 300 baud
  #endif

  #if RTTY_BAUD == 50
   delayMicroseconds(10000); // For 50 Baud uncomment this and the line below.
   delayMicroseconds(10150); // You can't do 20150 it just doesn't work as the
                            // largest value that will produce an accurate delay is 16383
                            // See : http://arduino.cc/en/Reference/DelayMicroseconds
  #endif
}

uint16_t gps_CRC16_checksum (char *string)
{
  size_t i;
  uint16_t crc;
  uint8_t c;

  crc = 0xFFFF;

  // Calculate checksum ignoring the first two $s
  for (i = 2; i < strlen(string); i++)
  {
    c = string[i];
    crc = _crc_xmodem_update (crc, c);
  }

  return crc;
}    
