#pragma config(I2C_Usage,	I2C1, 		i2cSensors)
#pragma config(Sensor, 		I2C_1,		,						sensorQuadEncoderOnI2CPort,    	, AutoAssign )
#pragma config(Sensor, 		I2C_2,		,						sensorQuadEncoderOnI2CPort,    	, AutoAssign )
#pragma config(Motor,  		port1,		lift_ML,		tmotorVex393HighSpeed_HBridge,	openLoop, reversed, driveLeft)
#pragma config(Motor, 	 	port2,		drive_FR,		tmotorVex393HighSpeed_MC29, 		openLoop, ,					driveRight)
#pragma config(Motor,  		port3,		drive_FL,		tmotorVex393HighSpeed_MC29,			openLoop, ,					driveLeft)
#pragma config(Motor,  		port4,		claw_R,			tmotorVex393_MC29,							openLoop, ,					driveRight)
#pragma config(Motor,  		port5,		lift_BL,		tmotorVex393HighSpeed_MC29,			openLoop, ,					driveLeft, 	encoderPort, I2C_1)
#pragma config(Motor,  		port6,		lift_BR,		tmotorVex393HighSpeed_MC29, 		openLoop, reversed, driveRight,	encoderPort, I2C_2)
#pragma config(Motor, 	 	port7,		claw_L,			tmotorVex393_MC29, 							openLoop, reversed, driveLeft)
#pragma config(Motor, 	 	port8,		drive_BR,		tmotorVex393HighSpeed_MC29, 		openLoop, reversed, driveRight)
#pragma config(Motor,  		port9,		drive_BL,		tmotorVex393HighSpeed_MC29, 		openLoop, reversed, driveLeft)
#pragma config(Motor,  		port10,		lift_MR,		tmotorVex393HighSpeed_HBridge, 	openLoop, ,					driveRight)

/*---------------------------------------------------------------------------*/
/*                                                                           */
/*        Description: Competition template for VEX EDR                      */
/*                                                                           */
/*---------------------------------------------------------------------------*/

// This code is for the VEX cortex platform
#pragma platform(VEX2)

// Select Download method as "competition"
#pragma competitionControl(Competition)

//Main competition background code...do not modify!
#include "Vex_Competition_Includes.c"

// User Defined Functions
void driveControl();
void liftControl();
void clawControl();

/* // no longer used
task initClaw() {
motor[claw_L] = -125;
motor[claw_R] = -125;
} // */

/*---------------------------------------------------------------------------*/
/*                          Pre-Autonomous Functions                         */
/*                                                                           */
/*  You may want to perform some actions before the competition starts.      */
/*  Do them in the following function.  You must return from this function   */
/*  or the autonomous and usercontrol tasks will not be started.  This       */
/*  function is only called once after the cortex has been powered on and    */
/*  not every time that the robot is disabled.                               */
/*---------------------------------------------------------------------------*/

void pre_auton() {
	bStopTasksBetweenModes = true;
	// startTask(initClaw);
}

/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                              Autonomous Task                              */
/*                                                                           */
/*  This task is used to control your robot during the autonomous phase of   */
/*  a VEX Competition.                                                       */
/*                                                                           */
/*  You must modify the code to add your own robot specific commands here.   */
/*---------------------------------------------------------------------------*/

task autonomous() {
	motor[lift_BL] = -45;
	motor[lift_BR] = -45;
	motor[lift_ML] = -45;
	motor[lift_MR] = -45;

	wait1Msec(400);

	motor[lift_BL] = 0;
	motor[lift_BR] = 0;
	motor[lift_ML] = 0;
	motor[lift_MR] = 0;

	wait1Msec(200);

	motor[claw_L] = -100;
	motor[claw_R] = -100;

	wait1Msec(1000);

	motor[claw_L] = -30;
	motor[claw_R] = -30;

	wait1Msec(500);

	motor[lift_BL] = -40;
	motor[lift_BR] = -40;
	motor[lift_ML] = -40;
	motor[lift_MR] = -40;

	wait1Msec(500);

	motor[drive_FR] =  -100;
	motor[drive_FL] =  100;
	motor[drive_BR] =  100;
	motor[drive_BL] =  -100;

	wait1Msec(2600);

	motor[drive_FR] =  0;
	motor[drive_FL] =  0;
	motor[drive_BR] =  0;
	motor[drive_BL] =  0;

	wait1Msec(500);

	motor[claw_L] = 40;
	motor[claw_R] = 40;

	wait1Msec(500);

	motor[claw_L] = -100;
	motor[claw_R] = -100;

	motor[drive_FR] =  100;
	motor[drive_FL] =  -100;
	motor[drive_BR] =  -100;
	motor[drive_BL] =  100;

	wait(500);

	motor[claw_L] = -25;
	motor[claw_R] = -25;

	motor[drive_FR] =  100;
	motor[drive_FL] =  -100;
	motor[drive_BR] =  100;
	motor[drive_BL] =  -100;

	wait1Msec(700);

	motor[drive_FR] =  0;
	motor[drive_FL] =  0;
	motor[drive_BR] =  0;
	motor[drive_BL] =  0;
}

/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                              User Control Task                            */
/*                                                                           */
/*  This task is used to control your robot during the user control phase of */
/*  a VEX Competition.                                                       */
/*                                                                           */
/*  You must modify the code to add your own robot specific commands here.   */
/*---------------------------------------------------------------------------*/

task usercontrol() {
	while (true) {
		liftControl();
		driveControl();
		clawControl();
	}
}

/// Begin User Defined Functions

// Drive Control, channel 1 rotates; channel 3 drives; channel 4 strafes
void driveControl() {
	motor[drive_FR] =  vexRT[Ch1] - vexRT[Ch3] + vexRT[Ch4];
	motor[drive_FL] =  vexRT[Ch1] + vexRT[Ch3] + vexRT[Ch4];
	motor[drive_BR] = -vexRT[Ch1] + vexRT[Ch3] + vexRT[Ch4];
	motor[drive_BL] = -vexRT[Ch1] - vexRT[Ch3] + vexRT[Ch4];
}

void liftControl() {
	if (vexRT[Btn6U]) {
		motor[lift_BL] = -65;
		motor[lift_ML] = -65;
		motor[lift_BR] = -65;
		motor[lift_MR] = -65;
	}
	else if (vexRT[Btn6D]) {
		motor[lift_BL] = 50;
		motor[lift_ML] = 50;
		motor[lift_BR] = 50;
		motor[lift_MR] = 50;
	}
	else {
		motor[lift_BL] = 0;
		motor[lift_ML] = 0;
		motor[lift_BR] = 0;
		motor[lift_MR] = 0;
	}
}

void clawControl() {
	if(vexRT[Btn5U]) {
		motor[claw_L] = -60;
		motor[claw_R] = -60;
	}
	else if (vexRT[Btn5D]) {
		motor[claw_L] = 60;
		motor[claw_R] = 60;
	}
	else {
		motor[claw_L] = 0;
		motor[claw_R] = 0;
	}
}
