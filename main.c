#pragma config(Sensor, dgtl1,  stopSensor,     sensorDigitalIn)
#pragma config(Sensor, dgtl3,  leftSensor,     sensorDigitalIn)
#pragma config(Sensor, dgtl4,  rightSensor,    sensorDigitalIn)
#pragma config(Motor,  port2,           motorLeft,     tmotorServoContinuousRotation, openLoop, reversed)
#pragma config(Motor,  port3,           rake,          tmotorServoStandard, openLoop)
#pragma config(Motor,  port4,           sizeChange,    tmotorServoStandard, openLoop)
#pragma config(Motor,  port5,           autobox,       tmotorServoContinuousRotation, openLoop)
#pragma config(Motor,  port6,           forkliftMotor, tmotorServoContinuousRotation, openLoop)
#pragma config(Motor,  port7,           claw,          tmotorServoStandard, openLoop)
#pragma config(Motor,  port8,           scooper,       tmotorServoStandard, openLoop)
#pragma config(Motor,  port9,           motorRight,    tmotorServoContinuousRotation, openLoop)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

/*
Control List

Ch3 - Left motor
Ch2 - Right motor
Btn6U - Forklift up
Btn6D - Forklift down
Btn5U - Autobox up
Btn5D - Autobox down
Btn8D - Open/Close Claw
Btn8L - Extend to full size (Exit compliance size)
Btn8R - Lift/lower scooper
Btn7R - Start/cancel autonomous brain biopsy
Btn7L - Slow Mode
Btn7U - Reverse Mode
Btn7D - Toggle Rake


*/

// Variables are defined outside of the main task to make them global.
// When variables are global, they can be viewed in the robot debug tools.
// Debugger tools can be found under the "Robot" menu when the robot is connected.

bool autonomous = false;

// These variables are used to create toggle buttons.
bool lastpress8D = false;
bool lastpress8R = false;
bool lastpress7L = false;
bool lastpress7U = false;
bool lastpress7D = false;
bool lastpress8L = false;
bool clawOpen = true;
bool scooperLowered = true;
bool rakeUp = false;
bool sizeLowered = false;

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
int limitForklift = 0;
int limitAutobox = 0;

task main(){

	// Move servo to fit in competition dimensions
	motor[sizeChange]=-127;
	motor[claw]=127;

	while (true){

		// Autonomous brain biopsy code
		if (autonomous){

			if(vexRT[Btn7L]){
				// Cancel autonomous
				autonomous = false;
			}

			// Backup code for if the floor is too dark to reflect IR.
			if (floorTooDark){
				// Motors drive forward unless they see the white line, in which case they reverse to correct their direction.

				if (SensorValue[leftSensor] == 1){
					motor[motorLeft] = 40;
				}
				else {
					motor[motorLeft] = -110;
				}

				if (SensorValue[rightSensor] == 1){
					motor[motorRight] = 40;
				}
				else {
					motor[motorRight] = -110;
				}
			}

			// Code for when the floor is light enough to reflect IR.
			else{
				// Motors drive forward unless they see a black line, in which case they reverse to correct their direction.

				if (SensorValue[leftSensor] == 0){
					motor[motorLeft] = 50;
				}
				else {
					motor[motorLeft] = -50;
				}

				if (SensorValue[rightSensor] == 0){
					motor[motorRight] = 50;
				}
				else {
					motor[motorRight] = -50;
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


			// Add controller deadzone to prevent controller drift issues and apply speed and direction modifiers.
			if (vexRT[Ch2] > 15 || vexRT[Ch2] < -15){
				Ch2Adjusted = vexRT[Ch2] * speedModifier * directionModifier;
			}
			else{
				Ch2Adjusted = 0;
			}

			if (vexRT[Ch3] > 15 || vexRT[Ch3] < -15){
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

			// Forklift controls
			if(vexRT[Btn6U]){
				motor[forkliftMotor]=64;
				limitForklift += 1;
				wait1Msec(10);
			}
			else if(vexRT[Btn6D]){
				motor[forkliftMotor]=-64;
				limitForklift -= 1;
				wait1Msec(10);

			}
			else{
				motor[forkliftMotor]=0;
			}

			// Move autobox platform
			if(vexRT[Btn5U]){
				motor[autobox]=127;
				limitAutobox += 1;
				wait1Msec(10);
			}
			else if(vexRT[Btn5D]){
				motor[autobox]=-127;
				limitAutobox -= 1;
				wait1Msec(10);
			}
			else{
				motor[autobox]=0;
			}

			// Toggle button code
			if(vexRT[Btn8D]){
				// Check if button is already being pressed
				if(lastpress8D==false){
					// Triggers once on button press
					// If claw is open, close it. If claw is close, open it.
					if(clawOpen){
						motor[claw]=127;
						clawOpen = false;
					}
					else{
						motor[claw]=-127;
						clawOpen = true;
					}
				}
				// Set button to pressed
				lastpress8D=true;
			}
			else{
				// Set button to unpressed
				lastpress8D=false;
			}

			if(vexRT[Btn8R]){
				// Check if button is already being pressed
				if(lastpress8R==false){
					// Triggers once on button press
					// If scooper is up, lower it, if scooper is down, lift it.
					if(scooperLowered){
						motor[scooper]=127;
						scooperLowered = false;
					}
					else{
						motor[scooper]=-127;
						scooperLowered = true;
					}
				}
				// Set button to pressed
				lastpress8R=true;
			}
			else{
				// Set button to unpressed
				lastpress8R=false;
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
		if(vexRT[Btn7D]){
			if (!lastpress7D){
				if (rakeUp){
					motor[rake] = 127;
					rakeUp = false;
				}
				else{
					motor[rake] = -127;
					rakeUp = true;
				}
			}
			lastpress7D = true;
		}
		else {
			lastpress7D = false;
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
	}
}
