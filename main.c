#pragma config(Motor,  port2,           motorLeft,     tmotorServoContinuousRotation, openLoop)
#pragma config(Motor,  port4,           sizeChange,    tmotorServoStandard, openLoop)
#pragma config(Motor,  port5,           autobox,       tmotorServoContinuousRotation, openLoop)
#pragma config(Motor,  port6,           forkliftMotor, tmotorServoContinuousRotation, openLoop)
#pragma config(Motor,  port7,           claw,          tmotorServoStandard, openLoop)
#pragma config(Motor,  port9,           motorRight,    tmotorServoContinuousRotation, openLoop)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

/*
	All of the code between this comment and the next comment is used to set the baud rate (communication speed) of the UART port.
	The UART port is the communication port that is used to give instructions to the IR transmitter used to change Squeaky's control scheme.
	I don't really understand this code. It was copied from a tutorial from BEST Robotics.
	Normally, setting the baudRate would be as simple as using `setBaudRate(UART1, <baud rate>);`.
	However, RobotC only allows the baud rate to be set to certain fixed values, such as baudRate1200.
	RobotC does not normally support a baud rate of 600 (the baud rate required to use the IR kit.)
	Because of this, we have to use the complicated code below to make our own method to change the baud rate.
*/

typedef unsigned long  uint32_t;
typedef unsigned short uint16_t;

typedef struct
{
  uint16_t SR;
  uint16_t RESERVED0;
  uint16_t DR;
  uint16_t RESERVED1;
  uint16_t BRR;
  uint16_t RESERVED2;
  uint16_t CR1;
  uint16_t RESERVED3;
  uint16_t CR2;
  uint16_t RESERVED4;
  uint16_t CR3;
  uint16_t RESERVED5;
  uint16_t GTPR;
  uint16_t RESERVED6;
} USART_TypeDef;

#define PERIPH_BASE           ((unsigned long)0x40000000)
#define APB1PERIPH_BASE       PERIPH_BASE
#define USART2_BASE           (APB1PERIPH_BASE + 0x4400)
#define USART3_BASE           (APB1PERIPH_BASE + 0x4800)
#define USART2                ((USART_TypeDef *) USART2_BASE)
#define USART3                ((USART_TypeDef *) USART3_BASE)

void setBaud( const TUARTs nPort, int baudRate ) {
    uint32_t tmpreg = 0x00, apbclock = 0x00;
    uint32_t integerdivider = 0x00;
    uint32_t fractionaldivider = 0x00;

    apbclock = 36000000;

    integerdivider = ((0x19 * apbclock) / (0x04 * (baudRate)));
    tmpreg = (integerdivider / 0x64) << 0x04;

    fractionaldivider = integerdivider - (0x64 * (tmpreg >> 0x04));
    tmpreg |= ((((fractionaldivider * 0x10) + 0x32) / 0x64)) & 0x0F;

    USART_TypeDef *uart = USART2;
    if( nPort == UART2 ) {
      uart = USART3;
    }
    uart->BRR = (uint16_t)tmpreg;
}
/*
	Now we are back to code that I can explain.
	The `main` task contains all of the code that is going to be run when the VEX Cortex is turned on.
*/

task main(){
	bool autonomous = false;
	bool lastpress8D = false;
	bool clawOpen = true;

	// Move servo to fit in competition dimensions
	motor[sizeChange]=127;

	while (true){
		if (autonomous){


		}
		// Manual Controls
		else{

			// Expand to full size
			if(vexRT[Btn8L]){
				motor[sizeChange]=-127;
			}

			// Drive motor controls
			motor[motorLeft]=vexRT[Ch3];
			motor[motorRight]=vexRT[Ch2];

			// Forklift controls
			if(vexRT[Btn6U]){
				motor[forkliftMotor]=127;
			}
			else if(vexRT[Btn6D]){
				motor[forkliftMotor]=-127;

			}
			else{
				motor[forkliftMotor]=0;
			}

			// Move autobox platform
			if(vexRT[Btn5U]){
				motor[autobox]=127;
			}
			else if(vexRT[Btn5D]){
				motor[autobox]=-127;
			}
			else{
				motor[autobox]=0;
			}

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

		}
	}
}
