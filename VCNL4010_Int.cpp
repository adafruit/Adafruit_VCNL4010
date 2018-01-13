#include "VCNL4010_Int.h"

VCNL4010_Int::~VCNL4010_Int(){

}

VCNL4010_Int::VCNL4010_Int(){

}

VCNL4010_Int::VCNL4010_Int(bool lowExceed, bool highExceed,
                           vcnl4010_int_t type){
  _lowExceed = lowExceed;
  _highExceed = highExceed;
  _type = type;
}

bool VCNL4010_Int::lowExceed(){return _lowExceed;}
bool VCNL4010_Int::highExceed(){return _highExceed;}
vcnl4010_int_t VCNL4010_Int::type(){return _type;}
