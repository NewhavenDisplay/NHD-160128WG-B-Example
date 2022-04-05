/**
 * @file NHD_160128WG_example.ino
 * @brief Program for writing to Newhaven Display 160x128 graphic LCD with RA6963 controller.
 *        This program draws vertical bars on the screen using 8-bit parallel.
 * @date 4-5-2022
 * 
 * (c)2022 Cody Johnson - Newhaven Display International, LLC.
 * 
 * Program Loop:
 * - Draw vertical bars
 * - 1 second delay
 *
 * Newhaven Display invests time and resources providing this open source code,
 * Please support Newhaven Display by purchasing products from Newhaven Display!
 *
 * @copyright Copyright (c) 2022, Newhaven Display International
 * 
 * This code is provided as an example only and is not guaranteed by Newhaven Display.
 * Newhaven Display accepts no responsibility for any issues resulting from its use.
 * The developer of the final application incorporating any parts of this
 * sample code is responsible for ensuring its safe and correct operation
 * and for any consequences resulting from its use.
 */

/**********************************************************
 *   Wiring Reference:
 *   - 1 FG - GND
 *   - 2 VSS - GND and side of 10k potentiometer
 *   - 3 VDD - 5V
 *   - 4 V0 - 10k potentiometer wiper
 *   - 5 VEE - Side of 10k potentiometer
 *   - 6 /WR - D31
 *   - 7 /RD - D32
 *   - 8 /CE - GND
 *   - 9 C/D - D30
 *   - 10 /HALT - D39
 *   - 11 /RES - D33
 *   - 12 DB0 - D22
 *   - 13 DB1 - D23
 *   - 14 DB2 - D24
 *   - 15 DB3 - D25
 *   - 16 DB4 - D26
 *   - 17 DB5 - D27
 *   - 18 DB6 - D28
 *   - 19 DB7 - D29
 *********************************************************/

// Interface globals
int _CD = 30;      // RS (C/D) signal connected to Arduino digital pin 30
int _WR = 31;      // /WR signal connected to Arduino digital pin 31
int _RD = 32;      // /RD signal connected to Arduino digital pin 32
int _RES = 33;     // /RES signal connected to Arduino digital pin 33
int _HALT = 39;    // /HALT signal connected to Arduino digital pin 39

/**
 * @brief Run once on startup.
 */
void setup()
{
  initLCD();
}

/**
 * @brief Loop infinitely.
 */
void loop()
{ 
  drawVerticalBars();
  delay(1000);
}

/**
 * @brief Set the C/D signal HIGH to indicate
 *        command bytes to be sent.
 * 
 * @return none 
 */
void setCommand()
{
  digitalWrite(_CD, HIGH);      //C/D = H = command
}

/**
 * @brief Set the C/D signal LOW to indicate
 *        data bytes to be sent.
 * 
 * @return none
 */
void setData()
{
  digitalWrite(_CD, LOW);     //C/D = L = data
}

/**
 * @brief Set the WR signal LOW to indicate
 *        a write sequence is occuring.
 * 
 * @return none
 */
void enableWrite()
{
  digitalWrite(_WR, LOW);
}

/**
 * @brief Set the WR signal HIGH to indicate
 *        that writing is disabled.
 * 
 * @return none
 */
void disableWrite()
{
  digitalWrite(_WR, HIGH);
}

/**
 * @brief Toggle the WR signal LOW to HIGH to 
 *        latch data.
 * 
 * @return none
 */
void dataLatch()
{
  enableWrite();
  delay(1);
  disableWrite();
  delay(1);
}

/**
 * @brief Send a command byte to the display.
 * 
 * @param command Command byte to be sent.
 * @return none 
 */
void comm_out(unsigned char command)
{
  setCommand();

  PORTA = command; // Put command byte on port A (D22-D29)
  dataLatch();
}

/**
 * @brief Send a data byte to the display.
 * 
 * @param data Data byte to be sent.
 * @return none
 */
void data_out(unsigned char data)
{
  setData();

  PORTA = data; // Put data byte on port A (D22-D29)
  dataLatch();
}

/**
 * @brief Perform a hardware reset on the display.
 * 
 * @return none
 */
void reset()
{
  digitalWrite(_RES, LOW);
  delay(100);
  digitalWrite(_RES, HIGH);
  delay(100);
}

/**
 * @brief Initialize IO ports and the connected LCD display.
 * 
 * @return none
 */
void initLCD()
{
  DDRA = 0xFF;         //set PORTA (data signals) as output (D22-D29)
  PORTA = 0x00;
  pinMode(_CD, OUTPUT);
  pinMode(_RD, OUTPUT);
  pinMode(_WR, OUTPUT);
  pinMode(_RES, OUTPUT);
  pinMode(_HALT, OUTPUT);

  digitalWrite(_RD, HIGH);
  digitalWrite(_WR, HIGH);
  digitalWrite(_RES, HIGH);
  digitalWrite(_HALT, HIGH);
  
  reset();

  data_out(0x00);
  data_out(0x00);
  comm_out(0x40);      //text home address
  data_out(0x00);
  data_out(0x40);

  comm_out(0x42);      //graphic home address
  data_out(0x1E);
  data_out(0x00);  
  
  comm_out(0x41);      //text area
  data_out(0x1E);
  data_out(0x00);  
  
  comm_out(0x43);      //graphic area

  comm_out(0x80);      //OR mode     
  
  delay(100);
}

/**
 * @brief Draw vertical bars on the entire screen.
 * 
 * @return none
 */
void drawVerticalBars()
{
  comm_out(0x98); //graphics ON, text OFF
  data_out(0x00);
  data_out(0x40);
  comm_out(0x24); //set address pointer = 0x0040 (first pixel)

  comm_out(0xB0); //auto write ON

  int width = 160;
  int height = 128;
  int pixelsPerByte = 8;
  //fill screen with vertical bars
  for (unsigned int i = 0; i < (width / pixelsPerByte) * height; i++)
  {
    data_out(0x1C); // Send graphical data
  }
  comm_out(0xB2); //auto write OFF
}
