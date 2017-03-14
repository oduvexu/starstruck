#pragma config(I2C_Usage, I2C1, i2cSensors)
#pragma config(Sensor, I2C_1,  M,              sensorQuadEncoderOnI2CPort,    , AutoAssign )
#pragma config(Motor,  port2,           M,             tmotorVex393HighSpeed_MC29, openLoop, encoderPort, I2C_1)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

// Defined by sizeof(vexRT)/4
#define NUM_OF_BUTTONS 22

bool _vexRT[NUM_OF_BUTTONS];

bool on = false;

void updateButtons()
{
	for (int i = 0; i < NUM_OF_BUTTONS; i++)
	{
		_vexRT[i] = (bool) vexRT[i];
	}
}

task main()
{
	while (true)
	{
		if (vexRT[Btn8D] && !_vexRT[Btn8D])
		{
			on = !on;

			if (on)
			{
				nMotorEncoder[M] = 0;
				datalogClear();
			}
		}

		if (on)
		{
			motor[M] = 20;
		}
		else
		{
			motor[M] = 0;
		}

		datalogDataGroupStart();
		datalogAddValue(0, motor[M]);
		datalogAddValue(1, nMotorEncoder[M]);
		datalogAddValue(2, getMotorVelocity(M));
		datalogDataGroupEnd();

		updateButtons();
		wait1Msec(10);
	}
}
