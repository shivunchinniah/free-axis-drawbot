#include "EncoderEvery.h"
#include "Arduino.h"

EncoderEvery *EncoderEvery::ISR_A = nullptr;
EncoderEvery *EncoderEvery::ISR_B = nullptr;
EncoderEvery *EncoderEvery::ISR_C = nullptr;
EncoderEvery *EncoderEvery::ISR_D = nullptr;

EncoderEvery::EncoderEvery(unsigned int triggerPin, unsigned int directionPin, char channel, unsigned int poles)
{

  // Initialise
  _ticks = 0;
  _previous = micros(); // initialise to when object is created

  _dtavg = new RollingAverage<unsigned long>(poles);

  _triggerPin = triggerPin;
  _directionPin = directionPin;
  _forward = true;
  _previous = micros();

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

unsigned long EncoderEvery::dt(){
   return _dtavg->avg();
}

unsigned long EncoderEvery::dtRaw(){
  return _dt;
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

  // update dt
  unsigned long now = micros();
  _dt = now - _previous;
  _previous = now;
  _dtavg->push(_dt);

  // push timestamp to edge buffer
  if(history_idx < BUFFER_SIZE)
    history[history_idx++] = _dtavg->avg();
     

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

  // update dt
  unsigned long now = micros();
  _dt = now - _previous;
  _previous = now;
  _dtavg->push(_dt); 

  // push timestamp to edge buffer
  if(history_idx < BUFFER_SIZE)
    history[history_idx++] = _dtavg->avg();
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


