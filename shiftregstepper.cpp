#include "shiftregstepper.h"
#include <Arduino.h>
#include <Wire.h>

ShiftregStepperControl::
ShiftregStepperControl(byte ser_pin, byte ltc_pin, byte clk_pin)
  : ser_pin_(ser_pin), ltc_pin_(ltc_pin), clk_pin_(clk_pin),
    status_(0), old_status_(0) {
  for (auto& stepper : stepper_) {
    stepper = nullptr;
  }
}

void
ShiftregStepperControl::begin() const {
  pinMode(clk_pin_, OUTPUT);
  pinMode(ser_pin_, OUTPUT);
  pinMode(ltc_pin_, OUTPUT);
}

void
ShiftregStepperControl::moveStep(byte step, byte unit) {
  int64_t unit_step = step << (4 * unit);
  int64_t unit_mask = 0xf << (4 * unit);
  status_ = (status_ & ~unit_mask) | unit_step;
}

void
ShiftregStepperControl::tick() {
  for (auto stepper : stepper_) {
    if (stepper) {
      stepper->move();
    }
  }
  if (status_ != old_status_) {
    old_status_ = status_;
    digitalWrite(clk_pin_, LOW);
    digitalWrite(ltc_pin_, LOW);
    int i = 8;
    while (i--) {
      shiftOut(ser_pin_, clk_pin_, MSBFIRST,
               (status_ & (0xff << i * 8)) >> (i * 8));
    }
    digitalWrite(ltc_pin_, HIGH);
  }
}

void
ShiftregStepperControl::addStepper(Stepper& stepper, byte unit) {
  if (unit < 16 && !stepper_[unit]) {
    stepper_[unit] = &stepper;
  }
}

ShiftregStepper::
ShiftregStepper(ShiftregStepperControl& shiftreg, byte unit)
  : shiftreg_(shiftreg), unit_(unit) {
  shiftreg_.addStepper(*this, unit_);
}

void
ShiftregStepper::doMove(byte step) {
  shiftreg_.moveStep(step, unit_);
}
