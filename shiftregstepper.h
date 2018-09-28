#ifndef SHIFTREGSTEPPER_H
#define SHIFTREGSTEPPER_H

#include "stepper.h"

class ShiftregStepper;

class ShiftregStepperControl {
public:
  ShiftregStepperControl(byte ser_pin, byte ltc_pin, byte clk_pin);
  void begin() const;
  void moveStep(byte step, byte unit);
  void tick();
  void addStepper(Stepper& stepper, byte unit);
private:
  byte ser_pin_;
  byte ltc_pin_;
  byte clk_pin_;
  int64_t status_;
  int64_t old_status_;
  Stepper* stepper_[16];
};


class ShiftregStepper : public Stepper {
public:
  explicit ShiftregStepper(ShiftregStepperControl& shiftreg, byte unit);

private:
  void doMove(byte step) override;
  ShiftregStepperControl& shiftreg_;
  byte unit_;
};
#endif
