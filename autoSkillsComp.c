#pragma config(Sensor, in1,    lineLeft,       sensorLineFollower)
#pragma config(Sensor, in2,    lineCenter,     sensorLineFollower)
#pragma config(Sensor, in3,    lineRight,      sensorLineFollower)
#pragma config(Sensor, in4,    gyro,           sensorGyro)
#pragma config(Sensor, dgtl1,  encoderMogo,    sensorQuadEncoder)
#pragma config(Sensor, dgtl3,  encoderRight,   sensorQuadEncoder)
#pragma config(Sensor, dgtl5,  encoderLeft,    sensorQuadEncoder)
#pragma config(Motor,  port2,           backR,         tmotorVex393TurboSpeed_MC29, openLoop, reversed)
#pragma config(Motor,  port3,           backL,         tmotorVex393TurboSpeed_MC29, openLoop)
#pragma config(Motor,  port4,           tipR,          tmotorVex393HighSpeed_MC29, openLoop)
#pragma config(Motor,  port5,           mogoR,         tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port6,           mogoL,         tmotorVex393_MC29, openLoop, reversed)
#pragma config(Motor,  port7,           tipL,          tmotorVex393HighSpeed_MC29, openLoop)
#pragma config(Motor,  port8,           frontL,        tmotorVex393TurboSpeed_MC29, openLoop)
#pragma config(Motor,  port9,           frontR,        tmotorVex393TurboSpeed_MC29, openLoop, reversed)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

/*---------------------------------------------------------------------------*/
/*                                                                           */
/*        Description: Competition template for VEX EDR                      */
/*                                                                           */
/*---------------------------------------------------------------------------*/

// This code is for the VEX cortex platform
#pragma platform(VEX2)

// Select Download method as "competition"
#pragma competitionControl(Competition)

//Main competition background code...do not modify!
#include "Vex_Competition_Includes.c"

/*---------------------------------------------------------------------------*/
/*                          Pre-Autonomous Functions                         */
/*                                                                           */
/*  You may want to perform some actions before the competition starts.      */
/*  Do them in the following function.  You must return from this function   */
/*  or the autonomous and usercontrol tasks will not be started.  This       */
/*  function is only called once after the cortex has been powered on and    */
/*  not every time that the robot is disabled.                               */
/*---------------------------------------------------------------------------*/

int front = 1;
int back  = -1;

/* Variables used to calculate how fast bot is moving*/
float wheelDiameter = 4.5;
float circumference = wheelDiameter * PI; //Roughly 14.13 inches

int constant = 1;

float distanceMogo1 = (48/circumference) * 360; //Distance for the first mogo
float distanceTest = (10/circumference) * 360;

int timeMogoIntake = 65; //number of ticks for the mogo intake to go down
int g = 0;
int motorEncoderTest = 0;

/*/////////////////////////////*/
/*       Reset Functions       */
/*/////////////////////////////*/

void resetMogo()
{
	motor[mogoL] = 0;
	motor[mogoR] = 0;
}

void resetDrive()
{
	motor[frontL] = 0;
	motor[backL] = 0;
	motor[frontR] = 0;
	motor[backR] = 0;
}

void resetDriveEncoder()
{
	SensorValue[encoderLeft] = 0;
	SensorValue[encoderRight] = 0;
}

void resetMogoEncoder()
{
	SensorValue[encoderMogo] = 0;
}

/*/////////////////////////////*/
/*    Basic Drive Functions    */
/*/////////////////////////////*/
void driveAlto(float distanceInInches, int spd)//distance controls direction not spd
{
	float leftSpd = spd;
	float rightSpd = spd;
	float inverter = 1.0;
	if(distanceInInches < 0.0)
	{
		inverter*=-1.0;
	}
	float wheelRadius = 2.0;
	float cir = wheelRadius*2*PI;
	float degree = ((distanceInInches/cir) * 360.0 *0.5);

	SensorValue[encoderRight] = 0;
	SensorValue[encoderLeft] = 0;
	while(abs(degree) > abs(SensorValue[encoderRight]) && abs(degree) > abs(SensorValue[encoderLeft]))
	{
		rightSpd = spd;
		leftSpd = spd;
		//uhhhhh idk if this works but whatever
		if(leftSpd > 125.0){
			leftSpd = 125.0;
		}
		else if(leftSpd < -125.0)
		{
			leftSpd = -125.0;
		}
		if(rightSpd > 125.0){
			rightSpd = 125.0;
		}
		else if(rightSpd < -125.0)
		{
			rightSpd = -125.0;
		}
		motor[backL] = leftSpd * inverter;
		motor[backR] = rightSpd * inverter;
		motor[frontL] = leftSpd * inverter;
		motor[frontR] = rightSpd * inverter;
	}
	motor[backL] = -(leftSpd * inverter) / 3;
	motor[backR] = -(rightSpd * inverter) / 3;
	motor[frontL] = -(leftSpd * inverter) / 3;
	motor[frontR] = -(rightSpd * inverter) / 3;
	wait1Msec(150);
	motor[backL] = 0;
	motor[backR] = 0;
	motor[frontR] = 0;
	motor[frontL] = 0;
}

void move(int power, int distance)
{
	resetDriveEncoder();
	distance = distance * constant;
	while(abs(SensorValue[encoderLeft]) < distance)
	{
		motor[frontL] = power;
		motor[backL] = power;
		motor[frontR] = power;
		motor[backR] = power;

	}
	resetDrive();
}
void turn(int bearing)//assume positive is right and negative is left
{
	//bearing*=(4/5);
	bearing = (bearing / 10) * 9;
	int currentBearing = SensorValue[gyro]/10;
	int targetBearing = bearing + currentBearing;
	int TURN_SLOWDOWN = 10;

	if(targetBearing > 0 && targetBearing > 360)
	{
		targetBearing = targetBearing - 360;
	}
	else if(targetBearing < 0 && targetBearing < -360)
	{
		targetBearing = targetBearing + 360;
	}

	//float gyr = SensorValue[Gyro]/10;


	//MAKE TURNING ACCURATE BY MAKING REVERSE FOR 80ms


	if (targetBearing < SensorValue[gyro]/10) // Left turn
	{
		while (SensorValue[gyro]/10 > targetBearing + TURN_SLOWDOWN)
		{
			motor[frontL] = -100;
			motor[backL] = -100;
			motor[frontR] = 100;
			motor[backR] = 100;
			//gyr = SensorValue[Gyro]/10;
		}
		while (SensorValue[gyro]/10 > targetBearing)
		{
			motor[frontL] = -50;
			motor[backL] = -50;
			motor[frontR] = 50;
			motor[backR] = 50;
			//gyr = SensorValue[Gyro]/10;
		}

		motor[frontL] = 50;
		motor[backL] = 50;
		motor[frontR] = -50;
		motor[backR] = -50;
		wait1Msec(200);

		resetDrive();
	}
	else // Right turn
	{
		while (SensorValue[gyro]/10 < targetBearing - TURN_SLOWDOWN)
		{
			motor[frontL] = 100;
			motor[backL] = 100;
			motor[frontR] = -100;
			motor[backR] = -100;
			//gyr = SensorValue[Gyro]/10;
		}
		while (SensorValue[gyro]/10 < targetBearing)
		{
			motor[frontL] = 50;
			motor[backL] = 50;
			motor[frontR] = -50;
			motor[backR] = -50;
			//gyr = SensorValue[gyro]/10;
		}
		motor[frontL] = -50;
		motor[backR] = -50;
		motor[frontR] = 50;
		motor[backR] = 50;

		wait1Msec(200);

		resetDrive();
	}
}


void mogo(int choice)
{
	resetMogoEncoder();

	//lower mogo intake
	if(choice == front)
	{
		while(abs(SensorValue[encoderMogo]) <= 95)
		{
			motor[mogoL] = 127;
			motor[mogoR] = 127;
		}
	}
	//raise mogo intake
	else if(choice == back)
	{
		while(abs(SensorValue[encoderMogo]) <= 95)
		{
			motor[mogoL] = -121;
			motor[mogoR] = -121;
		}
	}
	resetMogo();
}



/*/////////////////////////////*/
/*    Placing in Point Zones   */
/*/////////////////////////////*/



void pre_auton()
{
	// Set bStopTasksBetweenModes to false if you want to keep user created tasks
	// running between Autonomous and Driver controlled modes. You will need to
	// manage all user created tasks if set to false.
	bStopTasksBetweenModes = true;

	resetDriveEncoder();
	resetMogoEncoder();

	SensorType[gyro] = sensorNone;
	wait1Msec(500);
	SensorType[gyro] = sensorGyro;
	wait1Msec(1100);

	// Set bDisplayCompetitionStatusOnLcd to false if you don't want the LCD
	// used by the competition include file, for example, you might want
	// to display your team name on the LCD in this function.
	// bDisplayCompetitionStatusOnLcd = false;

	// All activities that occur before the competition starts
	// Example: clearing encoders, setting servo positions, ...
}

/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                              Autonomous Task                              */
/*                                                                           */
/*  This task is used to control your robot during the autonomous phase of   */
/*  a VEX Competition.                                                       */
/*                                                                           */
/*  You must modify the code to add your own robot specific commands here.   */
/*---------------------------------------------------------------------------*/

task autonomous()
{
	// ..........................................................................
	// Insert user code here.
	// ..........................................................................

	// Remove this function call once you have "real" code.
	//AutonomousCodePlaceholderForTesting();



	//127 sent to mogo intake (lower it)
	mogo(front);

	//moves forward to mogo
	driveAlto(62, 121);

	//-127 sent to mogo intake (raise it)
	mogo(back);
	wait1Msec(200);
	driveAlto(-48, 121);

	turn(45);
	driveAlto(-22, 121);

	turn(90);
	driveAlto(30, 121);
	wait1Msec(200);
	motor[frontL] = 50;
	motor[backR] = 50;
	motor[frontR] = 50;
	motor[backR] = 50;
	mogo(front);
	wait1Msec(200);
	driveAlto(-7, 121);
	mogo(back);
	wait1Msec(200);
	driveAlto(-25, 121);

	turn(87);
	driveAlto(-50,121);
	mogo(front);
	wait1Msec(200);
	driveAlto(32, 121);

	turn(-135);
	driveAlto(45, 121);
	mogo(front);
	driveAlto(-9, 121);
	turn(90);

}

/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                              User Control Task                            */
/*                                                                           */
/*  This task is used to control your robot during the user control phase of */
/*  a VEX Competition.                                                       */
/*                                                                           */
/*  You must modify the code to add your own robot specific commands here.   */
/*---------------------------------------------------------------------------*/


int threshold = 8;

int channel1;
int channel2;
int channel3;
int channel4;

void deadZoneCheck()
{
	channel1 = (abs(vexRT[Ch1]) > threshold) ? vexRT[Ch1] : 0;
	channel2 = (abs(vexRT[Ch2]) > threshold) ? vexRT[Ch2] : 0;
	channel3 = (abs(vexRT[Ch3]) > threshold) ? vexRT[Ch3] : 0;
	channel4 = (abs(vexRT[Ch4]) > threshold) ? vexRT[Ch4] : 0;

	/*if (abs(vexRT[Ch1]) > threshold)
	channel1 = vexRT[Ch1];
	else
	channel1 = 0;
	if (abs(vexRT[Ch2]) > threshold)
	channel2 = vexRT[Ch2];
	else
	channel2 = 0;
	if(abs(vexRT[Ch3]) > threshold)
	channel3 = vexRT[Ch3];
	else
	channel3 = 0;
	if(abs(vexRT[Ch4]) > threshold)
	channel4 = vexRT[Ch4];
	else
	channel4 = 0;*/
}



task usercontrol()
{
	// User control code here, inside the loop
	while (true)
	{
		g = SensorValue[gyro];
		motorEncoderTest = SensorValue[encoderMogo];
		// This is the main execution loop for the user control program.
		// Each time through the loop your program should update motor + servo
		// values based on feedback from the joysticks.

		// ........................................................................
		// Insert user code here. This is where you use the joystick values to
		// update your motors, etc.
		// ........................................................................

		// Remove this function call once you have "real" code.
		//UserControlCodePlaceholderForTesting();

		deadZoneCheck();

		//drive motors
		motor[frontL] = channel3;
		motor[backL] = channel3;
		motor[frontR] = channel2;
		motor[backR] = channel2;

		//mobile lift goes up on button 6U
		if (vexRT[Btn6U] == 1)
		{
			motor[mogoR] = -100;
			motor[mogoL] = -100;

		}
		//mobile lift goes down on button 6D
		else if (vexRT[Btn6D] == 1)
		{
			motor[mogoL] = 100;
			motor[mogoR] = 100;
		}
		else
		{
			motor[mogoL] = 0;
			motor[mogoR] = 0;
		}

		if(vexRT[Btn8D] == 1)
		{
			motor[tipL] = -23;
			motor[tipR] = -23;
		}


		if (vexRT[Btn5U] == 1)
		{
			motor[tipL] = -80;
			motor[tipR] = -80;

		}
		//tipper goes down on button 5D
		else if (vexRT[Btn5D] == 1)
		{
			motor[tipL] = 80;
			motor[tipR] = 80;
		}
		else
		{
			motor[tipL] = 0;
			motor[tipR] = 0;
		}
	}
}
