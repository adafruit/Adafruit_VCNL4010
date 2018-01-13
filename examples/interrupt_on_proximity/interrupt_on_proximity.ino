#include <Arduino.h>
#include "Adafruit_VCNL4010.h"

// global variables
Adafruit_VCNL4010             vcnl;
const uint8_t                 interrupt_pin = 14;

void setup() {

  // serial
  Serial.begin(115200);

  // starting vcnl4010
  vcnl.begin();

  uint8_t count = 0;
  uint16_t avg_proximity = 0;

  // VCNL4010 10 proximity measurements each 100msec. Total time 1 sec
  Serial.println("Measuring proximity sensor average reading over 1 sec");
  while(count < 10){
    avg_proximity = (avg_proximity * count + vcnl.readProximity()) / (count + 1);
    count++;
    Serial.print(".");
    delay(100);
  }
  Serial.println();
  Serial.printf("Proximity sensor avg. reading is: %u\n", avg_proximity);

  // interrupt on proximity sensor, INT_PROX
  // low threshold set to 0.90 * average proximity measurement
  // high threshold set to 1.1 * average proximity measurement
  vcnl.startHiLoInt(VCNL4010_INT_PROX, 0.9* avg_proximity,
                                       avg_proximity*1.1) ;
  Serial.println("Interrupt on proximity sensor started");

  // VCNL4010 int pin is connected to pin #14
  // when proximity measurement > 1.1 * avg_proximity or < 0.9* avg_proximity
  // pin #14 will be connected to ground
  pinMode(interrupt_pin, INPUT_PULLUP);

}

void loop() {

  if (digitalRead(interrupt_pin) == LOW){

    // get interrupt info
    VCNL4010_Int interrupt = vcnl.getInterrupt();

    // print stuff to serial
    Serial.print("Interrupt on: ");
    if(interrupt.type() == VCNL4010_INT_PROX){
      Serial.println("Proximity");
    }
    if(interrupt.type() == VCNL4010_INT_ALS){
      Serial.println("Ambient light");
    }
    Serial.print("Raised by: ");
    if(interrupt.lowExceed()){
        Serial.println("Low threshold");
    }
    if(interrupt.highExceed()){
        Serial.println("High threshold");
    }

    // reset interrupt so it can be fired again
    vcnl.clearInterrupt();
  }
}
