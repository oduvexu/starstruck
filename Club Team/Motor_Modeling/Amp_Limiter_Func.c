/*
When motor is moving same direction as effort than limit normally
When motor is moving opposite direction as effort, don't allow negative effort to allow more than negative of amp limit
*/


typedef struct{
int smotor;
int effort;
int deltaEffort;
float targetAmp;
int motorVel;
int currentVel;
float voltDelta;
float totalAmp;
float ampsPsec;
float timeDiv;
float motorRes;
float startVolts;
float currentVolts;
float effortVolts;
float Ke;
float amps;
bool off;
float backEMF;
int direction;
} SmartMotor;

void init_SmartMotor(SmartMotor sm, int smotor)
{
sm.direction = 0;
sm.smotor = smotor;
sm.effort = 0;
sm.deltaEffort = 0;
sm.targetAmp = 2;
sm.voltDelta = 0;
sm.totalAmp = 0;
sm.ampsPsec = 0;
sm.timeDiv = 0;
sm.motorRes = 2.6;
sm.backEMF = 0;
sm.startVolts = (float)(nImmediateBatteryLevel*0.001);
sm.currentVolts = 0;
sm.effortVolts = 0;
//if(motorType[sm.smotor] == "tmotorVex393_MC29")
sm.Ke = 0.0621; //For torque
sm.amps = 0;
sm.off = false;

}

void start_SmartMotor(SmartMotor sm){

int positionCurrent = 0;
int positionPrev = 0;


clearTimer(T1);

while(1){

sm.effort = motor[sm.smotor];
positionCurrent = getMotorEncoder(sm.smotor);
sm.backEMF = -(sm.Ke * (getMotorVelocity(sm.smotor))* sm.direction);


//Effort can never be "0" as far as the data is concern. This will not actually change the effort
if(sm.deltaEffort == 0)
{
	sm.deltaEffort = 1;
}

//Equation was built strictly from experimental data. Estimates volts given by effort

	if(sm.effort > 10)
	sm.effortVolts = (sm.startVolts/20.0)*(29.0-(88.0/sqrt(sm.effort)));

	else if(sm.effort < -10)
	sm.effortVolts = -(sm.startVolts/20.0)*(29.0-(88.0/sqrt(-sm.effort)));

	else
	sm.effortVolts = 0;

	if(sm.effortVolts > sm.startVolts && sm.startVolts > 0)
		sm.effortVolts = sm.startVolts;

	if(-sm.effortVolts < sm.startVolts && sm.startVolts < 0)
		sm.effortVolts = -sm.startVolts;

//This is a basic equation of "Amps = [V(int) - V(BackEMF)]/motorInternalResistance
if(sm.effort > 10 || sm.effort < -10)
sm.amps = ((sm.effortVolts+sm.backEMF)/sm.motorRes);

else if(sm.effort < 10 && sm.effort > -10)
sm.amps = ((sm.backEMF)/sm.motorRes);

if(positionCurrent > positionPrev)
sm.direction = 1;

else if(positionCurrent < positionPrev)
sm.direction = -1;

//Check if effort and direction are the same way
if((sm.direction * sm.effort) > 1){


	//If effort is less than 5 than zero the change of effort.
	//We don't need to monitor effort if it's lower than 5.
	if((sm.effort*sm.deltaEffort) < 0)
	sm.deltaEffort = 0;

	//If Amps go above target and the limiter is within bounds, deduct effort by 5 every 100msec
		if((sm.targetAmp < sm.amps) && (sm.direction == 1)  && (sm.deltaEffort < 127))
			sm.deltaEffort = sm.deltaEffort + 5;
		else if((sm.targetAmp < -sm.amps) && (sm.direction == -1))
			sm.deltaEffort = sm.deltaEffort - 5;


	//If Amps are below target and the limiter is within bounds, slowly allow more effort by 1 every 100msec
	else if((sm.targetAmp) > abs(sm.amps)){
		if(sm.direction == 1 && sm.effort > 0 && sm.deltaEffort > 0)
			sm.deltaEffort = sm.deltaEffort - 3;
		else if(sm.direction == -1 && sm.effort < 0 && sm.deltaEffort < 0)
			sm.deltaEffort = sm.deltaEffort + 3;
	}


}

//Check if effort and direction are different
if((sm.direction * sm.effort) < 1){

	//
	while(sm.targetAmp < abs(sm.amps)){
		motor[sm.smotor] = 0;
		sm.deltaEffort = 0;
		sm.amps = ((sm.backEMF)/sm.motorRes);
		sm.backEMF = -(sm.Ke * (getMotorVelocity(sm.smotor))* sm.direction);
		sm.effortVolts = 0;
	}

	//If effort is less than 5 than zero the change of effort.
	//We don't need to monitor effort if it's lower than 5.
	if((sm.effort < 10 && sm.effort > -10))
	sm.deltaEffort = 0;


	//If Amps are below target and the limiter is within bounds, slowly allow more effort by 1 every 100msec
	else if((sm.targetAmp) > abs(sm.amps)){

	}

}

sm.totalAmp = sm.totalAmp + sm.amps;
sm.timeDiv++;
sm.ampsPsec = sm.totalAmp/(sm.timeDiv*50);
sm.voltDelta = (sm.ampsPsec*(time1[T1]/1000))*0.0270/(0.686*212);
wait1Msec(100);
positionPrev = positionCurrent;
}

}
