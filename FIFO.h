#ifndef FIFO_H
#define FIFO_H

#include <stdint.h>

template<class T>
class FIFO {
public:
  
  FIFO<T>(uint8_t len) {
    _buffer = new T[len];
    _n = len;
    _idx=0;

    for(uint8_t i = 0; i < len; i++){
      _buffer[_idx] = (T) 0;
      incrementIdx();
    }
  }

  ~FIFO(){
    delete[] _buffer;
  }

  void push(T item) {
    _buffer[_idx] = item;
    incrementIdx();
  }

  // First In is the next to go, so it sits at the index
  T firstIn(){
    return _buffer[_idx];
  }

  void clear(){

    _idx = 0;

    for(uint8_t i = 0; i < _n; i++){
      _buffer[_idx] = (T) 0;
      incrementIdx();
    }

  }




private:
  T *_buffer;
  uint8_t _idx;
  uint8_t _n;

  void incrementIdx() {
    if (++_idx >= _n) _idx = 0;
  }
};


#endif