#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include "PWM.h"
#include "Motor.h"

// Initialize all required pins for motor operation
//   Left Motor:                Right Motor:
//    - Direction: P5.4         - Direction: P5.5
//    - PWM:       P2.7         - PWM        P2.6
//    - Enable:    P3.7         - Enable     P3.6
void Motor_Init(void){
    // initialize PWM outputs to 0% duty cycle on the two motor PWM pins (P2.6 and P2.7)
    PWM_Init34(15000, 0, 0);

    // initialize motor enable and direction pins as GPIO outputs
    GPIO_setAsOutputPin(GPIO_PORT_P3, GPIO_PIN6 | GPIO_PIN7);
    GPIO_setAsOutputPin(GPIO_PORT_P5, GPIO_PIN4 | GPIO_PIN5);

    // set motors to sleep mode
    GPIO_setOutputLowOnPin(GPIO_PORT_P3, GPIO_PIN6 | GPIO_PIN7);

    // set motor direction to forward initially
    GPIO_setOutputLowOnPin(GPIO_PORT_P5, GPIO_PIN4 | GPIO_PIN5);
}

// Drive both motors forwards at the given duty cycles
void Motor_Forward(uint16_t leftDuty, uint16_t rightDuty){
    // take motors out of sleep mode
    GPIO_setOutputHighOnPin(GPIO_PORT_P3, GPIO_PIN6 | GPIO_PIN7);

    // set motor directions to forward
    GPIO_setOutputLowOnPin(GPIO_PORT_P5, GPIO_PIN4 | GPIO_PIN5);

    // set PWM values for motor speed
    PWM_Duty3(rightDuty);
    PWM_Duty4(leftDuty);
}

// Drive both motors backwards at the given duty cycles
void Motor_Backward(uint16_t leftDuty, uint16_t rightDuty){
    // take motors out of sleep mode
    GPIO_setOutputHighOnPin(GPIO_PORT_P3, GPIO_PIN6 | GPIO_PIN7);

    // set motor directions to backwards
    GPIO_setOutputHighOnPin(GPIO_PORT_P5, GPIO_PIN4 | GPIO_PIN5);

    // set PWM values for motor speed
    PWM_Duty3(rightDuty);
    PWM_Duty4(leftDuty);
}

// Drive the right motor forwards and the left motor backwards at the given duty cycles
void Motor_Left(uint16_t leftDuty, uint16_t rightDuty){
    // take motors out of sleep mode
    GPIO_setOutputHighOnPin(GPIO_PORT_P3, GPIO_PIN6 | GPIO_PIN7);

    // set right motor direction to forward and left motor direction to backwards
    GPIO_setOutputLowOnPin(GPIO_PORT_P5, GPIO_PIN5);    // right motor forward
    GPIO_setOutputHighOnPin(GPIO_PORT_P5, GPIO_PIN4);    // left motor backwards

    // set PWM values for motor speed
    PWM_Duty3(rightDuty);
    PWM_Duty4(leftDuty);
}

// Drive the right motor backwards and the left motor forwards at the given duty cycles
void Motor_Right(uint16_t leftDuty, uint16_t rightDuty){
    // take motors out of sleep mode
    GPIO_setOutputHighOnPin(GPIO_PORT_P3, GPIO_PIN6 | GPIO_PIN7);

    // set right motor direction to backwards and left motor direction to forward
    GPIO_setOutputLowOnPin(GPIO_PORT_P5, GPIO_PIN4);    // left motor forward
    GPIO_setOutputHighOnPin(GPIO_PORT_P5, GPIO_PIN5);    // right motor backwards

    // set PWM values for motor speed
    PWM_Duty3(rightDuty);
    PWM_Duty4(leftDuty);
}


// Stop the motors and enter sleep mode
void Motor_Stop(void){
    // enter sleep mode
    GPIO_setOutputLowOnPin(GPIO_PORT_P3, GPIO_PIN6 | GPIO_PIN7);

    // set duty cycles to 0%
    PWM_Duty3(0);
    PWM_Duty4(0);
}
