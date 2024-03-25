#ifndef ENCODER_EVERY_H
#define ENCODER_EVERY_H

#include "RollingAverage.h"

#define BUFFER_SIZE 50

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

    EncoderEvery(unsigned int triggerPin, unsigned int directionPin, char channel, unsigned int poles);
    ~EncoderEvery(){ };
    long read();
    void write(long ticks);
    bool isReversed();
    void setReversed(bool reverse);
    
    // ---- Speed Measurement ----

    // Updates the current speed measurement, requries current timestamp and aproximate sample time
    // This function is called within a control loop whith assumed constant time sampling
    void updateSpeed(const long& ts); 
    
    float rps(); // speed in rotations per second
    unsigned long rpm(); // speed in rpm


    // ISR Pointers for 4 channels
    static EncoderEvery *ISR_A;
    static EncoderEvery *ISR_B;
    static EncoderEvery *ISR_C;
    static EncoderEvery *ISR_D;

private:

    unsigned int _tpr; // ticks per rotation

    // Speed measurement
    long _ts_ticks; // ticks in current speed measurement
   
    unsigned long _rpm; // rotations per minute


    // Position measurement
    bool _forward;
    long _ticks;


    // Interrupt handling
    void _tick();
    void _tick_90();

    // Pin Setup
    unsigned int _triggerPin;
    unsigned int _directionPin;
    bool _reversed;
    

    // calculate these as initialisation
    uint8_t _triggerPort;
    uint8_t _directionPort;
    uint8_t _bitMaskTriggerPin;
    uint8_t _bitMaskDirectionPin;

    // ISR Handlers for 4 channels, 0 degree and 90 degree 
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
