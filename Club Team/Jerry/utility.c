#include "utility.h"

void init()
{
	playSound(soundBeepBeep);
	slaveMotor(WINCH2,WINCH);

	engageLock();

	//initializeClutch();
}

void initializeClutch()
{
	motor[CLUTCH] = -20;
	wait1Msec(500);
	motor[CLUTCH] = 20;
	wait1Msec(2000);
	motor[CLUTCH] = 0;
	resetMotorEncoder(CLUTCH);
}

bool launching = false;

task launch()
{
	launching = true;

	motor[CLUTCH] = 127;
	wait1Msec(200);
	disengageLock();

	while (vexRT[Btn6D])
	{
		wait1Msec(10);
	}

	motor[CLUTCH] = 0;

	launching = false;
}

void engageLock()
{
	motor[LOCK] = 10;
	wait1Msec(100);
}

void disengageLock()
{
	motor[LOCK] = 90;
	wait1Msec(100);
}

void engageClutch()
{
	motor[CLUTCH] = -80;
	wait1Msec(300);
	motor[CLUTCH] = 0;
}

void disengageClutch()
{
	motor[CLUTCH] = 80;
	wait1Msec(300);
	motor[CLUTCH] = 0;
}

void raise()
{
	disengageLock();
	motor[WINCH] = -60;
	wait1Msec(200);
	motor[WINCH] = 0;
	engageLock();

}

void holdRaise(){
	motor[WINCH] = -60;
	disengageLock();
}

void endRaise(){
	motor[WINCH] = 0;
	engageLock();
}

void forward(int x){
		motor[FL] = -x;
		motor[FR] = x;
		motor[BR] = x;
		motor[BL] = -x*1.2;
}

void backward(int x){
		motor[FL] = x;
		motor[FR] = -x;
		motor[BR] = -x;
		motor[BL] = x*1.2;
}

void sideLeft(int x){
		motor[FL] = -x;
		motor[FR] = x;
		motor[BR] = -x;
		motor[BL] = x*1.2;
}

void speedVoltageCalibrationTest()
{
	datalogClear();

	datalogDataGroupStart();
	datalogAddValue(0, nAvgBatteryLevel);
	datalogDataGroupEnd();

	for (int i = 0; i <= 5; i++)
	{
		int effort = 25 + i*20;

		motorForward(effort);

		clearTimer(timer1);
		int time = time1[timer1];

		while (time < 3000)
		{
			float vel_FL = getMotorVelocity(FL);
			float vel_FR = getMotorVelocity(FR);
			float vel_BL = getMotorVelocity(BL);
			float vel_BR = getMotorVelocity(BR);

			datalogDataGroupStart();
			datalogAddValue(0, effort);
			datalogAddValue(1, vel_FL);
			datalogAddValue(2, vel_FR);
			datalogAddValue(3, vel_BL);
			datalogAddValue(4, vel_BR);
			datalogDataGroupEnd();

			motorForward(effort);

			wait1Msec(10);

			time = time1[timer1];
		}
	}
}

void moveForwardTicks(int target)
{
	int initial_tick = nMotorEncoder[FL];
	int current_tick = nMotorEncoder[FL];

	int tick_FL = nMotorEncoder[FL];
	int tick_FR = nMotorEncoder[FR];
	int tick_BL = nMotorEncoder[BL];
	int tick_BR = nMotorEncoder[BR];

	int tick_FL_prev = tick_FL;
	int tick_FR_prev = tick_FR;
	int tick_BL_prev = tick_BL;
	int tick_BR_prev = tick_BR;

	bool changed = false;

	int effort = 25;

	int voltage = nAvgBatteryLevel;

	datalogClear();

	datalogDataGroupStart();
	datalogAddValue(0, voltage);
	datalogAddValue(1, effort);
	datalogDataGroupEnd();

	motorForward(25);

	while (current_tick < initial_tick + target)
	{
		tick_FL = nMotorEncoder[FL];
		tick_FR = nMotorEncoder[FR];
		tick_BL = nMotorEncoder[BL];
		tick_BR = nMotorEncoder[BR];

		changed = (tick_FL != tick_FL_prev)||(tick_FR != tick_FR_prev)||(tick_BL != tick_BL_prev)||(tick_BR != tick_BR_prev);

		if (changed)
		{
			float vel_FL = getMotorVelocity(FL);
			float vel_FR = getMotorVelocity(FR);
			float vel_BL = getMotorVelocity(BL);
			float vel_BR = getMotorVelocity(BR);

			datalogDataGroupStart();
			datalogAddValue(0, vel_FL);
			datalogAddValue(1, vel_FR);
			datalogAddValue(2, vel_BL);
			datalogAddValue(3, vel_BR);
			datalogDataGroupEnd();

			motorForward(25);
		}

		tick_FL_prev = tick_FL;
		tick_FR_prev = tick_FR;
		tick_BL_prev = tick_BL;
		tick_BR_prev = tick_BR;

		current_tick = nMotorEncoder[FL];

		wait1Msec(10);
	}

	motorStop();
}

void motorForward(int speed)
{
	if (speed > 0)
	{
		motor[FL] = speed;
		motor[FR] = speed;
		motor[BL] = speed;
		motor[BR] = speed;
	}
}

void motorStop()
{
	motor[FL] = 0;
	motor[FR] = 0;
	motor[BL] = 0;
	motor[BR] = 0;
}

void autonomous()
{
	motor[FL] = 100;
	motor[FR] = 100;
	motor[BL] = 100;
	motor[BR] = 100;
	wait1Msec(2000);
	motorStop();

	disengageLock();
	motor[WINCH] = 60;
	wait1Msec(1500);
	motor[WINCH] = 0;

	motor[FL] = 100;
	motor[FR] = 100;
	motor[BL] = 100;
	motor[BR] = 100;
	wait1Msec(500);
	motorStop();

	disengageLock();
	disengageClutch();
	wait1Msec(200);
	engageClutch();
}

void zains_a_bitch_cunt()
{
	motor[FL]= 80:
	motor[FR]=80;
	motor[BL]=80;
	motor[BR]=80;
	wait1Msec(2500);
	motor[FL]= -80:
	motor[FR]=80;
	motor[BL]=-80;
	motor[BR]=80;
	wait1Msec(2000);
	motor[FL]=0;
	motor[FR]=0;
	motor[BL]=0;
	motor[BR]=0;

}
