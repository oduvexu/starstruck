
#pragma competitionControl(Competition)
#include "Vex_Competition_Includes.c"

char buffer[64];
int mode = 0;
bool holding = false;

void pre_auton()
{
	bStopTasksBetweenModes = true;
	bDisplayCompetitionStatusOnLcd = false;
}

task autonomous()
{

}

task usercontrol()
{
	init();

	int threshold = 10;
	int X1 = 0;
	int Y1 = 0;
	int X2 = 0;

	// This will output to the console in debug mode
	writeDebugStreamLine("Program Start");

	while (true)
	{
		word open_claw = vexRT[Btn6U]; // R1
		word close_claw = vexRT[Btn6D]; // R2

		word raise_arm = vexRT[Btn5U]; // L1
		word lower_arm  = vexRT[Btn5D]; // L2

		int ArmBattery = (int)((float)SensorValue[BATTERY] * 3.537);

		displayLCDPos(0,0);
		sprintf(buffer, "Main mV: %d", nImmediateBatteryLevel);
		displayNextLCDString(buffer);

		displayLCDPos(1,0);
		sprintf(buffer, " Arm mV: %d", ArmBattery);
		displayNextLCDString(buffer);


		if (!SensorValue[LIMITSWITCH])
		{
			sprintf(buffer, "Detected limit switch at %d\n", -nMotorEncoder[ARM]);
			writeDebugStream(buffer);
		}



		int button_status = SensorValue[BUTTON];

		if (!button_status && -nMotorEncoder[ARM] < 100)
		{
			nMotorEncoder[ARM] = 0;
			writeDebugStreamLine("Reset ARM encoder.");
		}

		// Set CLAW voltages.
		if (close_claw)
		{
			if (getMotorVelocity(CLAW) > 30)
			{
				motor[CLAW] = -100;
			}
			else
			{
				motor[CLAW] = -50;
			}

			holding = false;
		}
		else if (!holding)
		{
			motor[CLAW] = 0;
		}

		if (open_claw)
		{
			if (getMotorVelocity(CLAW) > 30)
			{
				motor[CLAW] = 100;
			}
			else
			{
				motor[CLAW] = 50;
			}

			holding = true;
		}
		else if (holding)
		{
			if (-nMotorEncoder[ARM] < 100)
			{
				motor[CLAW] = 50;
			}
			else
			{
				motor[CLAW] = 50;
			}
		}

		//int vel = getMotorVelocity(ARM);


		/*
		if (raise_arm)
		{
			motor[ARM] = 127;
		}
		else if (lower_arm)
		{
			motor[ARM] = -127;
		}
		else
		{
			motor[ARM] = 0;
		}

		if (vel != 0)
		{
			sprintf(buffer, "Velocity: %d\n", vel);
			writeDebugStream(buffer);
		}
		*/





		// Update ARM PID target.
		Pid *p = &pid_list[ARM];

		int delta = p->encoder_target - p->encoder;
		int delta_lim = 300;

		if (raise_arm && !(delta > delta_lim))
		{
			p->encoder_target += 10;
			setFixed(false);
		}
		else if (lower_arm && !(delta < -delta_lim))
		{
			p->encoder_target -= 10;
			setFixed(false);
		}

		if (p->encoder_target - p->encoder > 300)
		{
			p->encoder_target = p->encoder + 300;
		}
		else if (p->encoder - p->encoder_target > 300)
		{
			p->encoder_target = p->encoder - 300;
		}

		if (!button_status && -nMotorEncoder[ARM] < 100 && p->encoder_target < 0)
		{
			p->encoder_target = 0;
		}



		if (!SensorValue[LIMITSWITCH] && p->encoder_target > p->encoder)
		{
			p->encoder_target = p->encoder;
		}


		// Update CLAW PID target.
		p = &pid_list[CLAW];

		delta = p->encoder_target - p->encoder;
		delta_lim = 300;

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
			moveMotorsToTarget(3000, FRONT, true);
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

		float modifier = 0.787;
		if (nMotorEncoder[ARM] >= 900)
		{
				motor[FL] = (Y1 + X1 + X2)*modifier;
				motor[FR] = (Y1 - X1 - X2)*modifier;
				motor[BR] = (Y1 + X1 - X2)*modifier;
				motor[BL] = (Y1 - X1 + X2)*modifier;
		}
		else
		{
				motor[FL] = (Y1 + X1 + X2)*modifier;
				motor[FR] = (Y1 - X1 - X2)*modifier;
				motor[BR] = (Y1 + X1 - X2)*modifier;
				motor[BL] = (Y1 - X1 + X2)*modifier;
		}

		applyAllPID();



		for (int i = 0; i < NUMBER_OF_BUTTON_SLOTS; i++)
		{
			vexRT_P[i] = vexRT[i];
		}

		wait1Msec(10);
	}
}
