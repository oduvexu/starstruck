#include "utility.h"

char buffer_utility[64];

void init()
{
	// Set ARMSLAVE and ARMSLAVE2 to ARM motor values
	// so we only need to control ARM voltage.
	slaveMotor(ARMSLAVE, ARM);
	slaveMotor(ARMSLAVE2, ARM);

	initPID();

	addNewPID(ARM, 0.4, 0.25, 0.05, -0.3, 0, true, false, 100, -100, 10, 0);
	// addNewPID(CLAW, 0.3, 0.3, -0.2, -0.2, 0, true, false, 80, -100, 10, 0);

	// Clear both lines of LCD.
	clearLCDLine(0);
	clearLCDLine(1);

	bLCDBacklight = true;

	clearDebugStream();
}









void moveMotorsToTarget(int target, MovementMode mode, bool debug)
{
	float ramp_duration = 1; // How long to ramp up in seconds

	int distance_threshold = 500; // How close do we get to the target (in ticks) before we slow down

	float initial_effort = 20;

	float max_effort = 90;

	int wheels[4] = {FL, FR, BR, BL};
	int neg[4] = {1, 1, 1, 1};


	for (int i = 0; i < 4; i++)
	{
		int motor_id = wheels[i];
		nMotorEncoder[motor_id] = 0;
	}

	float t = 0;

	while (true)
	{
		// Get average encoder value (absolute relative to progress
		float progress = 0;

		for (int i = 0; i < 4; i++)
		{
			int motor_id = wheels[i];
			int encoder = nMotorEncoder[motor_id];
			progress += encoder*neg[i];
		}
		progress /= 4;

		sprintf(buffer_utility, "Progress: %f\n", progress);
		writeDebugStream(buffer_utility);

		int effort = 0;

		if (t < ramp_duration)
		{
			effort = initial_effort + t/ramp_duration * (max_effort - initial_effort);
		}
		else if (target - progress < distance_threshold)
		{
			effort = max_effort - (1 - (target - progress)/distance_threshold) * (max_effort - initial_effort);
		}
		else if (progress > target)
		{
			break;
		}
		else
		{
			effort = max_effort;
		}

		for (int i = 0; i < 4; i++)
		{
			int motor_id = wheels[i];
			motor[motor_id] = neg[i]*max_effort;
		}

		wait1Msec(10);
		t += .01;
	}
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

int tock = -1;
int effort_minus = 0;
bool fixed = false;
int hold_position = 0;
int saved_effort = 0;

void setFixed(bool _fixed)
{
	fixed = _fixed;
	effort_minus = 0;
}

void applyAllPID()
{
	tock++;

	if (tock > 1)
	{
		tock = 0;
	}

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

			if (velocity == 0 && effort_minus == 0)
			{
				hold_position = -nMotorEncoder[ARM];
			}

			if (velocity == 0 && tock == 0 && !fixed)
			{
				effort_minus += 2;
			}

			if (effort_minus > 0 && -nMotorEncoder[ARM] < hold_position && !fixed)
			{
				sprintf(buffer_utility, "Lowest Effort: %d\n", effort - effort_minus + 1);
				writeDebugStream(buffer_utility);

				effort_minus *= 1.3;

				saved_effort = effort - effort_minus;

				if (-nMotorEncoder[ARM] < 100)
				{
					saved_effort = 0;
				}
				else if (saved_effort < 7)
				{
					saved_effort = 7;
				}

				sprintf(buffer_utility, "Current Effort: %d\n", effort - effort_minus);
				writeDebugStream(buffer_utility);

				sprintf(buffer_utility, "Fixed set to true");
				writeDebugStream(buffer_utility);

				fixed = true;
			}




			if (effort - effort_minus > 0 && effort != 0)
			{
				if (!fixed)
				{
					motor[id] = effort - effort_minus;
				}
				else
				{
					motor[id] = saved_effort;
				}
			}
			else
			{
				motor[id] = effort;
			}

			if (p->debug)
			{
				sprintf(buffer_utility, "Cur: %d   Tar: %d   Eff: %d  Int: %f\n", p->encoder, p->encoder_target, effort, p->integral_error);
				writeDebugStream(buffer_utility);
			}
		}
	}
}
