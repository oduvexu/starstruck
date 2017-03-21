
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
} SmartMotor;

void init_SmartMotor(SmartMotor sm, int smotor)
{
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
sm.startVolts = 7.78;
sm.currentVolts = 0;
sm.effortVolts = 0;
sm.Ke = 0.0621; //For torque
sm.amps = 0;
sm.off = false;

}

void state_SmartMotor(SmartMotor sm){
sm.backEMF = sm.Ke * (getMotorVelocity(sm.smotor));

if(sm.effort == 0)
{
	sm.deltaEffort = 1;
}
else
{
	sm.effortVolts = (sm.startVolts/20)*(29-(88/sqrt(sm.effort)));


}

sm.amps = (sm.effortVolts-sm.backEMF)/sm.motorRes;

if(sm.amps <= 0 || vexRT[Ch3] < 20)
sm.deltaEffort = 0;

if((sm.targetAmp) < (sm.amps) && sm.deltaEffort > -127)
sm.deltaEffort = sm.deltaEffort - 5;

else if((sm.targetAmp) > (sm.amps) && sm.deltaEffort < 0)
sm.deltaEffort = sm.deltaEffort + 1;



if(!sm.off){
sm.effort = vexRT[Ch3] + sm.deltaEffort;
}

else{
motor[sm.smotor] = 0;
}

sm.voltDelta = (sm.ampsPsec*(time1[T1]/1000))*0.0270/(0.686*212);


}
