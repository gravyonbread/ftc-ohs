#ifndef LOW_LEVEL_FUNCTIONS_H
#define LOW_LEVEL_FUNCTIONS_H


/////////////////////////////
//    "namespace" Motor    //
/////////////////////////////

void Motor_Forward(tMotor motor_name, int power=75)
{
	motor[motor_name] = power;
}

void Motor_Reverse(tMotor motor_name, int power=75)
{
	motor[motor_name] = -1 * power;
}

void Motor_Stop(tMotor motor_name, bool brake=true)
{
	motor[motor_name] = 0;
	bFloatDuringInactiveMotorPWM = !(brake);
}

void Motor_ExactRotation(tMotor motor_name, int angle, int power=75, bool brake=true)
{
	//RotateMotor(port, power, angle);
	switch(brake)
	{
		case true:
			//Off(port);
			break;
		case false:
			//Coast(port);
			break;
	}
}

void Motor_GetRotation(tMotor motor_name)
{
	//MotorRotationCount(port);
}

void Motor_ResetRotation(tMotor motor_name, bool relative)
{
	switch(relative)
	{
		case true:
			//ResetBlockTachoCount(port);
			break;
		case false:
			//ResetRotationCount(port);
			break;
	}
}


/////////////////////////////
//    "namespace" Servo    //
/////////////////////////////


/////////////////////////////
//    "namespace" Sensor   //
/////////////////////////////


/////////////////////////////
//   "namespace" Joystick  //
/////////////////////////////

void Joystick_UpdateData()
{
	getJoystickSettings(joystick);
}


/////////////////////////////
//   "namespace" Buttons   //
/////////////////////////////


/////////////////////////////
//     "namespace" File    //
/////////////////////////////


/////////////////////////////
//     "namespace" Time    //
/////////////////////////////

//breaks down time to wait into 10ms and 1ms chunks
void Time_Wait(int ms)
{
	int waitTime10ms = (ms - (ms%320)) / 320;
	int waitTime1ms = (ms - (ms%32)) / 32;

	for (int i=0; i<waitTime10ms; i++)
	{
		wait10Msec(320);
	}
	for (int i=0; i<waitTime1ms; i++)
	{
		wait1Msec(32);
	}
}


/////////////////////////////
//    "namespace" Sound    //
/////////////////////////////


/////////////////////////////
//   "namespace" Display   //
/////////////////////////////


/////////////////////////////
//  "namespace" Bluetooth  //
/////////////////////////////


/////////////////////////////
//     "namespace" Misc    //
/////////////////////////////

#endif
