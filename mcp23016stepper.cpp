#include "mcp23016stepper.h"
#include <Arduino.h>
#include <Wire.h>

Mcp23016StepperControl::
Mcp23016StepperControl(int i2caddr)
  : i2caddr_(i2caddr), status_(0), old_status_(0) {}

void
Mcp23016StepperControl::begin() const {
  Wire.begin();
  Wire.beginTransmission(i2caddr_);
  Wire.write(0x06); // IODIRA register
  Wire.write(0x00); // set all of port A to outputs
  Wire.write(0x00); // set all of port B to outputs
  Wire.endTransmission();
}

void
Mcp23016StepperControl::moveStep(uint16_t step, byte unit) {
  uint16_t unit_step = step << (4 * unit);
  uint16_t unit_mask = 0xf << (4 * unit);
  status_ = (status_ & ~unit_mask) | unit_step;
}

void
Mcp23016StepperControl::tick() {
  for (auto stepper : stepper_) {
    if (stepper) {
      stepper->move();
    }
  }
  if (status_ != old_status_) {
    old_status_ = status_;
    Wire.beginTransmission(i2caddr_);
    Wire.write(0x00); // GPIOA
    Wire.write(status_ & 0xFF);
    Wire.write(status_ >> 8);
    Wire.endTransmission();
  }
}

void
Mcp23016StepperControl::addStepper(Stepper& stepper, byte unit) {
  if (unit < 4 && !stepper_[unit]) {
    stepper_[unit] = &stepper;
  }
}


Mcp23016Stepper::Mcp23016Stepper(Mcp23016StepperControl& mcp23016, byte unt)
  : expander(mcp23016), unit(unt) {
  expander.addStepper(*this, unit);
}

void
Mcp23016Stepper::doMove(byte step) {
  expander.moveStep(step, unit);
}
