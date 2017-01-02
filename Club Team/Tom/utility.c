#include "utility.h"

char buffer_utility[64];

void init()
{
	// Set ARMSLAVE and ARMSLAVE2 to ARM motor values
	// so we only need to control ARM voltage.
	slaveMotor(ARMSLAVE, ARM);
	slaveMotor(ARMSLAVE2, ARM);

	initPID();

	addNewPID(ARM, 0.6, 0.1, 0.05, -0.35, 0, true, false, 90, -90, 10, 0);
	addNewPID(CLAW, 0.6, 0.6, -0.2, -0.2, 0, true, false, 40, -100, 10, 0);

	float pk = 0.4;
	float pd = -0.6;
	float ki = 0.01;

	float error_deadzone = 50;

	int lim = 80;

//	addNewPID(FL, pk, pk, pd, pd, ki, false, false, lim, -lim, 20, error_deadzone);
//	addNewPID(FR, pk, pk, pd, pd, ki, false, false, lim, -lim, 20, error_deadzone);
//	addNewPID(BR, pk, pk, pd, pd, ki, false, false, lim, -lim, 20, error_deadzone);
//	addNewPID(BL, pk, pk, pd, pd, ki, false, false, lim, -lim, 20, error_deadzone);



	// Clear both lines of LCD.
	clearLCDLine(0);
	clearLCDLine(1);

	bLCDBacklight = true;
}



void resetWheelEncoders()
{
	int wheels[] = {FL, FR, BR, BL};
	int motor_id = 0;

	for (int i = 0; i < 4; i++)
	{
		motor_id = wheels[i];
		nMotorEncoder[motor_id] = 0;
		pid_list[motor_id].encoder_target = 0;
	}
}

// Returns whether or not the PID controllers are
bool wheelsActive()
{
	int wheels[] = {FL, FR, BR, BL};
	bool active = false;
	int motor_id = 0;

	for (int i = 0; i < 4; i++)
	{
		motor_id = wheels[i];
		active |= pid_list[motor_id].active;
	}

	return active;
}



void forward(float length)
{
	// ticks needed to go 1 tile
	float length_constant = 904;

	pid_list[FL].encoder_target += length * length_constant;
	pid_list[FR].encoder_target += length * length_constant;
	pid_list[BR].encoder_target += length * length_constant;
	pid_list[BL].encoder_target += length * length_constant;

	pid_list[FL].active = true;
}


void side(float length)
{
	// ticks needed to move 1 tile left
	float length_constant = 931;

	pid_list[FL].encoder_target += - length * length_constant;
	pid_list[FR].encoder_target += length * length_constant;
	pid_list[BR].encoder_target += - length * length_constant;
	pid_list[BL].encoder_target += length * length_constant;

	pid_list[FL].active = true;
}


void rotate(float angle)
{
	// ticks needed to go 360 deg
	float rotation_constant = 3880;

	pid_list[FL].encoder_target += angle/360.0 * rotation_constant;
	pid_list[FR].encoder_target += - angle/360.0 * rotation_constant;
	pid_list[BR].encoder_target += - angle/360.0 * rotation_constant;
	pid_list[BL].encoder_target += angle/360.0 * rotation_constant;

	pid_list[FL].active = true;
}

void raiseArm()
{
	pid_list[ARM].encoder_target = 800;
}

void highFenceArm()
{
	pid_list[ARM].encoder_target = 580;
}

void lowFenceArm()
{
	pid_list[ARM].encoder_target = 560;
}

void dropArm()
{
	pid_list[ARM].encoder_target = 0;
}



















void initPID()
{
	for (int i = 0; i < NUMBER_OF_PID_SLOTS; i++)
	{
		Pid *p = &pid_list[i];
		p->initialized = false;
		p->active = false;
	}
}

void addNewPID(int motor_id, float kp, float kp_down, float kd, float kd_down, float ki, bool reversed, bool debug, int limit, int reverse_limit, int deadzone, int error_deadzone)
{
	nMotorEncoder[motor_id] = 0;

	pid_list[motor_id].motor_id = motor_id;
	pid_list[motor_id].kp = kp;
	pid_list[motor_id].kp_down = kp_down;
	pid_list[motor_id].kd = kd;
	pid_list[motor_id].kd_down = kd_down;

	pid_list[motor_id].ki = ki;

	pid_list[motor_id].integral_error = 0;

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

	pid_list[motor_id].limit = limit;
	pid_list[motor_id].reverse_limit = reverse_limit;
	pid_list[motor_id].deadzone = deadzone;
	pid_list[motor_id].error_deadzone = error_deadzone;

	pid_list[motor_id].timer = 0.0;

	pid_list[motor_id].active = false;
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
				effort = round( error*p->kp + velocity*p->kd + p->integral_error);
			}
			else if (p->encoder_target < p->encoder)
			{
				effort = round( error*p->kp_down + velocity*p->kd_down + p->integral_error);
			}

			if (abs(effort) < p->deadzone)
			{
				effort = 0;
			}

			if (effort < p->reverse_limit)
			{
				effort = p->reverse_limit;
			}
			else if (effort > p->limit)
			{
				effort = p->limit;
			}

			// Integral error term.
			if (abs(error) < 200)
			{
				p->integral_error += p->ki * error;
			}


			// Error deadzone makes motors ignore error below threshold.
			// Error deadzone of zero means functionality is off.

			if (abs(error) < p->error_deadzone)
			{
				p->timer += .01; // Of a second

				if (p->timer > 0.5)
				{
					p->integral_error = 0;
					effort = 0;

					p->active = false;
				}
			}
			else
			{
				p->timer = 0;
			}

			if (effort != 0)
			{
				p->active = true;
			}

			motor[id] = effort;

			if (p->debug)
			{
				sprintf(buffer_utility, "Cur: %d   Tar: %d   Eff: %d  Int: %f\n", p->encoder, p->encoder_target, effort, p->integral_error);
				writeDebugStream(buffer_utility);
			}
		}
	}
}
