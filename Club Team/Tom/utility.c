#include "utility.h"

char buffer_utility[64];

void init()
{
	// Set ARMSLAVE and ARMSLAVE2 to ARM motor values
	// so we only need to control ARM voltage.
	slaveMotor(ARMSLAVE, ARM);
	slaveMotor(ARMSLAVE2, ARM);
	nMotorEncoder[ARM] = 0;
	nMotorEncoder[CLAW] = 0;

	initPID();

	addNewPID(ARM, 0.35, 0.1, 0.05, -0.2, true, false);
	addNewPID(CLAW, 0.5, 0.5, 0, 0, true, true);


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

	bool reversed;
	bool debug;
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

void addNewPID(int motor_id, float kp, float kp_down, float kd, float kd_down, bool reversed, bool debug)
{
	pid_list[motor_id].motor_id = motor_id;
	pid_list[motor_id].kp = kp;
	pid_list[motor_id].kp_down = kp_down;
	pid_list[motor_id].kd = kd;
	pid_list[motor_id].kd_down = kd_down;

	int mul = 1;
	if (reversed)
	{
		mul = -1;
	}

	pid_list[motor_id].encoder = mul*nMotorEncoder[motor_id];
	pid_list[motor_id].encoder_target = 0;

	pid_list[motor_id].direction = 0;
	pid_list[motor_id].initialized = true;

	pid_list[motor_id].reversed = reversed;
	pid_list[motor_id].debug = debug;
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

			int mul = 1;
			if (p->reversed)
			{
				mul = -1;
			}

			int new_encoder = mul*nMotorEncoder[id];

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

			if (abs(error) < 10)
			{
				error = 0;
			}

			int lim = 60;

			if (effort < -lim)
			{
				effort = -lim;
			}
			else if (effort > lim)
			{
				effort = lim;
			}

			motor[id] = effort;

			if (p->debug)
			{
				sprintf(buffer_utility, "Cur: %d   Tar: %d   Eff: %d\n", p->encoder, p->encoder_target, effort);
				writeDebugStream(buffer_utility);
			}
		}
	}
}
