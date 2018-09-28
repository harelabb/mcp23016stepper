#include "stepper.h"

Stepper::Stepper()  {
  clock_ = now();
}

void Stepper::forward(bool fwd) {
  forward_ = fwd;
}

bool Stepper::forward() const {
  return forward_;
}

void Stepper::reverse() {
  reverse_ = true;
}

void Stepper::speed(float rpm) {
  if (rpm > 0.0f) {
    micros_pr_step_ = 60.0e+6f / (rpm * steps_pr_turn);
    Serial.println(micros_pr_step_, DEC);
  }
  else {
    micros_pr_step_ = 0;
  }
  clock_ = now();
}

void Stepper::steps(uint32_t stp) {
  continuos_ = false;
  remaining_ = stp;
}

uint32_t Stepper::steps() const {
  return remaining_;
}

void Stepper::move() {
  if (remaining_ > 0) {
    if (now() - clock_ >= micros_pr_step_) {
      clock_ = now();
      if (reverse_) {
          forward_ = !forward_;
          reverse_ = false;
      }
      doMove(half_step ? hstep() : fstep());
      if (!continuos_) {
        --remaining_;
      }
    }
  }
  else if (micros_pr_step_ != 0) {
    doMove(0);
    clock_ = now();
  }
}

byte Stepper::hstep() {
  current_ = (current_ + 8 + 1 - 2 * int(forward_)) % 8;
  return (hsteps_ >> (4 * current_)) & 0xf;
}

byte Stepper::fstep() {
  current_ = (current_ + 4 + 1 - 2 * int(forward_)) % 4;
  return (fsteps_ >> (4 * current_)) & 0xf;
}

bool Stepper::running() const {
  return remaining_ > 0;
}

void Stepper::turn(float turns) {
  if (turns > 0) {
    steps(uint32_t(float(steps_pr_turn) * turns));
    if (half_step) {
      remaining_ *= 2;
    }
  }
  else {
    steps(0);
  }
}

void Stepper::run() {
  remaining_ = 1;
  continuos_ = true;
}

void Stepper::stop() {
  remaining_ = 0;
}

LoopClock::Time Stepper::now() const {
  return LoopClock::now();
}


// Arduino Stepper compatibility
void Stepper::step(int s) {
  if (s > 0) {
    forward(true);
    steps(s);
  }
  else {
    forward(false);
    steps(-s);
  }
}

void Stepper::setSpeed(long rpm) {
  if (rpm < 0) {
    forward(false);
    speed(-rpm);
  }
  else {
    forward(true);
    speed(rpm);
  }
}
