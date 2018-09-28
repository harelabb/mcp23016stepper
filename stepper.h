#ifndef STEPPER_H
#define STEPPER_H

#include <Arduino.h>
#include "loopclock.h"

class Stepper {
public:
  Stepper();
  void forward(bool fwd);
  bool forward() const;
  void reverse();
  void speed(float rpm);
  void steps(uint32_t stp);
  uint32_t steps() const;
  void turn(float turns);
  void stop();
  void move();
  bool running() const;
  void run();

  // Arduino Stepper compatibility
  void step(int steps);
  void setSpeed(long rpm);

  bool half_step = false;
  uint32_t steps_pr_turn = 2048;

private:
  static constexpr int32_t fsteps_ =
    0b1001001101101100;

  static constexpr int32_t hsteps_ =
    0b10010001001100100110010011001000;

  byte current_ = 0;
  bool forward_ = true;
  bool reverse_ = false;
  bool continuos_ = false;

  byte hstep();
  byte fstep();
  LoopClock::Time now() const;
  void setClock();

  virtual void doMove(byte step) = 0;

  uint32_t micros_pr_step_ = 0;
  uint32_t remaining_ = 0;
  const LoopClock& loop_clock_;
  LoopClock::Time clock_;
};
#endif
