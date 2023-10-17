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
    ~EncoderEvery(){ delete _dt_avg; delete _ts_avg; };
    long read();
    void write(long ticks);
    bool isReversed();
    void setReversed(bool reverse);
    bool forward();
    bool backward();

    void update_tps(unsigned long now);
    
    // ---- Speed Measurement ----

    // Updates the current speed measurement, requries current timestamp and aproximate sample time
    // This function is called within a control loop whith assumed constant time sampling
    void updateSpeed(unsigned long& now, const unsigned long& ts); 
    
    float rps(); // speed in rotations per second
    unsigned long rpm(); // speed in rpm

    unsigned long rpm_dt(); // rpm using dt purely



    // ISR Pointers for 4 channels
    static EncoderEvery *ISR_A;
    static EncoderEvery *ISR_B;
    static EncoderEvery *ISR_C;
    static EncoderEvery *ISR_D;

private:

    unsigned long _threshold_rpm;
    unsigned long _tpr_over_2;
    unsigned int _tpr; // ticks per rotation

    // Speed measurement
    //unsigned long _ts; // sample time for speed measurement 
    unsigned long _ts_ticks; // ticks in current speed measurement
    unsigned long _blank_intervals;
    unsigned long _previous_time; // timestamp of last tick
    unsigned long _previous_time_previous; // timestamp of tick before last
    unsigned long _previous_sample_delta; // the timestamp of the last tick in the previous



    unsigned long _rps; // Rotations Per Second
    unsigned long _rpm; // rotations per minute

    RollingAverage<unsigned long>* _dt_avg;
    RollingAverage<unsigned long>* _ts_avg;
    
    unsigned long estimate_previous_time_using_dt_avg(); 


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
