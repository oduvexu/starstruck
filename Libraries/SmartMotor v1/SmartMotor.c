/*
When motor is moving same direction as effort than limit normally
When motor is moving opposite direction as effort, don't allow negative effort to allow more than negative of amp limit
*/

typedef struct{
	int input;
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
	float multiplier;
	bool off;
	float backEMF;
	int direction;
} SmartMotor;

void init_SmartMotor(SmartMotor &sm, int smotor)
{
	sm.increaseAmpRate = 3;
	sm.reduceAmpRate = 8;
	sm.port = smotor;
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
float groupDelta[12] = {{0}};
float groupMultiplier[12] = {{0}};
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
	float calcDelta = 0;


	clearTimer(T1);

	while(1){
		sm.effort = motor[sm.smotor];
		positionCurrent = getMotorEncoder(sm.smotor);
		sm.backEMF = -(sm.Ke * (getMotorVelocity(sm.smotor))* sm.direction);
		calcAmp = 0;
		calcDelta = 0;

		for(int j = 0; j < 12; j++){
			if(monitorGroup[j]){
				for(int i = 0; i < groupAmount[j] + 1; i++){
					calcAmp = calcAmp + motorS[groups[j][i]].amps;
					if(abs(calcDelta) < abs(motorS[groups[j][i]].deltaEffort))
						calcDelta =  motorS[groups[j][i]].deltaEffort;
				}
				groupAmps[j] = calcAmp;
				groupDelta[j] = calcDelta;
				groupMultiplier[j] = (127.0-groupDelta[j])/127.0;
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

		else if(getMotorVelocity(sm.smotor) == 0)
			sm.direction = 0;

		//Check if effort and direction are different
		if((sm.direction * sm.effort) < -15){

			writeDebugStream("Wrong Way:%d",(sm.direction * sm.effort));

			if(getMotorVelocity(sm.smotor) > 15 && (sm.direction * sm.effort) < 0){
				positionCurrent = getMotorEncoder(sm.smotor);
				writeDebugStream("Loop:%d",(sm.direction * sm.effort));
				sm.input = sm.input*(15.0/127.0);
				sm.deltaEffort = 0;
				sm.amps = ((sm.backEMF)/sm.motorRes);
				sm.backEMF = -(sm.Ke * (getMotorVelocity(sm.smotor))* sm.direction);
				sm.effortVolts = 0;
				sm.effort = sm.input;

				if(positionCurrent > positionPrev)
					sm.direction = 1;

				else if(positionCurrent < positionPrev)
					sm.direction = -1;

				positionPrev = getMotorEncoder(sm.smotor);
			}

			//If effort is less than 5 than zero the change of effort.
			//We don't need to monitor effort if it's lower than 5.
			if((sm.effort < 10 && sm.effort > -10)){
				sm.deltaEffort = 0;
				sm.effort = 0;
			}

			//If Amps are below target and the limiter is within bounds, slowly allow more effort by 1 every 100msec
			else if((sm.targetAmp) > abs(sm.amps)){
				sm.input = sm.input*(20.0/127.0);
			}

		}

		//Check if effort and direction are the same way
		else{


			//If effort is less than 5 than zero the change of effort.
			//We don't need to monitor effort if it's lower than 5.
			//if((sm.effort*sm.deltaEffort) < 0)
			//	sm.deltaEffort = 0;

			clearDebugStream();
			writeDebugStream("Bool:%f\n",(((abs(sm.targetAmp) < abs(sm.amps)) || (abs(groupAmps[sm.group]) > groupAmpsLimit[sm.group])) && (sm.deltaEffort < 127));
			writeDebugStream("Delta:%f\n",sm.deltaEffort);
			writeDebugStream("Multi:%f\n",sm.multiplier);
			//If Amps go above target and the limiter is within bounds, deduct effort by 5 every 100msec
			if(((abs(sm.targetAmp) < abs(sm.amps)) || (abs(groupAmps[sm.group]) > groupAmpsLimit[sm.group])) && sm.deltaEffort < 127){
				//	if(abs(groupAmps[sm.group]) > groupAmpsLimit[sm.group])
				//		sm.deltaEffort = sm.deltaEffort + 1;
				writeDebugStream("Check:%d",(sm.direction * sm.effort));
				if(abs(sm.targetAmp) < abs(sm.amps)){
					sm.deltaEffort = sm.deltaEffort + 5;
					writeDebugStream("Check2:%d",(sm.direction * sm.effort));
				}
			}

			//If Amps are below target and the limiter is within bounds, slowly allow more effort by 1 every 100msec
			else if(((abs(sm.targetAmp) > abs(sm.amps))  || (abs(groupAmps[sm.group]) < groupAmpsLimit[sm.group])) && sm.deltaEffort > 0){
				sm.deltaEffort = sm.deltaEffort - sm.increaseAmpRate;
			}
			else
				sm.deltaEffort = 0;

			sm.multiplier = (127.0 - (sm.deltaEffort))/127.0;

			if((sm.direction * sm.input) > -1 && getMotorVelocity(sm.smotor) > 0)
				motor[sm.smotor] = sm.input;

			else if((sm.direction * sm.input) < -1 && getMotorVelocity(sm.smotor) > 0){
				motor[sm.smotor] = sm.input*((127-(getMotorVelocity(sm.smotor)*sm.Ke*60))/127);

			}

			else if(getMotorVelocity(sm.smotor) == 0)
				motor[sm.smotor] = sm.input;

		}


		sm.totalAmp = sm.totalAmp + sm.amps;
		sm.timeDiv++;
		sm.ampsPsec = sm.totalAmp/(sm.timeDiv*50);
		sm.voltDelta = (sm.ampsPsec*(time1[T1]/1000))*0.0270/(0.686*212);
		wait1Msec(10);
		positionPrev = positionCurrent;
	}

}
