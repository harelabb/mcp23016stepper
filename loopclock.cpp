#include "loopclock.h"
#include <Arduino.h>

void LoopClock::tick() {
  now_ = ::micros();
}

LoopClock::Time LoopClock::now() {
  return now_;
}

LoopClock::Time LoopClock::now_ = ::micros();
