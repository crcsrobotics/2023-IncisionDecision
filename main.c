#pragma config(Sensor, dgtl1,  stopSensor,     sensorDigitalIn)
#pragma config(Sensor, dgtl3,  leftSensor,     sensorDigitalIn)
#pragma config(Sensor, dgtl4,  rightSensor,    sensorDigitalIn)
#pragma config(Sensor, dgtl6,  upperLimitSensor, sensorDigitalIn)
#pragma config(Sensor, dgtl7,  lowerLimitSensor, sensorDigitalIn)
#pragma config(Motor,  port2,           motorLeft,     tmotorServoContinuousRotation, openLoop, reversed)
#pragma config(Motor,  port3,           rake,          tmotorServoStandard, openLoop)
#pragma config(Motor,  port4,           autobox,       tmotorServoContinuousRotation, openLoop, reversed)
#pragma config(Motor,  port7,           finger,        tmotorServoStandard, openLoop)
#pragma config(Motor,  port8,           sizeChange,    tmotorServoStandard, openLoop)
#pragma config(Motor,  port9,           motorRight,    tmotorServoContinuousRotation, openLoop)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

// Stephen will prevail
// It'd be really funny if we kept testing the IR kit on the field and it works and then the second the competition starts it just decides to not i am going to end everything brother pLEADR eork

/*
Control List

DRIVE CONTROLS
-------------------
Ch3 - Left motor
Ch2 - Right motor

VERTICAL CONTROLS
-------------------
Btn5U - Autobox up*
Btn5D - Autobox down*

PART CONTROLS
-------------------
Btn8L - Extend to full size (Exit compliance size)
Btn8D - Toggle Rake
Btn8R - Toggle Finger Positions
Btn8U - Autobox Calibrate

MODE CONTROLS
-------------------
Btn7R - Start autonomous brain biopsy
Btn7D - Cancel autonomous brain biopsy
Btn7L - Slow Mode
Btn7U - Reverse Mode

*may need to be updated

*/

// Variables are defined outside of the main task to make them global.
// When variables are global, they can be viewed in the robot debug tools.
// Debugger tools can be found under the "Robot" menu when the robot is connected.

bool autonomous = false;

// These variables are used to create toggle buttons.
bool lastpress7L = false;
bool lastpress7U = false;
bool lastpress8D = false;
bool lastpress8L = false;
bool lastpress8R = false;
bool rakeUp = false;
bool sizeLowered = false;

// Current finger position and list of position values
int fingerPosition = 0;
int fingerPositions[] = {-127, 5, 30, -60};


// Set to true if the floor is too dark to reflect the IR sensors.
bool floorTooDark = true;

// Smooth driving enabled
bool smoothDriving = false;
float currentCh2speed = 0;
float currentCh3speed = 0;


// Modifiers used for slow mode and reverse mode
float speedModifier = 1.0;
int directionModifier = 1;

// Adjusted Joystick Values (after modifiers and deadzone)
int Ch2Adjusted = 0;
int Ch3Adjusted = 0;

// Not implemented yet - Could be used for software limits.
// Testing so far has found them to be unreliable, however.
int limitAutobox = 0;

int autoboxPosition = 0;
int autoboxMaxPosition = 0;
int autoboxMidPoint = 0;
bool calibrated = false;
bool centering = false;


bool isWiggle = false;
bool rightSense = false;
bool leftSense = false;

task main(){

	// Move servo to fit in competition dimensions
	motor[sizeChange]=-127;
	motor[finger]=-127;

	while (true){
		// Autonomous brain biopsy code
		if (autonomous){

			if(vexRT[Btn7D]){
				// Cancel autonomous
				autonomous = false;
			}

			// Backup code for if the floor is too dark to reflect IR.
			if (floorTooDark){
				// Motors drive forward unless they see the white line, in which case they reverse to correct their direction.
				leftSense = false;
				rightSense = false;

				if (SensorValue[leftSensor] == 1){
					motor[motorLeft] = 40;
				}
				else {
					leftSense = true;
					motor[motorLeft] = -55;
				}

				if (SensorValue[rightSensor] == 1){
					motor[motorRight] = 40;
				}
				else {
					rightSense = true;
					motor[motorRight] = -55;
				}
				if (leftSense && rightSense){
					motor[motorRight] = 40;
					motor[motorLeft] = 40;
					wait1Msec(15);
				}
				wait1Msec(15);
			}

			// Code for when the floor is light enough to reflect IR.
			else{
				// Motors drive forward unless they see a black line, in which case they reverse to correct their direction.

				if (SensorValue[leftSensor] == 0){
					motor[motorLeft] = 40;
				}
				else {
					motor[motorLeft] = -40;
				}

				if (SensorValue[rightSensor] == 0){
					motor[motorRight] = 40;
				}
				else {
					motor[motorRight] = -40;
				}
			}

			// Stop sensor (no longer implemented)
			/*if (SensorValue[stopSensor] == 0){
			autonomous = false;
			}*/

		}
		// Manual Controls
		else{

			// Start autonomous task
			if(vexRT[Btn7R]){
				autonomous = true;
			}

			/* if (vexRT[Btn8U]){
				if (!calibrated){
					// Calibration
					while (SensorValue[upperLimitSensor] == 0){
						motor[autobox]=50;
					}
					autoboxPosition = 0;
					while (SensorValue[lowerLimitSensor] == 0){
						autoboxPosition += 1;
						motor[autobox]=-50;
						wait1Msec(50);
					}
					autoboxMaxPosition = autoboxPosition;
					autoboxMidPoint = autoboxMaxPosition/2;
					while (SensorValue[upperLimitSensor] == 0){
						motor[autobox]=50;
					}
					autoboxPosition = 0;
				}
				else {
					centering = true;
				}
			}

			if (centering){
				if(autoboxPosition < autoboxMidPoint){
					motor[autobox]=50;
					autoboxPosition -= 1;
					wait1Msec(50);
				}
				else if(autoboxPosition > autoboxMidPoint){
					motor[autobox]=-50;
					autoboxPosition += 1;
					wait1Msec(50);
				}
				else{
					motor[autobox]=0;
					centering=false;
				}
			}
			*/


			// Add controller deadzone to prevent controller drift issues and apply speed and direction modifiers.
			if (vexRT[Ch2] > 10 || vexRT[Ch2] < -10){
				Ch2Adjusted = vexRT[Ch2] * speedModifier * directionModifier;
			}
			else{
				Ch2Adjusted = 0;
			}

			if (vexRT[Ch3] > 10 || vexRT[Ch3] < -10){
				Ch3Adjusted = vexRT[Ch3] * speedModifier * directionModifier;
			}
			else{
				Ch3Adjusted = 0;
			}
			if (smoothDriving){
				// Instead of directly changing the speed, move values towards the target speed.
				if (currentCh2speed > Ch2Adjusted){
					currentCh2speed -= 0.04;
				}
				if (currentCh2speed < Ch2Adjusted){
					currentCh2speed += 0.04;
				}
				if (currentCh3speed > Ch3Adjusted){
					currentCh3speed -= 0.04;
				}
				if (currentCh3speed < Ch3Adjusted){
					currentCh3speed += 0.04;
				}
				if (directionModifier > 0){
					// Regular direction controls
					motor[motorLeft]=currentCh3speed;
					motor[motorRight]=currentCh2speed;
				}
				else{
					// Reverse direction controls (joysticks flipped)
					motor[motorLeft]=currentCh2speed;
					motor[motorRight]=currentCh3speed;
				}
			}
			else {
				// Drive motor controls with modifiers applied
				if (directionModifier > 0){
					// Regular direction controls
					motor[motorLeft]=Ch3Adjusted;
					motor[motorRight]=Ch2Adjusted;
				}
				else{
					// Reverse direction controls (joysticks flipped)
					motor[motorLeft]=Ch2Adjusted;
					motor[motorRight]=Ch3Adjusted;
				}
			}

			// Move autobox platform
			if(vexRT[Btn5U]){
				motor[autobox]=50;
				autoboxPosition -= 1;
				wait1Msec(50);
			}
			else if(vexRT[Btn5D]){
				motor[autobox]=-50;
				autoboxPosition += 1;
				wait1Msec(50);
			}
			else{
				motor[autobox]=0;
			}

		}

		// Toogle the speed mode (regular or slow)
		if(vexRT[Btn7L]){
			if (!lastpress7L){
				if (speedModifier == 1.0){
					speedModifier = 0.5;
				}
				else{
					speedModifier = 1.0;
				}
			}
			lastpress7L = true;
		}
		else {
			lastpress7L = false;
		}

		// Toggle the robot direction (forward or reverse)
		if(vexRT[Btn7U]){
			if (!lastpress7U){
				if (directionModifier == 1){
					directionModifier = -1;
				}
				else{
					directionModifier = 1;
				}
			}
			lastpress7U = true;
		}
		else {
			lastpress7U = false;
		}

		// Toggle the robot direction (forward or reverse)
		if(vexRT[Btn8D]){
			if (!lastpress8D){
				if (rakeUp){
					motor[rake] = 127;
					rakeUp = false;
				}
				else{
					motor[rake] = -127;
					rakeUp = true;
				}
			}
			lastpress8D = true;
		}
		else {
			lastpress8D = false;
		}

		if(vexRT[Btn8L]){
			if (!lastpress8L){
				if (sizeLowered){
					motor[sizeChange] = 127;
					sizeLowered = false;
				}
				else{
					motor[sizeChange] = -127;
					sizeLowered = true;
				}
			}
			lastpress8L = true;
		}
		else {
			lastpress8L = false;
		}

		if(vexRT[Btn8R]){
			if (!lastpress8R){
				if (fingerPosition < 3){
					fingerPosition += 1;
				}
				else{
					fingerPosition = 0;
				}
				motor[finger] = fingerPositions[fingerPosition];
			}
			lastpress8R = true;
		}
		else {
			lastpress8R = false;
		}
	}
}
