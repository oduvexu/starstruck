#define NUMBER_OF_PID_SLOTS 15
#define NUMBER_OF_BUTTON_SLOTS 32


void init();




void forward(float length);
void side(float length);
void rotate(float angle);


void raiseArm();
void highFenceArm();
void lowFenceArm();
void dropArm();



void initPID();
void addNewPID(int motor_id, float kp, float kp_down, float kd, float kd_down, float ki, bool reversed, bool debug, int limit, int reverse_limit, int deadzone, int error_deadzone);
void applyAllPID();






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

	float ki;

	bool reversed;
	bool debug;
	int limit;
	int reverse_limit;
	int deadzone;
	int error_deadzone;

	float timer;

	float integral_error;

	bool active;
} Pid;

Pid pid_list[NUMBER_OF_PID_SLOTS];


// Previous values of controller
int vexRT_P[NUMBER_OF_BUTTON_SLOTS];
