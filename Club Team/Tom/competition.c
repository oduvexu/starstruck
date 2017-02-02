
#pragma competitionControl(Competition)
#include "Vex_Competition_Includes.c"

char buffer[64];
int mode = 0;
bool holding = false;

// Recorded values for wheels
signed byte _record_FL[250] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 18, 33, 40, 48, 49, 34, 30, 29, 14, 11, 10, 11, 14, 25, 44, 44, 46, 60, 65, 51, 51, 50, 31, 12, 16, 36, 39, 16, 0, -4, -3, -3, -11, -21, 13, 81, 72, 53, 18, 11, 0, -18, -19, -19, -19, -14, -3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 15, 31, 40, 48, 55, 63, 50, 46, 17, 14, 31, 32, 24, 16, 15, 29, 15, 12, 8, 0, 0, 0, 0, 47, 114, 127, 127, 127, 127, 125, 127, 127, 107, 93, 98, 105, 81, 0, -8, -30, -55, -82, -62, -11, -29, -55, 0, 51, 79, 53, 0, 0, 0, 0, 0, 6, 16, 0, 0, 0, -8, -18, -38, 0, 0, 0, 0, -26, -36, -36, -53, -55, -60, -70, -88, 0, 0, 0, -14, -13, -13, -13, -36, -40, -45, -38, -23, -29, -44, -48, -48, -21, -15, -16, -21, -40, -40, -40, -40, -40, -40, -40, -42, -49, -55, -60, -48, -44, -33, -25, -19, -11, 0, -36, -50, -48, -9, -27, 0, 38, 50, 53, 45, 52, 68, 47, -9, -29, -33, -29, -19, -28, -31, 14, 32, 36, 18, 20, 18, 0, 0, -11, -13, 0, 0, 25, 26, 40, 0, 0, 0, 0, -17, 0, 0, 0, -8, -15, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} ;
signed byte _record_FR[250] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 18, 33, 40, 48, 49, 66, 70, 71, 86, 88, 90, 89, 85, 81, 73, 74, 77, 92, 98, 112, 112, 113, 127, 127, 127, 110, 108, 117, 93, 88, 87, 87, 84, 70, 84, 81, 89, 77, 18, 11, 0, -127, -127, -127, -127, -127, -21, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -15, -31, -40, -48, -55, -63, -50, -46, -17, -14, -31, -32, -24, -16, -15, -29, -15, -12, -8, 0, 0, 0, 0, 22, 53, 67, 68, 69, 69, 69, 48, 9, 3, -10, -16, -22, 23, 0, -8, -30, -6, 27, 7, -11, -3, 0, 0, 51, 38, 12, 0, 0, 0, 0, 0, 25, 33, 0, 0, 0, -8, -18, -55, 0, 0, 0, 0, -67, -103, -109, -127, -127, -127, -123, -88, 0, 0, 0, -14, -13, -13, -13, -36, -73, -84, -77, -61, -59, -82, -86, -86, -63, -69, -88, -121, -99, -99, -99, -99, -99, -99, -99, -97, -101, -98, -93, -68, -64, -33, -8, 2, 11, 0, -7, 4, 4, 9, 27, 0, -38, -26, -26, -18, -25, -41, -23, -9, -29, -33, -29, -19, -28, -31, -14, -32, -36, -40, -37, -18, 0, 0, 11, 13, 0, 0, 25, 47, 59, 0, 0, 0, 0, -39, 0, 0, 0, -8, -15, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} ;
signed byte _record_BR[250] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 18, 33, 40, 48, 49, 66, 70, 71, 67, 66, 65, 62, 59, 52, 44, 44, 46, 60, 65, 68, 70, 69, 84, 92, 87, 66, 66, 74, 65, 59, 59, 59, 55, 48, 66, 81, 72, 53, 18, 11, 0, -18, -19, -19, -19, -14, -3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -15, -31, -40, -48, -55, -63, -50, -46, -17, -14, -31, -32, -24, -16, -15, -29, -15, -12, -8, 0, 0, 0, 0, 47, 83, 96, 96, 97, 97, 96, 100, 96, 75, 62, 66, 73, 56, 0, -8, -30, -55, -82, -62, -11, -29, -55, 0, 51, 79, 53, 0, 0, 0, 0, 0, 6, 16, 0, 0, 0, -8, -18, -38, 0, 0, 0, 0, -26, -36, -36, -53, -55, -60, -70, -88, 0, 0, 0, -14, -13, -13, -13, -36, -40, -45, -38, -23, 8, 27, 29, 28, -21, -15, -16, -21, -40, -40, -40, -40, -40, -40, -40, -42, -49, -55, -60, -48, -44, -33, -8, 2, 11, 0, -36, -50, -48, -9, -27, 0, 38, 50, 53, 45, 52, 68, 47, -9, -29, -33, -29, -19, -28, -31, 14, 32, 36, 18, 20, 18, 0, 0, -11, -13, 0, 0, 25, 26, 40, 0, 0, 0, 0, -17, 0, 0, 0, -8, -15, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
signed byte _record_BL[250] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 18, 33, 40, 48, 49, 34, 30, 29, 33, 33, 35, 37, 41, 54, 73, 74, 77, 92, 98, 95, 93, 94, 73, 56, 60, 81, 81, 59, 27, 23, 24, 24, 16, 0, 30, 81, 89, 77, 18, 11, 0, -127, -127, -127, -127, -127, -21, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 15, 31, 40, 48, 55, 63, 50, 46, 17, 14, 31, 32, 24, 16, 15, 29, 15, 12, 8, 0, 0, 0, 0, 22, 84, 99, 99, 99, 99, 99, 78, 40, 34, 21, 14, 9, 48, 0, -8, -30, -6, 27, 7, -11, -3, 0, 0, 51, 38, 12, 0, 0, 0, 0, 0, 25, 33, 0, 0, 0, -8, -18, -55, 0, 0, 0, 0, -67, -103, -109, -127, -127, -127, -123, -88, 0, 0, 0, -14, -13, -13, -13, -36, -73, -84, -77, -61, -96, -127, -127, -127, -63, -69, -88, -121, -99, -99, -99, -99, -99, -99, -99, -97, -101, -98, -93, -68, -64, -33, -25, -19, -11, 0, -7, 4, 4, 9, 27, 0, -38, -26, -26, -18, -25, -41, -23, -9, -29, -33, -29, -19, -28, -31, -14, -32, -36, -40, -37, -18, 0, 0, 11, 13, 0, 0, 25, 47, 59, 0, 0, 0, 0, -39, 0, 0, 0, -8, -15, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

// Recorded values for arm and claw.
signed byte _record_ARM[250] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 40, 78, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 78, 54, 35, 29, 28, 28, 28, 28, 28, 0, -17, 0, 10, 0, 0, 14, 0, 0, 0, 0, 14, 15, 15, 15, -14, -38, 0, 17, 0, 0, 0, 0, -10, -10, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -31, -27, -28, -12, 0, 0, -11, -11, -10, -12, -14, -14, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
signed byte _record_CLAW[250] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 30, 30, 100, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 100, 100, 100, 100, 100, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, -30, -100, 0, 0, 0, -30, -100, -100, -100, -100, 0, 0, 0, 0, 0, 0, -30, -30, -30, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -30, -30, -30, -100, -100, -100, -100, -100, -100, -100, -100, -100, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

void pre_auton()
{
	bStopTasksBetweenModes = true;
	bDisplayCompetitionStatusOnLcd = false;
}

task autonomous()
{

}

void playRecord()
{
	int wait = 100;
	int size = 250;

	for (int i = 0; i < size; i++)
	{
		motor[FL] = _record_FL[i];
		motor[FR] = _record_FR[i];
		motor[BR] = _record_BR[i];
		motor[BL] = _record_BL[i];
		motor[ARM] = _record_ARM[i];
		motor[CLAW] = _record_CLAW[i];

		wait1Msec(wait);
	}

	motor[FL] = 0;
	motor[FR] = 0;
	motor[BR] = 0;
	motor[BL] = 0;
	motor[ARM] = 0;
	motor[CLAW] = 0;
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
		if (vexRT[Btn8D])
		{
			playRecord();
		}


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
				motor[CLAW] = -30;
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
				motor[CLAW] = 30;
			}

			holding = true;
		}
		else if (holding)
		{
			if (-nMotorEncoder[ARM] < 100)
			{
				motor[CLAW] = 30;
			}
			else
			{
				motor[CLAW] = 30;
			}
		}




		// Update ARM PID target.
		Pid *p = &pid_list[ARM];

		int delta = p->encoder_target - p->encoder;
		int delta_lim = 300;

		if (raise_arm && !(delta > delta_lim))
		{
			p->encoder_target += 100;
			setFixed(false);
		}
		else if (lower_arm && !(delta < -delta_lim))
		{
			p->encoder_target -= 100;
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

		if (vexRT[Btn5U])
		{
			motor[ARM] = 100;
		}
		else if (vexRT[Btn5D])
		{
			motor[ARM] = -100;
		}
		else
		{
			motor[ARM] = 0;
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

		// applyAllPID();



		for (int i = 0; i < NUMBER_OF_BUTTON_SLOTS; i++)
		{
			vexRT_P[i] = vexRT[i];
		}

		datalogDataGroupStart();
			datalogAddValue(0, motor[FL]);
			datalogAddValue(1, motor[FR]);
			datalogAddValue(2, motor[BR]);
			datalogAddValue(3, motor[BL]);
			datalogAddValue(4, motor[ARM]);
			datalogAddValue(5, motor[CLAW]);
		datalogDataGroupEnd();

		wait1Msec(100);
	}
}
