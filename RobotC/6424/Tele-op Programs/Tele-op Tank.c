#pragma config(Hubs,  S1, HTServo,  HTMotor,  HTMotor,  none)
#pragma config(Sensor, S1,     ,               sensorI2CMuxController)
#pragma config(Sensor, S2,     infrared,       sensorHiTechnicIRSeeker1200)
#pragma config(Sensor, S3,     weight,         sensorLightInactive)
#pragma config(Sensor, S4,     touch,          sensorTouch)
#pragma config(Motor,  motorA,          motor_A,       tmotorNXT, openLoop)
#pragma config(Motor,  motorB,          motor_B,       tmotorNXT, openLoop)
#pragma config(Motor,  motorC,          motor_C,       tmotorNXT, openLoop)
#pragma config(Motor,  mtr_S1_C2_1,     motor_L,       tmotorTetrix, PIDControl, reversed, encoder)
#pragma config(Motor,  mtr_S1_C2_2,     motor_R,       tmotorTetrix, PIDControl, encoder)
#pragma config(Motor,  mtr_S1_C3_1,     motor_lift,    tmotorTetrix, PIDControl, encoder)
#pragma config(Motor,  mtr_S1_C3_2,     motor_G,       tmotorTetrix, openLoop)
#pragma config(Servo,  srvo_S1_C1_1,    servo_IR,             tServoStandard)
#pragma config(Servo,  srvo_S1_C1_2,    servo_force,          tServoStandard)
#pragma config(Servo,  srvo_S1_C1_3,    servo_ramp,           tServoStandard)
#pragma config(Servo,  srvo_S1_C1_4,    servo4,               tServoNone)
#pragma config(Servo,  srvo_S1_C1_5,    servo5,               tServoNone)
#pragma config(Servo,  srvo_S1_C1_6,    servo6,               tServoNone)
// Code generated by the 'ROBOTC' configuration wizard.
#include "JoystickDriver.c"
#include "typedefs.h"
#include "global vars.h"
#include "enums.h"
#include "structs.h"
#include "low-level functions.h"
#include "high-level functions.h"
#include "subroutines.h"



void initializeRobot()
{
	// Place code here to init servos to starting positions.
	// Sensors are config'ed and setup by RobotC (need to stabalize).
	// Also add any settings that need to be set (other than global
	// variables), such as max PID speed, servo update rate, etc.

	Servo_Rotate(servo_IR, 0);	// fold back up after start of tele-op

	Motor_SetMaxSpeed(g_FullMotorPower);

	return;
}



task main()
{
	initializeRobot();

	// These will be used later and are declared here to save from having to
	// declare them every single loop.
	int powerL = 0;
	int powerR = 0;
	int powerLift = 0;

	waitForStart();

	while (true)
	{
		// Currently does (at least) 7 checks and 3 assignments per loop.

		Joystick_UpdateData();



		// See if a direction is being pressed, then test for the direction.
		// This is inside an `if` statement to optimize speed (less checking).
		// `JoystickController` arguments are not passed to increase speed.

		if ( Joystick_Direction() != DIRECTION_NONE )
		{
			switch ( Joystick_Direction() )
			{

				// Operate lift at full power if F/B.
				case DIRECTION_F:
					Motor_SetPower(motor_lift, 100);
					Time_Wait(100);
					Motor_Stop(motor_lift);
					break;
				case DIRECTION_B:
					Motor_SetPower(motor_lift, -100);
					Time_Wait(100);
					Motor_Stop(motor_lift);
					break;

				case DIRECTION_L:
					sub_PutRingOn();
					break;
				case DIRECTION_R:
					sub_TakeRingOff();
					break;
			}
		}



		// See if a button (not masked) is being pressed, then react.
		// This is inside an `if` statement to optimize speed (less checking).

		// The argument to this first `if` statement is a masked version
		// of the "bitmap" of buttons directly from the controller.

		// Everything other than the buttons used are masked off, to increase
		// processing speed (possibly, just speculation). Reasoning:
		// `&` compares all bits of the variables, so we might as well mask
		// everything we won't need, in case something irrelevant is pressed.

		// A `0` value means no buttons (that we are testing for) are pressed.
		// Directly using the struct since this is the only possible time to
		// use it, and this is very low-level anyways.

		if( (g_ControllerMask & joystick.joy1_Buttons) != 0 )
		{

			// Buttons Y/B/A will control lift height.
			if ( Joystick_Button(BUTTON_Y)==true )
			{
				sub_LiftToTop();
			}
			if ( Joystick_Button(BUTTON_B)==true )
			{
				sub_LiftToMiddle();
			}
			if ( Joystick_Button(BUTTON_A)==true )
			{
				sub_LiftToBottom();
			}

			// If only X is pressed, weigh the ring.
			// If JOYR is pressed as well, deploy ramp.
			if ( Joystick_Button(BUTTON_X)==true )
			{
				if ( Joystick_Button(BUTTON_JOYR) == true )
				{
					sub_DeployRamp();
				}
				else
				{
					sub_WeighRings();
				}
			}

			// Buttons LT/RT will fine-tune the lift.
			if ( Joystick_Button(BUTTON_LT)==true )
			{
				Motor_SetPower(motor_lift, 100/g_FineTuneFactor);
				Time_Wait(100);
				Motor_Stop(motor_lift);
			}
			if ( Joystick_Button(BUTTON_RT)==true )
			{
				Motor_SetPower(motor_lift, -100/g_FineTuneFactor);
				Time_Wait(100);
				Motor_Stop(motor_lift);
			}
		}



		// L/R motor code. Only triggered when a joystick returns a
		// value greater than the "drive" threshold (`global vars.h`).

		// Logarithmic control probably won't be implemented anytime soon.
		// Also need to stop using the `joystick` struct and switch to the
		// encapsulated version (Joystick_Joystick(...)).

		// These should be zeroed after every loop. In the case that there
		// isn't input, the robot won't keep moving at the last speed it had.
		powerL = 0;
		powerR = 0;

		// Y-axis code:
		if ( 	abs(joystick.joy1_y1) > g_DriveThreshold ||
				abs(joystick.joy1_y2) > g_DriveThreshold )
		{
			powerL = Math_ToLogarithmic(joystick.joy1_y1);
			powerR = Math_ToLogarithmic(joystick.joy1_y2);
		}

		// Last check: if LB/RB is pressed, fine-tune the power level.
		if ( (Joystick_Button(BUTTON_LB)||Joystick_Button(BUTTON_RB)) ==1 )
		{
			powerL /= g_FineTuneFactor;
			powerR /= g_FineTuneFactor;
		}

		Motor_SetPower(motor_L, powerL);
		Motor_SetPower(motor_R, powerR);



		// Input from CONTROLLER_2 will be processed last. Very low priority.

		powerLift = 0;

		if ( abs(joystick.joy2_y1)>g_LiftThreshold )
		{
			powerLift = Math_ToLogarithmic(-1*joystick.joy2_y1);
		}

		if ( (	Joystick_Button(BUTTON_LB, CONTROLLER_2) ||
				Joystick_Button(BUTTON_RB, CONTROLLER_2)) ==1 )
		{
			powerLift /= g_FineTuneFactor;
		}

		Motor_SetPower(motor_lift, powerLift);


		// Flush the controller input buffer periodically (every 1/4 sec?)



	}
}
