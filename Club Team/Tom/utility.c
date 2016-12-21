#include "utility.h"

void init()
{
	// set ARMSLAVE and ARMSLAVE2 to ARM motor values
	// so we only need to control ARM voltage
	slaveMotor(ARMSLAVE, ARM);
	slaveMotor(ARMSLAVE2, ARM);

	// Clear both lines of LCD
	clearLCDLine(0);
	clearLCDLine(1);
}

/*

void forward(int x)
{
		motor[FL] = -x;
		motor[FR] = x;
		motor[BR] = x;
		motor[BL] = -x;
}

void backward(int x)
{
		motor[FL] = x;
		motor[FR] = -x;
		motor[BR] = -x;
		motor[BL] = x;
}

void sideLeft(int x)
{
		motor[FL] = x;
		motor[FR] = x;
		motor[BR] = -x;
		motor[BL] = -x;
}

*/
