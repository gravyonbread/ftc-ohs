#pragma config(Hubs,  S1, HTServo,  HTMotor,  HTMotor,  none)
#pragma config(Sensor, S1,     ,               sensorI2CMuxController)
#pragma config(Sensor, S2,     infrared,       sensorHiTechnicIRSeeker1200)
#pragma config(Sensor, S3,     weight,         sensorHiTechnicTouchMux)
#pragma config(Sensor, S4,     touch,          sensorTouch)
#pragma config(Motor,  motorA,          motor_popcorn, tmotorNXT, openLoop, reversed)
#pragma config(Motor,  motorB,          motor_B,       tmotorNXT, openLoop)
#pragma config(Motor,  motorC,          motor_C,       tmotorNXT, openLoop)
#pragma config(Motor,  mtr_S1_C2_1,     motor_L,       tmotorTetrix, PIDControl, reversed, encoder)
#pragma config(Motor,  mtr_S1_C2_2,     motor_R,       tmotorTetrix, PIDControl, encoder)
#pragma config(Motor,  mtr_S1_C3_1,     motor_lift,    tmotorTetrix, PIDControl, reversed, encoder)
#pragma config(Motor,  mtr_S1_C3_2,     motor_G,       tmotorTetrix, openLoop)
#pragma config(Servo,  srvo_S1_C1_1,    servo_IR,             tServoStandard)
#pragma config(Servo,  srvo_S1_C1_2,    servo_claw,           tServoStandard)
#pragma config(Servo,  srvo_S1_C1_3,    servo_ramp,           tServoStandard)
#pragma config(Servo,  srvo_S1_C1_4,    servo4,               tServoNone)
#pragma config(Servo,  srvo_S1_C1_5,    servo5,               tServoNone)
#pragma config(Servo,  srvo_S1_C1_6,    servo6,               tServoNone)
// Code generated by the 'ROBOTC' configuration wizard.
#include "JoystickDriver.c"
#include "hitechnic-irseeker-v2.h"
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

	Servo_SetSpeed(servo_IR, 10);	// maximum speed!
	Servo_SetSpeed(servo_claw, 10);	// maximum speed!

	Servo_Rotate(servo_IR, g_IRServoExtended);		// fold back up after start of tele-op
	Servo_Rotate(servo_claw, g_clawServoExtended);	// keep it straight out after tele-op


	Motor_SetMaxSpeed(g_FullMotorPower);

	Motor_ResetEncoder(motor_lift);

	nMotorEncoder[motor_lift] = 0;

	Time_Wait(10);

	return;
}



task main()
{
	initializeRobot();

	waitForStart();

	const int forwardTimeA = 70;
	const int turnTimeB = 80;
	const int forwardTimeC = 130;
	const int turnTimeD = 85;
	const int forwardTimeE = 30;
	const int liftTimeF = 20;

	Move_Forward	(forwardTimeA, g_AccurateMotorPower);
	Turn_Left		(turnTimeB, g_AccurateMotorPower, g_AccurateMotorPower);
	Move_Forward	(forwardTimeC, g_AccurateMotorPower);
	Turn_Left		(turnTimeD, g_AccurateMotorPower, g_AccurateMotorPower);
	Move_Forward	(forwardTimeE, g_AccurateMotorPower);
	Lift_Lift		(liftTimeF, g_AccurateMotorPower);
}