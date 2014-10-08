#include <avr/io.h>

const int LEFT_JOY_X   = 0;
const int LEFT_JOY_Y   = 1;
const int RIGHT_JOY_X  = 2;
const int RIGHT_JOY_Y  = 3;
const int DPAD_UP      = 8;
const int DPAD_RT      = 9;
const int DPAD_DN      = 10;
const int DPAD_LT      = 11;
const int L2           = 12;
const int R2           = 13;
const int L1           = 14;
const int R1           = 15;
const int TRIANGLE     = 16;
const int CIRCLE       = 17;
const int CROSS        = 18;
const int SQUARE       = 19;
const int L3           = 29;
const int R3           = 30;
const int SEL          = 28;
const int START        = 31;
const int PS           = 32;

const int ON           = 255;
const int OFF          = 0;

#define spi_xfer(b)    (SPDR = (b))
#define spi_is_busy()  (!(SPSR & _BV(SPIF)))

//
int lastCmd = 0;

unsigned long switchTime = 0;
unsigned long debounce = 200;

void JoystickInit()
{
  // Set up SPI pins
  DDRB = (DDRB & ~_BV(PB4)) | _BV(PB5) | _BV(PB3) | _BV(PB2);
  PORTB = (PORTB & ~(_BV(PB5) | _BV(PB4))) | _BV(PB2);

  // Enable SPI
  // Clock divisor max
  // no Double speed
  // MSB first
  // Idle low
  // Sample edge lead
  // No interrupt
  // Mode master
  SPCR = (_BV(SPE) | _BV(MSTR) | _BV(CPHA) | _BV(SPR0) | _BV(SPR1));
  SPSR = _BV(SPIF);
}


void JoystickControl(unsigned char control, unsigned char val)
{
  PORTB &= ~_BV(PB2);
  spi_xfer(control);
  while (spi_is_busy());
  spi_xfer(val);
  while (spi_is_busy());
  PORTB |= _BV(PB2);
}


void setup()
{
  JoystickInit();
  Serial.begin(19200);
}

int off = 0;

unsigned long last_millis = 0;

void loop()
{


  int numBytes = Serial.available();
  
  if (numBytes >= 2) {

    int x = Serial.read(); //read first byte
    int val = Serial.read(); //read second byte if available;

    if (x == '#') {
     Serial.flush(); 
    }
    if (x == 'L') {
      JoystickControl(LEFT_JOY_Y, val);
    } 

    else if (x == 'l') {
      JoystickControl(LEFT_JOY_X, val);
    }  
    else if (x == 'R') {
      JoystickControl(RIGHT_JOY_Y, val);
    } 
    else if (x == 'r') {
      JoystickControl(RIGHT_JOY_X, val);
    } 


    else if (x == 'X') {
      JoystickControl(CROSS, val);
    } 
    else if (x == 'T') {
      JoystickControl(TRIANGLE, val);
    } 
    else if (x == 'O') {
      JoystickControl(CIRCLE, val);
    } 
    else if (x == 'S') {
      JoystickControl(SQUARE, val);
    } 
    else if (x == 'P') {
      JoystickControl(PS, val);
    } 
    else if (x == '1') {
      JoystickControl(SEL, val);
    } 
    else if (x == '2') {
      JoystickControl(START, val);
    } 
    else if (x == 'i') {
      JoystickControl(DPAD_UP, val);
    } 
    else if (x == 'm') {
      JoystickControl(DPAD_DN, val);
    } 
    else if (x == 'k') {
      JoystickControl(DPAD_LT, val);
    } 
    else if (x == 'j') {
      JoystickControl(DPAD_RT, val);
    } 
    else if (x == '[') {
      JoystickControl(L2, val);
    } 
    else if (x == ']') {
      JoystickControl(R2, val);
    } 
    else if (x == '{') {
      JoystickControl(L3, val);
    } 
    else if (x == '}') {
      JoystickControl(R3, val);
    } 
    else if (x == '(') {
      JoystickControl(L1, val);
    } 
    else if (x == ')') {
      JoystickControl(R1, val);
    }

    last_millis = millis();

    lastCmd = x; //hold the last cmd received over serial
  } 
  
  
  /*
  else if (millis() - last_millis >= 100) {
    int i;
    if (lastCmd != 'l' && lastCmd  != 'L' && lastCmd !='r' && lastCmd != 'R') {

      for (i = 0; i < 4; i++) {
        JoystickControl(i, 0x80);
      }

    }

    for (; i < 64; i++) {
      JoystickControl(i, 0);
    }
  }
  */
  

}