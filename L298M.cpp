#include "L298M.h"
#include <Arduino.h>

L298M::L298M(unsigned int cw, unsigned int ccw)
{
    CW_pin = cw;
    CCW_pin = ccw;
    _hold = false;
    _vector = 100;

    // set pin to output
    pinMode(CW_pin, OUTPUT);
    pinMode(CCW_pin, OUTPUT);
}

void L298M::stop()
{
    if (_hold)
    {
        analogWrite(CCW_pin, 255);
        analogWrite(CW_pin, 255);
    }
    else
    {
        analogWrite(CCW_pin, 0);
        analogWrite(CW_pin, 0);
    }
}

void L298M::run()
{
    if (_vector >= 0)
    {
        analogWrite(CCW_pin, 0);
        analogWrite(CW_pin, _vector);
    }
    else
    {
        analogWrite(CW_pin, 0);
        analogWrite(CCW_pin, -_vector);
    }
}

void L298M::setHold(bool hold)
{
    _hold = hold;
}

void L298M::setVector(int16_t vector)
{
    if (vector < -255)
    {
        _vector = -255;
    }
    else if (vector > 255)
    {
        _vector = 255;
    }
    else
    {
        _vector = vector;
    }
}

int16_t L298M::getVector()
{
    return _vector;
}

bool L298M::getHold()
{
    return _hold;
}
