#ifndef ENCODER_EVERY_H
#define ENCODER_EVERY_H

#include "RollingAverage.h"

enum class EncoderISRChannel
{
    A,
    B,
    C,
    D
};

class EncoderEvery
{
public:
    
    unsigned long dt();
    unsigned long dtRaw();
    EncoderEvery(unsigned int triggerPin, unsigned int directionPin, char channel);
    long read();
    void write(long ticks);
    bool isReversed();
    void setReversed(bool reverse);
    bool forward();
    bool backward();

    static EncoderEvery *ISR_A;
    static EncoderEvery *ISR_B;
    static EncoderEvery *ISR_C;
    static EncoderEvery *ISR_D;

private:

      

    bool _forward;
    long _ticks;
    void _tick();
    void _tick_90();
    unsigned int _triggerPin;
    unsigned int _directionPin;
    bool _reversed;
    unsigned long _previous;
    RollingAverage<unsigned long> _dtavg = RollingAverage<unsigned long>(3);
    unsigned long _dt;

    static void ISRHandlerA();
    static void ISRHandlerB();
    static void ISRHandlerC();
    static void ISRHandlerD();

    static void ISRHandlerA_90();
    static void ISRHandlerB_90();
    static void ISRHandlerC_90();
    static void ISRHandlerD_90();
};

#endif
