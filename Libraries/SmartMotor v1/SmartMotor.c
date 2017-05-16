/*

Updated: 4/30/2017

Basic Instructions:
      Thank you for using the smart motor library made at ODU as an attempt to reduce the over heating of motors. The method used is to predict the amps and limit the effort in respect to amps.
      Anyone of the settings in the init_SmartMotor initializer can be set to how you want such as amp limit, rate of decrease in effort, rate of increase in effort etc...

      ---------Step 1: Initialize Smart Motors------------------
      In the main function, initialize all the motors you want to be limited by calling the init_SmartMotor function so...

      init_SmartMotor(smart_motor[Wheel1], Wheel1);
      init_SmartMotor(smart_motor[Wheel2], Wheel2);

      where "Wheel" is what you called the motor when you created it. It uses the same value that you created for the motor to convert it into a Smart Motor.

      ---------Step2: Begin the Smart Motor Task-----------------------------------
      Once you've initialized the Smotor, you will create a task for every motor. so...

      //declare the tasks
      task smotor1;
      task smotor2;

      //define the tasks
      task smotor1{
      start_SmartMotor(smart_motor[Wheel1]);
      }

      task smotor2{
      start_SmartMotor(smart_motor[Wheel2]);
      }

      //In your main function, start the tasks
      startTask(smotor1);
      startTask(smotor2);

      ---------Step3: Change any settings if default isn't desired-----------------
      You can change various settings if you'd like

      //Change amp limit, 2 amps is default
      smart_motor[Wheel1].targetAmp = 2;

      //Change amp increase when under amp limit,3 is default
      smart_motor[Wheel1].increaseAmpRate = 3;

      //Change amp decrease when over amp limit, 8 is default
      smart_motor[Wheel1].reduceAmpRate = 8;

      ---------Step4: Multiply by the multiplier-----------------------------------
      When you us the Smart Motor Library you no long equate the motor[] with the input. All you have to do is add an "S" at the end and put a .input so smotor[].input . This will activate all the calculations needed to limit the motors.
      If you want to limit the motors however you need to multiply by the multiplier. so...

      motor[TEST] = vexRT[CH1];

      becomes

      smart_motor[TEST].input = vexRT[CH1] * smart_motor[TEST].multiplier;



Grouping Motors:
         We can also set the limit of groups of motors. This is to protect the cortex itself from over heating. Ports 1-5, 6-10 and the expander all share it's own
         respected PTC (Positive Temperature Coefficient) fuse. It's a good idea to limit the entire group of motors on each set of ports to some amps as a whole.
         Grouping motors is extremely easy. First you need to set all the motors to Smart Motors as stated above, then add them to groups. Once they're in groups you can multiply by
         the group multiplier and the set will be changed equally. This is optimal for drive systems where you don't want to reduce the effort unevenly.

       ---------Step1: Add motor to group------------------------------------------

       //Adds 3 motors to group 0
       group_SmartMotor(0,smart_motor[Wheel1]);
       group_SmartMotor(0,smart_motor[Wheel2]);
       group_SmartMotor(0,smart_motor[Wheel3]);
       
       //Monitor group 0
       monitorGroup[0] = true;

       //Set multiplier

      smart_motor[Wheel1].input = vexRT[CH1] * groupMultiplier[0];
      smart_motor[Wheel2].input = vexRT[CH1] * groupMultiplier[0];
      smart_motor[Wheel3].input = vexRT[CH1] * groupMultiplier[0];


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

SmartMotor smart_motor[12];
int groups[12][12] = {{0}};
int groupAmount[12] = {{0}};
int groupGhostAmount[12] = {{0}};
float groupAmps[12] = {{0}};
float groupDelta[12] = {{0}};
float groupMultiplier[12] = {{1}};
float groupAmpsLimit[12] = {{4}};
bool monitorGroup[12] = {{false}};
int check = 0;

void group_SmartMotor(int group, SmartMotor &sm){
	sm.group = group;
	groups[group][groupAmount[group]] = sm.smotor;
	check = groups[group][groupAmount[group]];
	groupAmount[group]++;
}

void group_GhostMotor(int group, int amount){
	groupGhostAmount[group] = amount;
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
	////Stream Troubleshoot
	//		clearDebugStream();
	//		writeDebugStream("Delta:%f\n",sm.deltaEffort);
	//		writeDebugStream("Multi:%f\n",groupMultiplier[1]);
	//		writeDebugStream("Check:%f\n",groups[0][1]);
	//		writeDebugStream("GroupAmps:%f",groupAmps[1]);
	
	
	
		sm.effort = motor[sm.smotor];
		positionCurrent = getMotorEncoder(sm.smotor);
		sm.backEMF = -(sm.Ke * (getMotorVelocity(sm.smotor))* sm.direction);
		calcAmp = 0;
		calcDelta = 0;

		for(int j = 0; j < 12; j++){
			if(monitorGroup[j]){
				for(int i = 0; i < groupAmount[j] + 1; i++){
					calcAmp = calcAmp + smart_motor[groups[j][i]].amps;
					if((abs(groupAmps[sm.group]) > groupAmpsLimit[sm.group]) && groupDelta[sm.group] < 127)
						groupDelta[j] = groupDelta[j] + 5;
					else if((abs(groupAmps[sm.group]) < groupAmpsLimit[sm.group]) && groupDelta[sm.group] > 0)
						groupDelta[j] = groupDelta[j] - 1;
					
				}
				
				groupAmps[j] = calcAmp + (calcAmp * (groupGhostAmount[j]/groupAmount[j]));
				groupMultiplier[j] = abs((127.0-groupDelta[j])/127.0);
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

			if(getMotorVelocity(sm.smotor) > 15 && (sm.direction * sm.effort) < 0){
				positionCurrent = getMotorEncoder(sm.smotor);
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
			//	sm.input = sm.input*(20.0/127.0);
			}

		}

		//Check if effort and direction are the same way
		else{


			//If effort is less than 5 than zero the change of effort.
			//We don't need to monitor effort if it's lower than 5.
			//if((sm.effort*sm.deltaEffort) < 0)
			//	sm.deltaEffort = 0;


			//If Amps go above target and the limiter is within bounds, deduct effort by 5 every 100msec
			if((abs(sm.targetAmp) < abs(sm.amps)) && sm.deltaEffort < 127){
					sm.deltaEffort = sm.deltaEffort + 5;
			}

			//If Amps are below target and the limiter is within bounds, slowly allow more effort by 1 every 100msec
			else if(((abs(sm.targetAmp) > abs(sm.amps))  || (abs(groupAmps[sm.group]) < groupAmpsLimit[sm.group])) && sm.deltaEffort > 0){
				sm.deltaEffort = sm.deltaEffort - sm.increaseAmpRate;
			}
			else
				sm.deltaEffort = 0;

			sm.multiplier = (127.0 - (sm.deltaEffort))/127.0;
	}
			
			if((sm.direction * sm.input) > -1 && getMotorVelocity(sm.smotor) > 0)
				motor[sm.smotor] = sm.input;

			else if((sm.direction * sm.input) <= -1 && getMotorVelocity(sm.smotor) > 0){
				motor[sm.smotor] = sm.input*(1/(getMotorVelocity(sm.smotor)*sm.Ke*2));

			}

			else if(getMotorVelocity(sm.smotor) == 0)
				motor[sm.smotor] = sm.input;




		sm.totalAmp = sm.totalAmp + sm.amps;
		sm.timeDiv++;
		sm.ampsPsec = sm.totalAmp/(sm.timeDiv*50);
		sm.voltDelta = (sm.ampsPsec*(time1[T1]/1000))*0.0270/(0.686*212);
		wait1Msec(10);
		positionPrev = positionCurrent;
	}

}
