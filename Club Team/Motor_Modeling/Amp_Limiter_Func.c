/*
When motor is moving same direction as effort than limit normally
When motor is moving opposite direction as effort, don't allow negative effort to allow more than negative of amp limit
*/

typedef struct{
	int increaseAmpRate;
	int reduceAmpRate;
	int port;
	int group;
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

void init_SmartMotor(SmartMotor &sm, int smotor, int port)
{
	sm.increaseAmpRate = 3;
	sm.reduceAmpRate = 8;
	sm.port = port;
	sm.group = NULL;
	sm.direction = 0;
	sm.smotor = smotor;
	sm.effort = 0;
	sm.deltaEffort = 0;
	sm.targetAmp = 1.5;
	sm.voltDelta = 0;
	sm.totalAmp = 0;
	sm.ampsPsec = 0;
	sm.timeDiv = 0;
	sm.motorRes = 2.6;
	sm.backEMF = 0;
	sm.startVolts = (float)(nImmediateBatteryLevel*0.001);
	sm.currentVolts = 0;
	sm.effortVolts = 0;
	sm.Ke = 6.21;
	if(motorType[sm.smotor] == 11)
		sm.Ke = sm.Ke/100.0; //For torque
	else if(motorType[sm.smotor] == 12)
		sm.Ke = sm.Ke/160.0; //For High Speed
	else if(motorType[sm.smotor] == 13)
		sm.Ke = sm.Ke/240.0; //For Turbo
	else
		sm.Ke = 0.0; //Null if not recognized
	sm.amps = 0;
	sm.off = false;

}

SmartMotor motorS[12];
int groups[12][12] = {{0}};
int groupAmount[12] = {{0}};
float groupAmps[12] = {{0}};
float groupAmpsLimit[12] = {{4}};
bool monitorGroup[12] = {{false}};

void group_SmartMotor(int group, SmartMotor &sm){
	sm.group = group;
	groups[group][groupAmount[group]] = sm.smotor;
	groupAmount[group]++;
}



void start_SmartMotor(SmartMotor &sm){

	int positionCurrent = 0;
	int positionPrev = 0;
	float ampAvgDiv = 1;
	float ampAvg = 0;
	float calcAmp = 0;


	clearTimer(T1);

	while(1){

		sm.effort = motor[sm.smotor];
		positionCurrent = getMotorEncoder(sm.smotor);
		sm.backEMF = -(sm.Ke * (getMotorVelocity(sm.smotor))* sm.direction);
		calcAmp = 0;

		for(int j = 0; j < 12; j++){
			if(monitorGroup[j]){
				for(int i = 0; i < groupAmount[j] + 1; i++){
					calcAmp = calcAmp + motorS[groups[j][i]].amps;
				}
				groupAmps[j] = calcAmp;

			}
		}

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

		ampAvg = (sm.amps + ampAvg)/ampAvgDiv;


		++ampAvgDiv;
		//sm.amps = ampAvg;

		if(ampAvgDiv > 10){
			ampAvgDiv = 1;
			ampAvg = 0;
		}

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
			if(((sm.targetAmp < sm.amps) || (abs(groupAmps[sm.group]) > groupAmpsLimit[sm.group])) && (sm.direction == 1)  && (sm.deltaEffort < 127)){
				if(abs(groupAmps[sm.group]) > groupAmpsLimit[sm.group])
					sm.deltaEffort = sm.deltaEffort + 1;
				if(sm.targetAmp < sm.amps)
					sm.deltaEffort = sm.deltaEffort + 5;
			}

			else if(((sm.targetAmp < -sm.amps) || (abs(groupAmps[sm.group]) > groupAmpsLimit[sm.group])) && (sm.direction == -1)){
				if(abs(groupAmps[sm.group]) > groupAmpsLimit[sm.group])
					sm.deltaEffort = sm.deltaEffort - 1;
				if(sm.targetAmp < -sm.amps)
					sm.deltaEffort = sm.deltaEffort - 5;
			}


			//If Amps are below target and the limiter is within bounds, slowly allow more effort by 1 every 100msec
			else if(((sm.targetAmp) > abs(sm.amps))  || (abs(groupAmps[sm.group]) < groupAmpsLimit[sm.group])){
				if(sm.direction == 1 && sm.effort > 0 && sm.deltaEffort > 0)
					sm.deltaEffort = sm.deltaEffort - sm.increaseAmpRate;
				else if(sm.direction == -1 && sm.effort < 0 && sm.deltaEffort < 0)
					sm.deltaEffort = sm.deltaEffort + sm.increaseAmpRate;
			}


		}

		//Check if effort and direction are different
		if((sm.direction * sm.effort) < 1){

			//
			while(0.2 < abs(sm.amps)){
				motor[sm.smotor] =0;
				sm.deltaEffort = sm.effort;
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
