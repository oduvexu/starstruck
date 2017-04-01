#pragma config(I2C_Usage, I2C1, i2cSensors)
#pragma config(Sensor, I2C_1,  ,               sensorQuadEncoderOnI2CPort,    , AutoAssign )
#pragma config(Sensor, I2C_2,  ,               sensorQuadEncoderOnI2CPort,    , AutoAssign )
#pragma config(Motor,  port2,           TEST,          tmotorVex393HighSpeed_MC29, openLoop, encoderPort, I2C_1)
#pragma config(Motor,  port3,           TEST2,         tmotorVex393HighSpeed_MC29, openLoop, reversed, encoderPort, I2C_2)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

#include "Amp_Limiter_Func.c"


task one;
task two;


task main()
{

	init_SmartMotor(motorS[0], TEST, 2);
	init_SmartMotor(motorS[1], TEST2, 3);

	group_SmartMotor(0,motorS[0]);
	group_SmartMotor(0,motorS[1]);


	groupAmpsLimit[0] = 4;

	monitorGroup[0] = true;

	motorS[0].targetAmp = 2;
	motorS[1].targetAmp = 2;

	startTask(one);
	startTask(two);

	bool racing = false;
	bool _Btn8D = false;

	nMotorEncoder[TEST] = 0;
	nMotorEncoder[TEST2] = 0;

	while(1){

		int forward_channel = abs(vexRT[Ch3]) <= 15 ? 0 : vexRT[Ch3];
		int turn_channel = abs(vexRT[Ch1]) <= 15 ? 0 : vexRT[Ch1];

		int effort_L = 0;
		int effort_R = 0;

		if (vexRT[Btn8D])
		{
			if (!_Btn8D)
			{
				// racing = !racing;
				_Btn8D = true;

				nMotorEncoder[TEST] = 0;
				nMotorEncoder[TEST2] = 0;
			}
		}
		else
		{
			_Btn8D = false;
		}

		if (racing)
		{
			int race_effort = 0;
			effort_L = race_effort;
			effort_R = race_effort;
		}
		else
		{
			effort_L = 0;

			int vel_L = getMotorVelocity(TEST);
			int vel_R = getMotorVelocity(TEST2);

			// Linearly interpolate multiplier from 1 to .1 (RPM from 0 to 150)

			float initial_mul = 1;
			float final_mul = 0.4;

			float mul_L = initial_mul - (1.0 - final_mul) * vel_L/100.0;
			float mul_R = initial_mul - (1.0 - final_mul) * vel_R/100.0;

			if (mul_L < final_mul)
			{
				mul_L = final_mul;
			}

			if (mul_R < final_mul)
			{
				mul_R = final_mul;
			}

			effort_L = (forward_channel*1.5 + turn_channel*mul_L)/1.5;
			effort_R = (forward_channel*1.5 - turn_channel*mul_R)/1.5;
		}

		if (effort_L > 0)
		{
			int max_effort = getMotorVelocity(TEST)*50.0/150.0 + 40;
			if (effort_L > max_effort)
			{
				effort_L = max_effort;
			}
		}

		if (effort_R > 0)
		{
			int max_effort = getMotorVelocity(TEST2)*50.0/150.0 + 40;
			if (effort_R > max_effort)
			{
				effort_R = max_effort;
			}
		}

		if (effort_R > 90)
		{
			effort_R = 90;
		}

		if (effort_L > 90)
		{
			effort_L = 90;
		}

		float initial_delta = 0;
		float final_delta = 10;

		int vel_L = getMotorVelocity(TEST);
		int vel_R = getMotorVelocity(TEST2);

		float delta_L = initial_delta + final_delta * vel_L/100.0;
		float delta_R = initial_delta + final_delta * vel_R/100.0;

		if (delta_L > final_delta)
		{
			delta_L = final_delta;
		}

		if (delta_R > final_delta)
		{
			delta_R = final_delta;
		}

		if (nMotorEncoder[TEST] > nMotorEncoder[TEST2])
		{
			effort_R += delta_R;
		}
		else if (nMotorEncoder[TEST2] > nMotorEncoder[TEST])
		{
			effort_L += delta_L;
		}

		if (getMotorVelocity(TEST) > getMotorVelocity(TEST2))
		{
			effort_R += delta_R/2;
		}
		else if (getMotorVelocity(TEST2) > getMotorVelocity(TEST))
		{
			effort_L += delta_L/2;
		}

		motorS[TEST-1].input = effort_L * (127.0 - motorS[0].deltaEffort)/127.0;
		motorS[TEST2-1].input = effort_R * (127.0 - abs(motorS[0].deltaEffort)*(motorS[1].direction))/127.0;

		clearDebugStream();
		writeDebugStream("A Effort:%d\n",motorS[0].effort);
		writeDebugStream("A Effort Delta:%d\n",motorS[0].deltaEffort);
		writeDebugStream("B Effort:%d\n",motorS[1].effort);
		writeDebugStream("B Effort Delta:%d\n",motorS[1].deltaEffort);
		writeDebugStream("A GroupA:%f\n",groupAmps[0]);
		writeDebugStream("A GroupB:%f\n",groups[0][1]);
		writeDebugStream("BackEMF:%f\n",motorS[0].backEMF);
		writeDebugStream("Amp:%f\n",motorS[0].amps);

		wait1Msec(10);

	}

}

task one{
	start_SmartMotor(motorS[0]);
}

task two{
	start_SmartMotor(motorS[1]);
}
