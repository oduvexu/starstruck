#include "utility.h"

void init()
{
	// Set ARMSLAVE and ARMSLAVE2 to ARM motor values
	// so we only need to control ARM voltage.
	slaveMotor(ARMSLAVE, ARM);
	slaveMotor(ARMSLAVE2, ARM);
	nMotorEncoder[ARM] = 0;

	initPID();

	addNewPID(ARM, 0.35, 0.1, 0.05, -0.2);



	// Clear both lines of LCD.
	clearLCDLine(0);
	clearLCDLine(1);

	bLCDBacklight = true;
}


typedef struct
{
	bool initialized;

	int motor_id;
	int encoder;
	int encoder_target;
	int direction;

	float kp;
	float kp_down;
	float kd;
	float kd_down;
} Pid;

Pid pid_list[15];

void initPID()
{
	for (int i = 0; i < NUMBER_OF_PID_SLOTS; i++)
	{
		Pid *p = &pid_list[i];
		p->initialized = false;
	}
}

void addNewPID(int motor_id, float kp, float kp_down, float kd, float kd_down)
{
	pid_list[motor_id].motor_id = motor_id;
	pid_list[motor_id].kp = kp;
	pid_list[motor_id].kp_down = kp_down;
	pid_list[motor_id].kd = kd;
	pid_list[motor_id].kd_down = kd_down;

	pid_list[motor_id].encoder = -nMotorEncoder[motor_id];
	pid_list[motor_id].encoder_target = 0;

	pid_list[motor_id].direction = 0;
	pid_list[motor_id].initialized = true;
}

void applyAllPID()
{
	for (int i = 0; i < NUMBER_OF_PID_SLOTS; i++)
	{
		Pid *p = &pid_list[i];

		if (p->initialized)
		{
			int id = p->motor_id;
			int target = p->encoder_target;
			int new_encoder = -nMotorEncoder[id];

			if (new_encoder > p->encoder)
			{
				p->direction = 1;
			}
			else if (new_encoder < p->encoder)
			{
				p->direction = -1;
			}

			p->encoder = new_encoder;


			float error = target - p->encoder;

			float velocity = getMotorVelocity(id) * p->direction;

			int effort = 0;

			if (p->encoder_target >= p->encoder)
			{
				effort = round( error*p->kp + velocity*p->kd);
			}
			else if (p->encoder_target < p->encoder)
			{
				effort = round( error*p->kp_down + velocity*p->kd_down);
			}

			motor[id] = effort;
		}
	}
}
