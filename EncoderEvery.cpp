#include "Arduino.h"
#include "EncoderEvery.h"

EncoderEvery *EncoderEvery::ISR_A = nullptr;
EncoderEvery *EncoderEvery::ISR_B = nullptr;
EncoderEvery *EncoderEvery::ISR_C = nullptr;
EncoderEvery *EncoderEvery::ISR_D = nullptr;

EncoderEvery::EncoderEvery(unsigned int triggerPin, unsigned int directionPin, char channel, unsigned int poles)
{

  // Initialise
  _ticks = 0;
  _ts_ticks = 0;
  _previous_time = micros(); // initialise to when object is created
  _previous_time_previous = micros();

  _tpr = poles;

  _dt_avg = new RollingAverage<unsigned long>(poles);


  _previous_sample_delta = 0;
  _triggerPin = triggerPin;
  _directionPin = directionPin;
  _forward = true;

  void (*ISRHandler)() = nullptr;
  void (*ISRHandler_90)() = nullptr;

  switch (channel)
  {
    case 'A':
    case 'a':
      ISRHandler = &ISRHandlerA;
      ISRHandler_90 = &ISRHandlerA_90;
      EncoderEvery::ISR_A = this;
      break;

    case 'B':
    case 'b':
      ISRHandler = &ISRHandlerB;
      ISRHandler_90 = &ISRHandlerB_90;
      EncoderEvery::ISR_B = this;
      break;

    case 'C':
    case 'c':
      ISRHandler = &ISRHandlerC;
      ISRHandler_90 = &ISRHandlerC_90;
      EncoderEvery::ISR_C = this;
      break;

    case 'D':
    case 'd':
      ISRHandler = &ISRHandlerD;
      ISRHandler_90 = &ISRHandlerD_90;
      EncoderEvery::ISR_D = this;
      break;

    default:
      break;
  }

 // Nano Every has Interupts on all pins
 attachInterrupt(digitalPinToInterrupt(_triggerPin), ISRHandler, CHANGE);
 attachInterrupt(digitalPinToInterrupt(_directionPin), ISRHandler_90, CHANGE);


}


bool EncoderEvery::forward(){
  return _forward;
}

bool EncoderEvery::backward(){
  return !_forward;
}


long EncoderEvery::read()
{

  return _ticks;
}

void EncoderEvery::write(long ticks)
{
  _ticks = ticks;
}

void EncoderEvery::_tick()
{

  bool trigger = digitalRead(_triggerPin);
  bool direction = digitalRead(_directionPin);

  // XOR to determine direction of rotation
  if (trigger ^ direction ^ _reversed)
  {
    // CW
    _ticks++;
    _forward = true;
  }
  else
  {
    // CCW
    _ticks--;
    _forward = false;
  }

  // update last tick time
  _previous_time_previous = _previous_time;
  _previous_time = micros();
  _dt_avg->push(_previous_time - _previous_time_previous);
  _ts_ticks++;
}

void EncoderEvery::_tick_90()
{

  bool trigger = digitalRead(_triggerPin);
  bool direction = digitalRead(_directionPin);

  // XOR to determine direction of rotation
  if (!trigger ^ direction ^ _reversed)
  {
    // CW
    _ticks++;
    _forward = true;
  }
  else
  {
    // CCW
    _ticks--;
    _forward = false;
  }

  // update last tick time
  _previous_time_previous = _previous_time;
  _previous_time = micros();
  _dt_avg->push(_previous_time - _previous_time_previous);
  _ts_ticks++;
}

bool EncoderEvery::isReversed()
{
  return _reversed;
}

void EncoderEvery::setReversed(bool reverse)
{

  _reversed = reverse;
}

void EncoderEvery::ISRHandlerA()
{

  EncoderEvery::ISR_A->_tick();
}

void EncoderEvery::ISRHandlerB()
{

  EncoderEvery::ISR_B->_tick();
}

void EncoderEvery::ISRHandlerC()
{

  EncoderEvery::ISR_C->_tick();
}

void EncoderEvery::ISRHandlerD()
{

  EncoderEvery::ISR_D->_tick();
}

void EncoderEvery::ISRHandlerA_90()
{

  EncoderEvery::ISR_A->_tick_90();
}

void EncoderEvery::ISRHandlerB_90()
{

  EncoderEvery::ISR_B->_tick_90();
}

void EncoderEvery::ISRHandlerC_90()
{

  EncoderEvery::ISR_C->_tick_90();
}

void EncoderEvery::ISRHandlerD_90()
{

  EncoderEvery::ISR_D->_tick_90();
}



//.----|    .     .     .    .    .    .    .---|
//  ^--- this is previous time delta           ^-- curent delta
void EncoderEvery::updateSpeed(unsigned long& now, const unsigned long& ts)
{
  // 60 mill useconds in 60 seconds
  //unsigned long sample_delta = now - _previous_time;
  unsigned long sample_delta = now - estimate_previous_time_using_dt_avg();
  _rpm = (_ts_ticks * 60000000ul) / ( (ts + _previous_sample_delta - sample_delta) * _tpr);
  
  _previous_sample_delta = sample_delta;
  
  _ts_ticks = 0;

}

unsigned long EncoderEvery::rpm()
{
  return _rpm;
}

float EncoderEvery::rps(){
  //return _rps;
  return (float) _rpm / 60.0f;
}

unsigned long EncoderEvery::estimate_previous_time_using_dt_avg(){

  unsigned long sum = 0;

  for(unsigned int idx = 0; idx < _tpr ; idx++){
    sum += _dt_avg->get(idx) * (idx +1);
  }

  return _previous_time - (sum / (_tpr+1)) + ( _dt_avg->avg() *  (_tpr / 2));

}


