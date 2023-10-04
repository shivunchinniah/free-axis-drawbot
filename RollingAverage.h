#ifndef ROLLING_AVE_H
#define ROLLING_AVE_H

#include <stdint.h>

template<class T>
class RollingAverage {
public:

  RollingAverage(uint8_t len) {
    _buffer = new T[len];
    _n = len;
    _idx=0;

    for(unsigned int i = 0; i < len; i++){
      _buffer[i] = 0;
    }
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
    return _sum / (T)_n;
  }


private:
  T *_buffer;
  uint8_t _idx;
  uint8_t _n;
  T _sum = 0;

  void incrementIdx() {
    if (++_idx >= _n) _idx = 0;
  }
};


#endif