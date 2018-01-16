#ifndef VCNL4010_INT_H_
#define VCNL4010_INT_H_

#include <Arduino.h>

typedef enum
{

  VCNL4010_INT_PROX = 0,
  VCNL4010_INT_ALS = 1

} vcnl4010_int_t;

typedef enum 
{

  VCNL4010_1_COUNT      = 0, 
  VCNL4010_2_COUNT      = 32,
  VCNL4010_4_COUNT      = 64,
  VCNL4010_8_COUNT      = 96,
  VCNL4010_16_COUNT     = 128,
  VCNL4010_32_COUNT     = 160,
  VCNL4010_64_COUNT     = 192,
  VCNL4010_128_COUNT    = 224

} vcnl4010_count_t;

class VCNL4010_Int {

    public:

    ~VCNL4010_Int();
    VCNL4010_Int();
    VCNL4010_Int(bool lowExceed, bool highExceed, vcnl4010_int_t type);

    bool                    lowExceed();
    bool                    highExceed();
    vcnl4010_int_t          type();

    private:

    bool                    _lowExceed;
    bool                    _highExceed;
    vcnl4010_int_t          _type;
    uint16_t                _value;

};



#endif
