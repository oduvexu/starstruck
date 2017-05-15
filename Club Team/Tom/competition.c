
#pragma competitionControl(Competition)
#include "Vex_Competition_Includes.c"

char buffer[64];
bool holding = false;

signed float _record_time[436] = {0.528,0.629,0.730,0.831,0.933,1.034,1.135,1.236,1.338,1.439,1.540,1.641,1.743,1.844,1.945,2.046,2.148,2.249,2.350,2.451,2.553,2.654,2.755,2.856,2.958,3.059,3.160,3.261,3.363,3.464,3.565,3.666,3.768,3.869,3.970,4.071,4.173,4.274,4.375,4.476,4.578,4.679,4.780,4.881,4.983,5.084,5.185,5.286,5.388,5.489,5.590,5.691,5.793,5.894,5.995,6.096,6.198,6.299,6.400,6.501,6.603,6.704,6.805,6.906,7.008,7.109,7.210,7.311,7.413,7.514,7.615,7.716,7.818,7.919,8.020,8.121,8.223,8.324,8.425,8.526,8.628,8.729,8.830,8.931,9.033,9.134,9.235,9.336,9.438,9.539,9.640,9.741,9.843,9.944,10.045,10.146,10.248,10.349,10.450,10.551,10.653,10.754,10.855,10.956,11.058,11.159,11.260,11.361,11.463,11.564,11.665,11.766,11.868,11.969,12.070,12.171,12.273,12.374,12.475,12.576,12.678,12.779,12.880,12.981,13.083,13.184,13.285,13.386,13.488,13.589,13.690,13.791,13.893,13.994,14.095,14.196,14.298,14.399,14.500,14.601,14.703,14.804,14.905,15.006,15.108,15.209,15.310,15.411,15.513,15.614,15.715,15.816,15.918,16.019,16.120,16.221,16.323,16.424,16.525,16.626,16.728,16.829,16.930,17.031,17.133,17.234,17.335,17.436,17.538,17.639,17.740,17.841,17.943,18.044,18.145,18.246,18.348,18.449,18.550,18.651,18.753,18.854,18.955,19.056,19.158,19.259,19.360,19.461,19.563,19.664,19.765,19.866,19.968,20.069,20.170,20.271,20.373,20.474,20.575,20.676,20.778,20.879,20.980,21.081,21.183,21.284,21.385,21.486,21.588,21.689,21.790,21.891,21.993,22.094,22.195,22.296,22.398,22.499,22.600,22.701,22.803,22.904,23.005,23.106,23.208,23.309,23.410,23.511,23.613,23.714,23.815,23.916,24.018,24.119,24.220,24.321,24.423,24.524,24.625,24.726,24.828,24.929,25.030,25.131,25.233,25.334,25.435,25.536,25.638,25.739,25.840,25.941,26.043,26.144,26.245,26.346,26.448,26.549,26.650,26.751,26.853,26.954,27.055,27.156,27.258,27.359,27.460,27.561,27.663,27.764,27.865,27.966,28.068,28.169,28.270,28.371,28.473,28.574,28.675,28.776,28.878,28.979,29.080,29.181,29.283,29.384,29.485,29.586,29.688,29.789,29.890,29.991,30.093,30.194,30.295,30.396,30.498,30.599,30.700,30.801,30.903,31.004,31.105,31.206,31.308,31.409,31.510,31.611,31.713,31.814,31.915,32.016,32.118,32.219,32.320,32.421,32.523,32.624,32.725,32.826,32.928,33.029,33.130,33.231,33.333,33.434,33.535,33.636,33.738,33.839,33.940,34.041,34.143,34.244,34.345,34.446,34.548,34.649,34.750,34.851,34.953,35.054,35.155,35.256,35.358,35.459,35.560,35.661,35.763,35.864,35.965,36.066,36.168,36.269,36.370,36.471,36.573,36.674,36.775,36.876,36.978,37.079,37.180,37.281,37.383,37.484,37.585,37.686,37.788,37.889,37.990,38.091,38.193,38.294,38.395,38.496,38.598,38.699,38.800,38.901,39.003,39.104,39.205,39.306,39.408,39.509,39.610,39.711,39.813,39.914,40.015,40.116,40.218,40.319,40.420,40.521,40.623,40.724,40.825,40.926,41.028,41.129,41.230,41.331,41.433,41.534,41.635,41.736,41.838,41.939,42.040,42.141,42.243,42.344,42.445,42.546,42.648,42.749,42.850,42.951,43.053,43.154,43.255,43.356,43.458,43.559,43.660,43.761,43.863,43.964,44.065,44.166,44.268,44.369,44.470,44.571,};
signed byte _record_FL[436] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,77,85,99,99,108,81,56,31,31,27,14,14,13,11,-3,5,11,11,54,99,112,112,99,99,44,13,7,73,55,0,0,0,-83,-108,-108,-108,-108,-108,-99,-99,-99,-74,-69,-65,-43,-2,42,99,99,99,99,99,16,0,0,0,0,0,0,0,0,0,0,34,114,115,115,115,113,113,113,113,113,113,113,113,99,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-99,-99,-99,-99,-99,-99,-99,-99,-99,-39,0,33,127,127,127,127,127,127,127,127,127,127,127,127,127,127,127,127,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-27,-69,-38,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-99,-99,-88,-83,-69,-44,44,52,65,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-25,-29,0,0,15,16,-5,0,0,0,0,0,0,68,109,109,127,117,73,73,75,76,40,29,-5,-76,-99,-99,-99,-91,-88,-88,-90,-127,99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,16,0,0,0,0,0,0,0,0,0,10,88,85,40,-13,6,0,0,0,-46,16,85,85,85,85,71,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,9,11,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-9,-99,-99,-99,-99,-99,-99,-99,-99,-99,-99,-99,-99,-99,-99,-99,-99,0,0,0,11,127,126,126,126,126,126,116,114,114,114,114,114,114,114,114,113,90,0,0,0,0,0,0,-18,-99,-99,-99,-99,-99,-99,-99,-99,-99,-99,-56,0,27,63,99,99,99,99,99,99,99,0,0,0,0,0,0,0,0,0,-50,-99,-10,0,58,127,127,127,127,1};
signed byte _record_FR[436] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,121,114,99,99,91,118,127,127,127,127,127,127,127,127,127,127,127,127,127,99,87,87,99,99,127,127,127,126,11,0,0,0,-83,-91,-91,-91,-91,-91,-99,-99,-99,-125,-127,-127,-127,-84,-42,-99,-99,-99,-99,-99,-16,0,0,0,0,0,0,0,0,0,0,17,84,84,84,84,86,86,86,86,86,86,86,86,99,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,99,99,99,99,99,99,99,99,99,39,0,7,48,24,22,8,0,0,25,50,50,51,51,51,51,51,51,51,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-27,-127,-38,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-99,-99,-111,-116,-127,-127,-127,-127,-114,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-25,-29,0,0,-15,-16,-110,0,0,0,0,0,0,48,90,90,3,82,125,126,124,123,127,127,71,-76,-99,-99,-99,-108,-111,-111,-109,-69,-99,-99,-99,-99,-99,-99,-99,-99,-99,-99,-99,-99,-99,-99,-99,-99,-16,0,0,0,0,0,0,0,0,0,10,110,114,127,127,127,0,0,0,46,16,114,114,114,114,96,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,9,11,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,9,99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,0,0,0,11,72,73,73,73,73,73,83,85,85,85,85,85,85,85,85,86,60,0,0,0,0,0,0,-18,-99,-99,-99,-99,-99,-99,-99,-99,-99,-99,-23,0,-27,-63,-99,-99,-99,-99,-99,-99,-99,0,0,0,0,0,0,0,0,0,50,99,10,0,22,65,65,65,38,3};
signed byte _record_BR[436] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,77,85,99,99,108,81,56,31,31,27,14,14,13,11,-3,5,11,11,54,99,112,112,99,99,44,13,7,73,55,0,0,0,-83,-108,-108,-108,-108,-108,-99,-99,-99,-74,-69,-65,-43,-2,-42,-99,-99,-99,-99,-99,-16,0,0,0,0,0,0,0,0,0,0,34,114,115,115,115,113,113,113,113,113,113,113,113,99,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,99,99,99,99,99,99,99,99,99,39,0,33,127,127,127,127,127,127,127,127,127,127,127,127,127,127,127,127,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-27,-69,-38,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-99,-99,-88,-83,-69,-44,44,52,65,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-25,-29,0,0,15,16,-5,0,0,0,0,0,0,68,109,109,127,117,73,73,75,76,40,29,-5,-76,-99,-99,-99,-91,-88,-88,-90,-127,-99,-99,-99,-99,-99,-99,-99,-99,-99,-99,-99,-99,-99,-99,-99,-99,-16,0,0,0,0,0,0,0,0,0,10,88,85,127,127,127,0,0,0,46,16,85,85,85,85,71,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,9,11,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,9,99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,0,0,0,11,127,126,126,126,126,126,116,114,114,114,114,114,114,114,114,113,90,0,0,0,0,0,0,-18,-99,-99,-99,-99,-99,-99,-99,-99,-99,-99,-56,0,-27,-63,-99,-99,-99,-99,-99,-99,-99,0,0,0,0,0,0,0,0,0,50,99,10,0,58,127,127,127,127,1};
signed byte _record_BL[436] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,121,114,99,99,91,118,127,127,127,127,127,127,127,127,127,127,127,127,127,99,87,87,99,99,127,127,127,126,11,0,0,0,-83,-91,-91,-91,-91,-91,-99,-99,-99,-125,-127,-127,-127,-84,42,99,99,99,99,99,16,0,0,0,0,0,0,0,0,0,0,17,84,84,84,84,86,86,86,86,86,86,86,86,99,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-99,-99,-99,-99,-99,-99,-99,-99,-99,-39,0,7,48,24,22,8,0,0,25,50,50,51,51,51,51,51,51,51,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-27,-127,-38,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-99,-99,-111,-116,-127,-127,-127,-127,-114,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-25,-29,0,0,-15,-16,-110,0,0,0,0,0,0,48,90,90,3,82,125,126,124,123,127,127,71,-76,-99,-99,-99,-108,-111,-111,-109,-69,99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,16,0,0,0,0,0,0,0,0,0,10,110,114,68,13,23,0,0,0,-46,16,114,114,114,114,96,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,9,11,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-9,-99,-99,-99,-99,-99,-99,-99,-99,-99,-99,-99,-99,-99,-99,-99,-99,0,0,0,11,72,73,73,73,73,73,83,85,85,85,85,85,85,85,85,86,60,0,0,0,0,0,0,-18,-99,-99,-99,-99,-99,-99,-99,-99,-99,-99,-23,0,27,63,99,99,99,99,99,99,99,0,0,0,0,0,0,0,0,0,-50,-99,-10,0,22,65,65,65,38,3};
signed byte _record_ARM[436] = {0,0,0,0,0,0,0,0,0,0,100,100,100,100,100,100,100,100,100,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-100,-100,-100,-100,-100,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,0,0,0,0,100,100,0,0,0,0,100,100,0,0,0,0,100,100,0,0,0,0,100,100,0,0,0,0,-100,-100,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,100,100,100,100,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-100,-100,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,100,100,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-100,-100,-100,-100,-100,-100,-100,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,0,0,0,0,100,100,0,0,0,0,100,100,0,0,0,0,100,100,0,0,0,0,0,100,100,0,0,0,0,100,100,0,0,0,0,100,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-100,-100,-100,-100,-100,-100,-100,-100,-100,0,0,0,0,0,0,0,0};
signed byte _record_CLAW[436] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,100,30,30,30,30,30,30,30,30,30,30,-100,-100,0,0,0,0,0,0,0,0,0,0,100,30,30,30,30,-100,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,-100,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-100,-100,-100,-100,-100,-100,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-100,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,30,30,30,-100,-100,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

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
	int i = 0;

	clearTimer(T1);

	while (time1[T1] < 45*1000)
	{
		if (time1[T1] > round(_record_time[i]*1000.0))
		{
			// Do this step of motor values

			motor[FL] = _record_FL[i];
			motor[FR] = _record_FR[i];
			motor[BR] = _record_BR[i];
			motor[BL] = _record_BL[i];
			motor[ARM] = _record_ARM[i];
			motor[CLAW] = _record_CLAW[i];

			i++;
		}

		wait1Msec(1);
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
			if (SensorValue[LIMITSWITCH])
			{
				motor[ARM] = 100;
			}
			else
			{
				writeDebugStream("Detected higher limit\n");
				motor[ARM] = 0;
			}
		}
		else if (vexRT[Btn5D])
		{
			if (SensorValue[BUTTON])
			{
				motor[ARM] = -100;
			}
			else
			{
				writeDebugStream("Detected lower limit\n");
				motor[ARM] = 0;
			}
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
