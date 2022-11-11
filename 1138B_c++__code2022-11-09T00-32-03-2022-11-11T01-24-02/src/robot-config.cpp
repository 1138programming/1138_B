#include "vex.h"

using namespace vex;
using signature = vision::signature;
using code = vision::code;

// A global instance of brain used for printing to the V5 Brain screen
brain  Brain;

// VEXcode device constructors
motor leftMotorA = motor(PORT9, ratio18_1, true);
motor leftMotorB = motor(PORT12, ratio18_1, true);
motor_group LeftDriveSmart = motor_group(leftMotorA, leftMotorB);
motor rightMotorA = motor(PORT10, ratio18_1, false);
motor rightMotorB = motor(PORT11, ratio18_1, false);
motor_group RightDriveSmart = motor_group(rightMotorA, rightMotorB);
drivetrain Drivetrain = drivetrain(LeftDriveSmart, RightDriveSmart, 319.19, 295, 40, mm, 1);
motor intakestorage = motor(PORT1, ratio18_1, true);
motor flywheelMotorA = motor(PORT3, ratio18_1, false);
motor flywheelMotorB = motor(PORT4, ratio18_1, false);
motor_group flywheel = motor_group(flywheelMotorA, flywheelMotorB);
controller Controller1 = controller(primary);
controller Controller2 = controller(partner);
/*vex-vision-config:begin*/
vision Vision8 = vision (PORT8, 50);
/*vex-vision-config:end*/
digital_out endgame = digital_out(Brain.ThreeWirePort.A);
digital_out shooter = digital_out(Brain.ThreeWirePort.H);
motor Roller = motor(PORT2, ratio18_1, false);

// VEXcode generated functions
// define variable for remote controller enable/disable
bool RemoteControlCodeEnabled = true;
// define variables used for controlling motors based on controller inputs
bool Controller1LeftShoulderControlMotorsStopped = true;
bool DrivetrainLNeedsToBeStopped_Controller1 = true;
bool DrivetrainRNeedsToBeStopped_Controller1 = true;

// define a task that will handle monitoring inputs from Controller1
int rc_auto_loop_function_Controller1() {
  // process the controller input every 20 milliseconds
  // update the motors based on the input values
  while(true) {
    if(RemoteControlCodeEnabled) {
      // calculate the drivetrain motor velocities from the controller joystick axies
      // left = Axis3 + Axis1
      // right = Axis3 - Axis1
      int drivetrainLeftSideSpeed = -Controller1.Axis3.position() + Controller1.Axis1.position();
      int drivetrainRightSideSpeed = -Controller1.Axis3.position() - Controller1.Axis1.position();
      
      // check if the value is inside of the deadband range
      if (drivetrainLeftSideSpeed < 5 && drivetrainLeftSideSpeed > -5) {
        // check if the left motor has already been stopped
        if (DrivetrainLNeedsToBeStopped_Controller1) {
          // stop the left drive motor
          LeftDriveSmart.stop();
          // tell the code that the left motor has been stopped
          DrivetrainLNeedsToBeStopped_Controller1 = false;
        }
      } else {
        // reset the toggle so that the deadband code knows to stop the left motor nexttime the input is in the deadband range
        DrivetrainLNeedsToBeStopped_Controller1 = true;
      }
      // check if the value is inside of the deadband range
      if (drivetrainRightSideSpeed < 5 && drivetrainRightSideSpeed > -5) {
        // check if the right motor has already been stopped
        if (DrivetrainRNeedsToBeStopped_Controller1) {
          // stop the right drive motor
          RightDriveSmart.stop();
          // tell the code that the right motor has been stopped
          DrivetrainRNeedsToBeStopped_Controller1 = false;
        }
      } else {
        // reset the toggle so that the deadband code knows to stop the right motor next time the input is in the deadband range
        DrivetrainRNeedsToBeStopped_Controller1 = true;
      }
      
      // only tell the left drive motor to spin if the values are not in the deadband range
      if (DrivetrainLNeedsToBeStopped_Controller1) {
        LeftDriveSmart.setVelocity(drivetrainLeftSideSpeed, percent);
        LeftDriveSmart.spin(forward);
      }
      // only tell the right drive motor to spin if the values are not in the deadband range
      if (DrivetrainRNeedsToBeStopped_Controller1) {
        RightDriveSmart.setVelocity(drivetrainRightSideSpeed, percent);
        RightDriveSmart.spin(forward);
      }
      // check the ButtonL1/ButtonL2 status to control intakestorage
      if (Controller1.ButtonL1.pressing()) {
        intakestorage.spin(forward);
        Controller1LeftShoulderControlMotorsStopped = false;
      } else if (Controller1.ButtonL2.pressing()) {
        intakestorage.spin(reverse);
        Controller1LeftShoulderControlMotorsStopped = false;
      } else if (!Controller1LeftShoulderControlMotorsStopped) {
        intakestorage.stop();
        // set the toggle so that we don't constantly tell the motor to stop when the buttons are released
        Controller1LeftShoulderControlMotorsStopped = true;
      }
    }
    //flywheel on button r1
    if (Controller2.ButtonR1.pressing()) {
      flywheel.setVelocity(60,percent);
      flywheel.spin(forward);
    }
    else {
      flywheel.stop();
    }
    // wait before repeating the process
    wait(20, msec);
  }
  return 0;
}

// define variables used for controlling motors based on controller inputs
bool Controller2LeftShoulderControlMotorsStopped = true;

// define a task that will handle monitoring inputs from Controller2
int rc_auto_loop_function_Controller2() {
  // process the controller input every 20 milliseconds
  // update the motors based on the input values
  while(true) {
    if(RemoteControlCodeEnabled) {
      // check the ButtonL1/ButtonL2 status to control Roller
      if (Controller2.ButtonL1.pressing()) {
        Roller.spin(forward);
        Controller2LeftShoulderControlMotorsStopped = false;
      } else if (Controller2.ButtonL2.pressing()) {
        Roller.spin(reverse);
        Controller2LeftShoulderControlMotorsStopped = false;
      } else if (!Controller2LeftShoulderControlMotorsStopped) {
        Roller.stop();
        // set the toggle so that we don't constantly tell the motor to stop when the buttons are released
        Controller2LeftShoulderControlMotorsStopped = true;
      }
    }
    // wait before repeating the process
    wait(20, msec);
  }
  return 0;
}


/**
 * Used to initialize code/tasks/devices added using tools in VEXcode Pro.
 * 
 * This should be called at the start of your int main function.
 */
void vexcodeInit( void ) {
  task rc_auto_loop_task_Controller1(rc_auto_loop_function_Controller1);
  task rc_auto_loop_task_Controller2(rc_auto_loop_function_Controller2);
}