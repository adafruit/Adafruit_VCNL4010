/**************************************************************************/
/*! 
    @file     Adafruit_VCNL4010.h
    @author   K. Townsend (Adafruit Industries)
	@license  BSD (see license.txt)
	
	This is a library for the Adafruit VCNL4010 Temp Sensor breakout board
	----> http://www.adafruit.com/products/1782
	
	Adafruit invests time and resources providing this open source code, 
	please support Adafruit and open-source hardware by purchasing 
	products from Adafruit!

	@section  HISTORY

    v1.0  - First release
*/
/**************************************************************************/

#if ARDUINO >= 100
 #include "Arduino.h"
#else
 #include "WProgram.h"
#endif

#include <Wire.h>

// the i2c address
#define VCNL4010_I2CADDR_DEFAULT 0x13

// commands and constants
#define VCNL4010_COMMAND 0x80
#define VCNL4010_PRODUCTID 0x81
#define VCNL4010_PROXRATE 0x82
#define VCNL4010_IRLED 0x83
#define VCNL4010_AMBIENTPARAMETER 0x84
#define VCNL4010_AMBIENTDATA 0x85
#define VCNL4010_PROXIMITYDATA 0x87
#define VCNL4010_INTCONTROL 0x89
#define VCNL4010_PROXINITYADJUST 0x8A
#define VCNL4010_INTSTAT 0x8E
#define VCNL4010_MODTIMING 0x8F

typedef enum
  {
    VCNL4010_1_95    = 0,
    VCNL4010_3_90625 = 1,
    VCNL4010_7_8125  = 2,
    VCNL4010_16_625  = 3,
    VCNL4010_31_25   = 4,
    VCNL4010_62_5    = 5,
    VCNL4010_125     = 6,
    VCNL4010_250     = 7,
  } vcnl4010_freq;

#define VCNL4010_MEASUREAMBIENT 0x10
#define VCNL4010_MEASUREPROXIMITY 0x08
#define VCNL4010_AMBIENTREADY 0x40
#define VCNL4010_PROXIMITYREADY 0x20
  
class Adafruit_VCNL4010 {
 public:
  Adafruit_VCNL4010();
  boolean begin(uint8_t a = VCNL4010_I2CADDR_DEFAULT);  

  uint8_t getLEDcurrent(void);
  void setLEDcurrent(uint8_t c);

  void setFrequency(vcnl4010_freq f);
  uint16_t readProximity(void);
  uint16_t readAmbient(void);

 private:

  void write8(uint8_t address, uint8_t data);
  uint16_t read16(uint8_t address);
  uint8_t read8(uint8_t address);

  uint8_t _i2caddr;
};
