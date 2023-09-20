#ifndef L298M_H
#define L298M_H

#include <stdint.h>

class L298M
{

private:
    unsigned int CW_pin;
    unsigned int CCW_pin;

    int16_t _vector; // -255 to 255
    bool _hold; // when _vector is 0 should position be held
    

public:
    L298M(unsigned int cw, unsigned int ccw);
    void stop();
    void run();
    void setVector(int16_t vector);
    int16_t getVector();
    void setHold(bool hold);
    bool getHold();
};

#endif
