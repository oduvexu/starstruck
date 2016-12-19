#include "utility.h"

void init()
{
	slaveMotor(ARMSLAVE, ARM);
	slaveMotor(ARMSLAVE2, ARM);
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
		motor[FL] = x;
		motor[FR] = x;
		motor[BR] = -x;
		motor[BL] = -x*1.2;
}

void autonomous(){




}

//Jerry Functions
/*



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
	disengageLock();
	motor[WINCH] = -60;
}

void endRaise(){
	motor[WINCH] = 0;
	engageLock();
}
*/
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
		motor[FL] = x;
		motor[FR] = x;
		motor[BR] = -x;
		motor[BL] = -x*1.2;
}

void autonomous(){




}
