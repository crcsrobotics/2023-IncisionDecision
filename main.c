#pragma config(Motor,  port2,           motorLeft,     tmotorServoContinuousRotation, openLoop)
#pragma config(Motor,  port4,           sizeChange,    tmotorNone, openLoop)
#pragma config(Motor,  port5,           autobox,       tmotorServoContinuousRotation, openLoop)
#pragma config(Motor,  port6,           forkliftMotor, tmotorServoContinuousRotation, openLoop)
#pragma config(Motor,  port7,           claw,          tmotorServoStandard, openLoop)
#pragma config(Motor,  port9,           motorRight,    tmotorServoContinuousRotation, openLoop)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

task main()
{bool autonomous = false;
while (true){
	if (autonomous){


}
else{
	motor[motorLeft]=vexRT[Ch3];
	motor[motorRight]=vexRT[Ch2];
	if(vexRT[Btn6U]){
	motor[forkliftMotor]=127;


}
	if(vexRT[Btn6D]){
	motor[forkliftMotor]=-127;

}
}
}
}
