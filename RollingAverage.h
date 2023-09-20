#ifndef ROLLING_AVE_H
#define ROLLING_AVE_H

#include <stdint.h>

template<class T>
class RollingAverage {
public:

  RollingAverage(uint8_t pow2) {
    _n = 1 << pow2;
    _pow2 = pow2;
    _buffer = new T[_n];
  }

  ~RollingAverage(){
    delete[] _buffer;
  }

  void push(T item) {
    _sum -= _buffer[_idx];
    _sum += item;
    _buffer[_idx] = item;
    incrementIdx();
  }

  T avg(){
    return _sum >> _pow2;
  }


private:
  T *_buffer;
  uint8_t _idx;
  uint8_t _n;
  uint8_t _pow2;
  T _sum = 0;

  void incrementIdx() {
    if (++_idx >= _n) _idx = 0;
  }
};


#endif