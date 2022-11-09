using namespace vex;

extern brain Brain;

using signature = vision::signature;

// VEXcode devices
extern drivetrain Drivetrain;
extern motor intakestorage;
extern motor_group flywheel;
extern controller Controller1;
extern controller Controller2;
extern signature Vision8__SIG_1;
extern signature Vision8__SIG_2;
extern signature Vision8__SIG_3;
extern signature Vision8__SIG_4;
extern signature Vision8__SIG_5;
extern signature Vision8__SIG_6;
extern signature Vision8__SIG_7;
extern vision Vision8;
extern digital_out endgame;
extern digital_out shooter;
extern motor Roller;

/**
 * Used to initialize code/tasks/devices added using tools in VEXcode Pro.
 * 
 * This should be called at the start of your int main function.
 */
void  vexcodeInit( void );