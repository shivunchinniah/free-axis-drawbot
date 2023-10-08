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
    

    /* Ticks Per Second */
    unsigned long tps_period_method();
    unsigned long tps_frequency_method();

    unsigned long dt();
    unsigned long dtRaw();
    EncoderEvery(unsigned int triggerPin, unsigned int directionPin, char channel, unsigned int poles);
    ~EncoderEvery(){ delete _dtavg; };
    long read();
    void write(long ticks);
    bool isReversed();
    void setReversed(bool reverse);
    bool forward();
    bool backward();

    void update_tps(unsigned long now);
    
    //CircularBuffer<unsigned long, BUFFER_SIZE>& getEdgeHistory();
    unsigned long history[BUFFER_SIZE];
    unsigned int history_idx = 0;

    static EncoderEvery *ISR_A;
    static EncoderEvery *ISR_B;
    static EncoderEvery *ISR_C;
    static EncoderEvery *ISR_D;

private:

      
   // CircularBuffer<unsigned long, BUFFER_SIZE> _edgeHistory; // Store the edge history (micros() intervals) in a FIFO buffer.
    bool _forward;
    long _ticks;
    void _tick();
    void _tick_90();
    unsigned int _triggerPin;
    unsigned int _directionPin;
    bool _reversed;
    unsigned long _previous;
    RollingAverage<unsigned long>* _dtavg;
    unsigned long _dt;

    unsigned long _previous_time;

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
