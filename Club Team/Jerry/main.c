#pragma config(I2C_Usage, I2C1, i2cSensors)
#pragma config(Sensor, I2C_1,  ,               sensorQuadEncoderOnI2CPort,    , AutoAssign )
#pragma config(Sensor, I2C_2,  ,               sensorQuadEncoderOnI2CPort,    , AutoAssign )
#pragma config(Sensor, I2C_3,  ,               sensorQuadEncoderOnI2CPort,    , AutoAssign )
#pragma config(Sensor, I2C_4,  ,               sensorQuadEncoderOnI2CPort,    , AutoAssign )
#pragma config(Sensor, I2C_5,  ,               sensorQuadEncoderOnI2CPort,    , AutoAssign )
#pragma config(Sensor, I2C_6,  ,               sensorQuadEncoderOnI2CPort,    , AutoAssign )
#pragma config(Motor,  port2,           BL,            tmotorVex393HighSpeed_MC29, openLoop, , driveLeft, encoderPort, I2C_1)
#pragma config(Motor,  port3,           BR,            tmotorVex393HighSpeed_MC29, openLoop, reversed, driveRight, encoderPort, I2C_3)
#pragma config(Motor,  port4,           FL,            tmotorVex393HighSpeed_MC29, openLoop, , driveLeft, encoderPort, I2C_2)
#pragma config(Motor,  port5,           WINCH,         tmotorVex393HighSpeed_MC29, openLoop, encoderPort, I2C_6)
#pragma config(Motor,  port6,           WINCH2,        tmotorVex393HighSpeed_MC29, openLoop)
#pragma config(Motor,  port7,           CLUTCH,        tmotorVex393_MC29, openLoop, encoderPort, I2C_5)
#pragma config(Motor,  port8,           LOCK,          tmotorServoStandard, openLoop)
#pragma config(Motor,  port9,           FR,            tmotorVex393HighSpeed_MC29, openLoop, reversed, driveRight, encoderPort, I2C_4)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

#include "utility.c"

// Robot is 24 rubber bands on each side
// Competition is on Nov 19th

word winch_control_current;
word winch_control_previous;

word launch_control_current;
word launch_control_previous;

task main()
{
	init();

	int X2 = 0, Y1 = 0, X1 = 0, threshold = 15;
	int max_voltage = 8400;
	int min_voltage = 7000;
	int delta = max_voltage - min_voltage;
	int current_voltage = nImmediateBatteryLevel;
	float current_delta = max_voltage - current_voltage;
	float percentage = 1 - current_delta/delta;
	percentage *= 100;

	percentage= 9001;

	//writeDebugStream("Hello World\n");


	while(true)
	{
		winch_control_current = vexRT[Btn6U];
		launch_control_current = vexRT[Btn6D];

		if (vexRT[Btn8U]){
			 autonomous();
			// moveForwardTicks(1000);
			//speedVoltageCalibrationTest();
		}

		if (vexRT[Btn7U])
		{
			engageLock();
			//motor[FL] = 30;
		}
		else if (vexRT[Btn7D])
		{
			disengageLock();
			//motor[BR] = 30;
		}
		else if (vexRT[Btn7R])
		{
			engageClutch();
			//motor[FR] = 30;
		}
		else if (vexRT[Btn7L])
		{
			disengageClutch();
			//motor[BL] = 30;
		}
		else
		{
			motor[FL] = 0;
			motor[BR] = 0;
			motor[FR] = 0;
			motor[BL] = 0;
		}


		// Winch Edge Control
		if (winch_control_current && !winch_control_previous) // Button Pressed
		{
			engageClutch();
			disengageLock();
		}
		else if (winch_control_current && winch_control_previous) // Button Held
		{
			motor[WINCH] = 127;
		}
		else if (!winch_control_current && winch_control_previous) // Button Released
		{
			motor[WINCH] = 0;
			engageLock();
			// disengageClutch();
		}



		// Launch
		if (launch_control_current && !launch_control_previous)
		{
			if (!launching)
			{
				startTask(launch);
			}

		}


		// Raise slightly
		if (vexRT[Btn8D])
		{
			raise();
		}

		// Run Jame's code
		if (vexRT[Btn8L])
		{
			zains_a_bitch_cunt();
		}


		// Ch3: Y1
		if (abs(vexRT[Ch3]) > threshold)
		{
			Y1 = vexRT[Ch3];
		}
		else
		{
			Y1 = 0;
		}

		// Ch4: X1
		if (abs(vexRT[Ch4]) > threshold)
		{
			X1 = vexRT[Ch4];
		}
		else
		{
			X1 = 0;
		}

		// Ch1: X2
		if (abs(vexRT[Ch1]) > threshold)
		{
			X2 = vexRT[Ch1];
		}
		else
		{
			X2 = 0;
		}

		if (vexRT[Btn5D])
		{
			if (vexRT[Btn7R])
			{
				// rotate(90);
			}
			else if (vexRT[Btn7L])
			{
				// rotate(-90);
			}

			Y1 /= 5;
			X1 /= 2;
			X2 /= 4;
		}

		motor[FL] = Y1 + X1 + X2;
		motor[FR] =  Y1 - X1 - X2;
		motor[BR] =  Y1 + X1 - X2;
		motor[BL] = (Y1 - X1 + X2)*1.2;

		winch_control_previous = winch_control_current;
		launch_control_previous = launch_control_current;

		wait1Msec(10);
	}
}