#include "Adafruit_VCNL4010.h"

/**************************************************************************/
/*!
    @brief  Instantiates a new VCNL4010 class
*/
/**************************************************************************/
Adafruit_VCNL4010::Adafruit_VCNL4010() {
}

/**************************************************************************/
/*!
    @brief  Setups the I2C connection and tests that the sensor was found. If so, configures for 200mA IR current and 390.625 KHz.
    @param addr Optional I2C address (however, all chips have the same address!)
    @return true if sensor was found, false if not
*/
/**************************************************************************/
boolean Adafruit_VCNL4010::begin(uint8_t addr) {
  _i2caddr = addr;
  Wire.begin();

  uint8_t rev = read8(VCNL4010_PRODUCTID);
  //Serial.println(rev, HEX);
  if ((rev & 0xF0) != 0x20) {
    return false;
  }

  setLEDcurrent(20);   // 200 mA
  setFrequency(VCNL4010_16_625); // 16.625 readings/second

  write8(VCNL4010_INTCONTROL, 0x08);
  return true;
}


/**************************************************************************/
/*!
    @brief  Set the LED current.
    @param  current_10mA  Can be any value from 0 to 20, each number represents 10 mA, so if you set it to 5, its 50mA. Minimum is 0 (0 mA, off), max is 20 (200mA)
*/
/**************************************************************************/

void Adafruit_VCNL4010::setLEDcurrent(uint8_t current_10mA) {
  if (current_10mA > 20) current_10mA = 20;
  write8(VCNL4010_IRLED, current_10mA);
}

/**************************************************************************/
/*!
    @brief  Get the LED current
    @return  The value directly from the register. Each bit represents 10mA so 5 == 50mA
*/
/**************************************************************************/

uint8_t Adafruit_VCNL4010::getLEDcurrent(void) {
  return read8(VCNL4010_IRLED);
}

/**************************************************************************/
/*!
    @brief  Set the measurement signal frequency
    @param  freq Sets the measurement rate for proximity. Can be VCNL4010_1_95 (1.95 measurements/s), VCNL4010_3_90625 (3.9062 meas/s), VCNL4010_7_8125 (7.8125 meas/s), VCNL4010_16_625 (16.625 meas/s), VCNL4010_31_25 (31.25 meas/s), VCNL4010_62_5 (62.5 meas/s), VCNL4010_125 (125 meas/s) or VCNL4010_250 (250 measurements/s)
*/
/**************************************************************************/

void Adafruit_VCNL4010::setFrequency(vcnl4010_freq freq) {
  write8(VCNL4010_MODTIMING, freq);
}


/**************************************************************************/
/*!
    @brief  Get proximity measurement
    @return Raw 16-bit reading value, will vary with LED current, unit-less!
*/
/**************************************************************************/

uint16_t  Adafruit_VCNL4010::readProximity(void) {
  uint8_t i = read8(VCNL4010_INTSTAT);
  i &= ~0x80;
  write8(VCNL4010_INTSTAT, i);

  write8(VCNL4010_COMMAND, VCNL4010_MEASUREPROXIMITY);
  while (1) {
    //Serial.println(read8(VCNL4010_INTSTAT), HEX);
    uint8_t result = read8(VCNL4010_COMMAND);
    //Serial.print("Ready = 0x"); Serial.println(result, HEX);
    if (result & VCNL4010_PROXIMITYREADY) {
      return read16(VCNL4010_PROXIMITYDATA);
    }
    delay(1);
  }
}

/**************************************************************************/
/*!
    @brief  Get ambient light measurement
    @return Raw 16-bit reading value, unit-less!
*/
/**************************************************************************/

uint16_t  Adafruit_VCNL4010::readAmbient(void) {
  uint8_t i = read8(VCNL4010_INTSTAT);
  i &= ~0x40;
  write8(VCNL4010_INTSTAT, i);


  write8(VCNL4010_COMMAND, VCNL4010_MEASUREAMBIENT);
  while (1) {
    //Serial.println(read8(VCNL4010_INTSTAT), HEX);
    uint8_t result = read8(VCNL4010_COMMAND);
    //Serial.print("Ready = 0x"); Serial.println(result, HEX);
    if (result & VCNL4010_AMBIENTREADY) {
      return read16(VCNL4010_AMBIENTDATA);
    }
    delay(1);
  }
}

/**************************************************************************/
/*!
    @brief  I2C low level interfacing
*/
/**************************************************************************/


// Read 1 byte from the VCNL4000 at 'address'
uint8_t Adafruit_VCNL4010::read8(uint8_t address)
{
  uint8_t data;

  Wire.beginTransmission(_i2caddr);
#if ARDUINO >= 100
  Wire.write(address);
#else
  Wire.send(address);
#endif
  Wire.endTransmission();

  delayMicroseconds(170);  // delay required

  Wire.requestFrom(_i2caddr, (uint8_t)1);

#if ARDUINO >= 100
  return Wire.read();
#else
  return Wire.receive();
#endif
}

void Adafruit_VCNL4010::write16(uint8_t address, uint16_t data){

    write8(address, data >> 8);
    write8(address+1, data & 0xFF );
}

// Read 2 byte from the VCNL4000 at 'address'
uint16_t Adafruit_VCNL4010::read16(uint8_t address)
{
  uint16_t data;

  Wire.beginTransmission(_i2caddr);
#if ARDUINO >= 100
  Wire.write(address);
#else
  Wire.send(address);
#endif
  Wire.endTransmission();

  Wire.requestFrom(_i2caddr, (uint8_t)2);
  while(!Wire.available());
#if ARDUINO >= 100
  data = Wire.read();
  data <<= 8;
  while(!Wire.available());
  data |= Wire.read();
#else
  data = Wire.receive();
  data <<= 8;
  while(!Wire.available());
  data |= Wire.receive();
#endif

  return data;
}

// write 1 byte
void Adafruit_VCNL4010::write8(uint8_t address, uint8_t data)
{
  Wire.beginTransmission(_i2caddr);
#if ARDUINO >= 100
  Wire.write(address);
  Wire.write(data);
#else
  Wire.send(address);
  Wire.send(data);
#endif
  Wire.endTransmission();
}

void Adafruit_VCNL4010::startHiLoInt(vcnl4010_int_t select, uint16_t low_threshold,
                    uint16_t high_threshold, vcnl4010_count_t count_exceeed){

  // Int count exceed | INT_THRES_EN | INT_THRES_SEL
  uint8_t int_cont =  count_exceeed | 0x02 | select;
  uint8_t cmd = 0x00;

  if(select == VCNL4010_INT_PROX){ cmd |= 0x03; }
  if(select == VCNL4010_INT_ALS) { cmd |= 0x05; }

  write8(VCNL4010_COMMAND, 0x00); // clear cmd register
  write16(VCNL4010_LOW_THRESHOLD, low_threshold); // set low threshold register
  write16(VCNL4010_HIGH_THRESHOLD, high_threshold); // set high threshold register
  write8(VCNL4010_INTCONTROL, int_cont); // set interrupt control register
  write8(VCNL4010_COMMAND, cmd);
}

void Adafruit_VCNL4010::startHiLoInt(vcnl4010_int_t select, uint16_t low_threshold,
                    uint16_t high_threshold){

  // default: 1 count
  startHiLoInt(select, low_threshold, high_threshold, VCNL4010_1_COUNT);

}

VCNL4010_Int Adafruit_VCNL4010::getInterrupt(){

     bool prox = false;
     bool als = false;
     bool low_thresh = false;
     bool high_thresh = false;
     vcnl4010_int_t type = VCNL4010_INT_PROX;

     uint8_t status = read8(VCNL4010_INTSTAT);

     if( (status & 0x08)  >> 3  ){prox = true;}
     if( (status & 0x04)  >> 2  ){
       als = true;
       type = VCNL4010_INT_ALS;
     }
     if( (status & 0x02)  >> 1  ){low_thresh = true;}
     if( (status & 0x01) ){ high_thresh = true; }

     VCNL4010_Int interrupt(low_thresh, high_thresh, type);

     return interrupt;
}

void Adafruit_VCNL4010::clearInterrupt(){
  uint8_t status = read8(VCNL4010_INTSTAT);
  write8(VCNL4010_INTSTAT, 0xF);
}
