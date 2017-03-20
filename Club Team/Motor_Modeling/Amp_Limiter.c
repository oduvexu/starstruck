#pragma config(I2C_Usage, I2C1, i2cSensors)
#pragma config(Sensor, I2C_1,  ,               sensorQuadEncoderOnI2CPort,    , AutoAssign )
#pragma config(Motor,  port2,           TEST,          tmotorVex393_HBridge, openLoop, encoderPort, I2C_1)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

int effort = 0;
int deltaEffort = 0;
int targetAmp = 2;
float voltDelta = 0;
float totalAmp = 0;
float ampsPsec = 0;
float timeDiv = 0;
float motorRes = 2;
float backEMF = 0;
float startVolts = 7.78;
float currentVolts = 0;
float effortVolts = 0;
float Ke = 0.0621; //For torque
float amps = 0;
bool off = false;
task input();


task main()
{

clearTimer(T1);
startTask(input);

while(1){

backEMF = Ke * (getMotorVelocity(TEST));

if(effort == 0)
deltaEffort = 1;

else
effortVolts = (startVolts/20)*(29-(88/sqrt(effort)));

amps = (effortVolts-backEMF)/motorRes;



/*
else if((targetVelocity) < (getMotorVelocity(TEST)) && effort > 0)
deltaEffort = deltaEffort - 1;

else if((targetVelocity) > (getMotorVelocity(TEST)) && effort < 127)
deltaEffort = deltaEffort + 1;

else if((int)(targetVelocity - 10) < (int)(getMotorVelocity(TEST/10)) && effort > 0)
deltaEffort = deltaEffort - 15;

else if((int)(targetVelocity + 10) > (int)(getMotorVelocity(TEST)/10) && effort < 127)
deltaEffort = deltaEffort + 5;
*/

if(amps == 0)
deltaEffort = 1;

else if((targetAmp) < (amps) && effort > 0)
deltaEffort = deltaEffort - 1;

else if((targetAmp) > (amps) && effort < 127)
deltaEffort = deltaEffort + 1;

else if((int)(targetAmp - 1) < (int)(amps) && effort > 0)
deltaEffort = deltaEffort - 15;

else if((int)(targetAmp + 1) > (int)(amps/10) && effort < 127)
deltaEffort = deltaEffort + 5;

if(!off){
effort = deltaEffort;
motor[TEST] = effort;
}

else{
motor[TEST] = 0;
}

clearDebugStream();
writeDebugStream("Effort:%d\n",effort);
writeDebugStream("Target Amp:%d\n",targetAmp);
writeDebugStream("Amp:%f\n",amps);
writeDebugStream("Amp/s:%f\n",ampsPsec);
writeDebugStream("Current Volts/s:%f\n",currentVolts);
writeDebugStream("Delta Volts/s:%f\n",voltDelta);
writeDebugStream("sec:%d\n",time1[T1]/1000);
writeDebugStream("RPM:%d\n",getMotorVelocity(TEST));
writeDebugStream("Off:%d\n",off);

wait1Msec(100);

}
}

task input()
{

while(1){

/*
if(vexRT[Btn8D])
targetVelocity = 5 + targetVelocity;

else if(vexRT[Btn8U])
targetVelocity = targetVelocity - 5;
*/

voltDelta = (ampsPsec*(time1[T1]/1000))*0.0270/(0.686*212);

if(vexRT[Btn8L])
off = true;

else if(vexRT[Btn8R])
off = false;

totalAmp = amps + totalAmp;
++timeDiv;

ampsPsec = totalAmp/timeDiv;
currentVolts = startVolts - voltDelta;

wait1Msec(200);

}



}
