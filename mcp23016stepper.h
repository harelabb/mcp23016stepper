#include "stepper.h"

class Mcp23016Stepper;

class Mcp23016StepperControl {
public:
  explicit Mcp23016StepperControl(int i2caddr);
  void begin() const;
  void moveStep(uint16_t step, byte unit);
  void tick();
  void addStepper(Stepper& stepper, byte unit);
private:
  int i2caddr_;
  uint16_t status_;
  uint16_t old_status_;
  Stepper* stepper_[4];
};


class Mcp23016Stepper : public Stepper {
public:
  explicit Mcp23016Stepper(Mcp23016StepperControl& expander, byte unit);

private:
  void doMove(byte step) override;
  Mcp23016StepperControl& expander;
  byte unit;
};
