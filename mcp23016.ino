#include "mcp23016stepper.h"
#include "shiftregstepper.h"
#include <AccelStepper.h>

Mcp23016StepperControl expander(0x20);
ShiftregStepperControl shift_reg(10, 11, 12);
Mcp23016Stepper stp1(expander, 1);
ShiftregStepper stp2(shift_reg, 1);
Mcp23016Stepper stp3(expander, 3);
ShiftregStepper stp4(shift_reg, 0);

void fwd(Stepper& stp) {
  stp.forward(true);
  stp.speed(0);
  stp.steps(1);
}

void bwd(Stepper& stp) {
  stp.forward(false);
  stp.speed(0);
  stp.steps(1);
}

AccelStepper stepper1([](){fwd(stp1);}, [](){bwd(stp1);});
AccelStepper stepper2([](){fwd(stp2);}, [](){bwd(stp2);});


void setup() {
  Serial.begin(9600);
  expander.begin();
  shift_reg.begin();

  stepper1.setMaxSpeed(1000);
  stepper1.setSpeed(600);
  stepper1.setAcceleration(300.0);
  stepper1.moveTo(4096);

  stepper2.setMaxSpeed(1000);
  stepper2.setSpeed(200);
  stepper2.setAcceleration(100.0);
  stepper2.moveTo(10000000);

  stp4.turn(1);
  stp4.speed(10);

  stp3.speed(10);
  stp3.run();
  }


void loop() {
  LoopClock::tick();
  expander.tick();
  shift_reg.tick();

  // Change direction at the limits
  if (stepper1.distanceToGo() == 0) {
    if (stepper2.isRunning()) {
      stepper2.stop();
      stp3.run();
    }
    else {
      stepper2.move(10000000);
      stp3.stop();
    }
    if (!stp4.running()) {
      stepper1.moveTo(-stepper1.currentPosition());
      stp4.turn(0.5);
      stp4.reverse();
    }
  }
  stepper1.run();
  stepper2.run();
//  stepper3.run();
//  stepper4.run();
}
