#pragma config(I2C_Usage, I2C1, i2cSensors)
#pragma config(Sensor, I2C_1,  ,               sensorQuadEncoderOnI2CPort,    , AutoAssign )
#pragma config(Sensor, I2C_2,  ,               sensorQuadEncoderOnI2CPort,    , AutoAssign )
#pragma config(Sensor, I2C_3,  ,               sensorQuadEncoderOnI2CPort,    , AutoAssign )
#pragma config(Sensor, I2C_4,  ,               sensorQuadEncoderOnI2CPort,    , AutoAssign )
#pragma config(Sensor, I2C_5,  ,               sensorQuadEncoderOnI2CPort,    , AutoAssign )
#pragma config(Sensor, I2C_6,  ,               sensorQuadEncoderOnI2CPort,    , AutoAssign )
#pragma config(Motor,  port2,           BL,            tmotorVex393_MC29, openLoop, encoderPort, I2C_4)
#pragma config(Motor,  port3,           BR,            tmotorVex393_MC29, openLoop, reversed, encoderPort, I2C_1)
#pragma config(Motor,  port4,           FL,            tmotorVex393_MC29, openLoop, encoderPort, I2C_3)
#pragma config(Motor,  port5,           FR,            tmotorVex393_MC29, openLoop, reversed, encoderPort, I2C_2)
#pragma config(Motor,  port6,           ARM,           tmotorVex393_MC29, openLoop, reversed, encoderPort, I2C_5)
#pragma config(Motor,  port7,           ARMSLAVE,      tmotorVex393_MC29, openLoop, reversed)
#pragma config(Motor,  port8,           ARMSLAVE2,     tmotorVex393_MC29, openLoop, reversed)
#pragma config(Motor,  port9,           CLAW,          tmotorVex393_MC29, openLoop, encoderPort, I2C_6)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

#include "utility.c"

// Used for printing to debug console.
char buffer[64];

int mode = 0;

task main()
{
	init();

	int threshold = 10;
	int X1 = 0;
	int Y1 = 0;
	int X2 = 0;

	// This will output to the console in debug mode
	writeDebugStreamLine("Program Start");

	while(true)
	{
		word open_claw = vexRT[Btn6U]; // R1
		word close_claw = vexRT[Btn6D]; // R2

		word raise_arm = vexRT[Btn5U]; // L1
		word lower_arm  = vexRT[Btn5D]; // L2

		displayLCDPos(0,0);
		sprintf(buffer, "Volts: %d", nAvgBatteryLevel);
		displayNextLCDString(buffer);


		// Set CLAW voltages.
		if (close_claw)
		{
			motor[CLAW] = -100;
		}
		else if (open_claw)
		{
			motor[CLAW] = 80;
		}
		else
		{
			motor[CLAW] = 0;
		}



		// Update ARM PID target.
		Pid *p = &pid_list[ARM];
		if (raise_arm && p->encoder_target < 800)
		{
			p->encoder_target += 10;
		}
		else if (lower_arm && p->encoder_target > 0)
		{
			p->encoder_target -= 10;
		}



		// Update CLAW PID target.
		p = &pid_list[CLAW];

		int delta = p->encoder_target - p->encoder;
		int delta_lim = 300;

		// If claw is trying to go backward above delta_lim
		// don't allow it to go target backward further.
		// Also for going forward.

		if (open_claw && !(delta > delta_lim))
		{
			p->encoder_target += 20;
		}
		else if (close_claw && !(delta < -delta_lim))
		{
			p->encoder_target -= 20;
		}



		// Trigger autonomous experiment
		if (vexRT[Btn7D] && !vexRT_P[Btn7D])
		{
			moveMotorsToTarget(1000, FRONT, true);
		}



		if (abs(vexRT[Ch3]) > threshold)
		{
			Y1 = vexRT[Ch3];
		}
		else
		{
			Y1 = 0;
		}

		if (abs(vexRT[Ch4]) > threshold)
		{
			X1 = vexRT[Ch4];
		}
		else
		{
			X1 = 0;
		}

		if (abs(vexRT[Ch1]) > threshold)
		{
			X2 = vexRT[Ch1];
		}
		else
		{
			X2 = 0;
		}



		// Modifier sets max speed to percentage
		// so the bot goes slower if is holding something

		float modifier = 1;
		if (nMotorEncoder[ARM] >= 900)
		{
				motor[FL] = (Y1 + X1 + X2)*modifier;
				motor[FR] = (Y1 - X1 - X2)*modifier;
				motor[BR] = (Y1 + X1 - X2)*modifier;
				motor[BL] = (Y1 - X1 + X2)*modifier;
		}
		else
		{
				motor[FL] = (Y1 + X1 + X2);
				motor[FR] = (Y1 - X1 - X2);
				motor[BR] = (Y1 + X1 - X2);
				motor[BL] = (Y1 - X1 + X2);
		}

		applyAllPID();



		for (int i = 0; i < NUMBER_OF_BUTTON_SLOTS; i++)
		{
			vexRT_P[i] = vexRT[i];
		}

		wait1Msec(10);
	}
}
