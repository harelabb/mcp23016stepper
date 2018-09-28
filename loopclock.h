#ifndef LOOPCLOCK_H
#define LOOPCLOCK_H

#include <Arduino.h>

class LoopClock {
public:
  typedef decltype(::micros()) Time;

  LoopClock();
  static void tick();
  static Time now();
private:
  static Time now_;
};

#endif
