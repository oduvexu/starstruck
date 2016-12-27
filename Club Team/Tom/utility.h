#define NUMBER_OF_PID_SLOTS 15

void init();

void initPID();
void addNewPID(int motor_id, float kp, float kp_down, float kd, float kd_down);
void applyAllPID();
