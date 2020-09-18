/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       main.cpp                                                  */
/*    Author:       VEX                                                       */
/*    Created:      Wed Sep 25 2019                                           */
/*    Description:                                                            */
/*    A robot to stack ball on towers                                         */
/*    Use axis 3 4 to controll the movement of the robot                      */
/*    r2 to increase multiplier, r1 to decrease it                            */
/*    is_digital=true enables B button pressing and roll,                     */
/*                                                                            */
/*    false means using axis 1 2.                                             */
/*    Name:                                                                   */
/*    Date                                                                    */
/*    Class:                                                                  */
/*                                                                            */
/*----------------------------------------------------------------------------*/

// ---- START VEXCODE CONFIGURED DEVICES ----
// Robot Configuration:
// [Name]               [Type]        [Port(s)]
// Controller1          controller
// leftmotor            motor         1
// rightmotor           motor         7
// roller               motor         15
// ---- END VEXCODE CONFIGURED DEVICES ----

#include "vex.h"

using namespace vex;

//Global variables
double leftpower, rightpower;
double multiplier = 0.5;
bool is_digital = true; // bool status for roller

class driver {
public:
  void move() {
    rightpower = (Controller1.Axis3.value() + (Controller1.Axis4.value() * 2)) *
                 multiplier / 2;
    leftpower = (Controller1.Axis3.value() - (Controller1.Axis4.value() * 2)) *
                multiplier / 2;
    leftmotor.spin(vex::directionType::fwd, leftpower, vex::velocityUnits::pct);
    rightmotor.spin(vex::directionType::fwd, leftpower,
                    vex::velocityUnits::pct);
  }
  void speedsetup() {
    if (Controller1.ButtonR2.pressing()) {
      if (multiplier < 1) {
        multiplier += 0.1;
      } else {
        Controller1.rumble(rumbleShort);
      }

    } else {
      if (Controller1.ButtonR1.pressing()) {
        if (multiplier > 0) {
          multiplier -= 0.1;
        } else {
          Controller1.rumble(rumbleShort);
        }
      }
    }
  }
};

void display_status() {
  Controller1.Screen.clearScreen();
  Controller1.Screen.print("Mul: ");
  Controller1.Screen.print(multiplier);
  Controller1.Screen.newLine();
}

void roll_digital() {
  if (Controller1.ButtonB.pressing()) {
    roller.spin(vex::directionType::fwd, 0.6, vex::velocityUnits::pct);
  }
}

void roll_analog() {
  roller.spin(vex::directionType::fwd, Controller1.Axis2.value() * 0.1,
              vex::velocityUnits::pct);
}

void set_breaking() {
  leftmotor.setStopping(hold);
  rightmotor.setStopping(hold);
  roller.setStopping(hold);
}

void breaking() {
  if (leftpower == 0 && rightpower == 0) {
    leftmotor.stop();
    rightmotor.stop();
  }
  if (is_digital) {
    if (!Controller1.ButtonB.pressing()) {
      roller.stop();
    }
  } else {
    if (Controller1.Axis2.value() == 0) {
      roller.stop();
    }
  }
}

driver Driver;
int main() {
  // Initializing Robot Configuration. DO NOT REMOVE!
  vexcodeInit();
  Driver.speedsetup();
  Driver.move();
  breaking();
  if (is_digital) {
    roll_digital();
  } else {
    roll_analog();
  }
  display_status();
  vex::task::sleep(50);
}
