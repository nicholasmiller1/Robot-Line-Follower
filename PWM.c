#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include "PWM.h"


// Struct for configuration options for PWM on P2.4
Timer_A_PWMConfig pwm1Config =
{
 TIMER_A_CLOCKSOURCE_SMCLK,     // timer sources from SMCLK
 TIMER_A_CLOCKSOURCE_DIVIDER_1,     // SMCLK / 1 = 12 MHz
 1,     // default period value before initialization
 TIMER_A_CAPTURECOMPARE_REGISTER_1, // P2.4 PWM is mapped to TA0 CCR1
 TIMER_A_OUTPUTMODE_RESET_SET,      // Reset set mode allows for PWM output
 0      // default duty cycle value before initialization
};


// Struct for configuration options for PWM on P2.5
Timer_A_PWMConfig pwm2Config =
{
 TIMER_A_CLOCKSOURCE_SMCLK,     // timer sources from SMCLK
 TIMER_A_CLOCKSOURCE_DIVIDER_1,     // SMCLK / 1 = 12 MHz
 1,     // default period value before initialization
 TIMER_A_CAPTURECOMPARE_REGISTER_2, // P2.5 PWM is mapped to TA0 CCR2
 TIMER_A_OUTPUTMODE_RESET_SET,      // Reset set mode allows for PWM output
 0      // default duty cycle value before initialization
};


// Struct for configuration options for PWM on P2.6 (right motor)
Timer_A_PWMConfig pwm3Config =
{
 TIMER_A_CLOCKSOURCE_SMCLK,     // timer sources from SMCLK
 TIMER_A_CLOCKSOURCE_DIVIDER_1,     // SMCLK / 1 = 12 MHz
 1,     // default period value before initialization
 TIMER_A_CAPTURECOMPARE_REGISTER_3, // P2.6 PWM is mapped to TA0 CCR3
 TIMER_A_OUTPUTMODE_RESET_SET,      // Reset set mode allows for PWM output
 0      // default duty cycle value before initialization
};


// Struct for configuration options for PWM on P2.7 (left motor)
Timer_A_PWMConfig pwm4Config =
{
 TIMER_A_CLOCKSOURCE_SMCLK,     // timer sources from SMCLK
 TIMER_A_CLOCKSOURCE_DIVIDER_1,     // SMCLK / 1 = 12 MHz
 1,     // default period value before initialization
 TIMER_A_CAPTURECOMPARE_REGISTER_4, // P2.7 PWM is mapped to TA0 CCR4
 TIMER_A_OUTPUTMODE_RESET_SET,      // Reset set mode allows for PWM output
 0      // default duty cycle value before initialization
};

// initialize PWM output on P2.4
void PWM_Init1(uint16_t period, uint16_t duty){
    // update config structs
    pwm1Config.timerPeriod = period;
    pwm1Config.dutyCycle = period * duty / 100;

    // set P2.4 to timer mode
    GPIO_setAsPeripheralModuleFunctionOutputPin(GPIO_PORT_P2, GPIO_PIN4, GPIO_PRIMARY_MODULE_FUNCTION);

    // Initialize PWM output
    Timer_A_generatePWM(TIMER_A0_BASE, &pwm1Config);
}

// initialize PWM output on P2.4 and P2.5
void PWM_Init12(uint16_t period, uint16_t duty1, uint16_t duty2){
    // update config structs
    pwm1Config.timerPeriod = period;
    pwm2Config.timerPeriod = period;
    pwm1Config.dutyCycle = period * duty1 / 100;
    pwm2Config.dutyCycle = period * duty2 / 100;

    // set P2.4 and P2.5 to timer mode
    GPIO_setAsPeripheralModuleFunctionOutputPin(GPIO_PORT_P2, GPIO_PIN4 | GPIO_PIN5, GPIO_PRIMARY_MODULE_FUNCTION);

    // Initialize PWM output
    Timer_A_generatePWM(TIMER_A0_BASE, &pwm1Config);
    Timer_A_generatePWM(TIMER_A0_BASE, &pwm2Config);
}

// initialize PWM output on P2.6 (right motor) and P2.7 (left motor)
void PWM_Init34(uint16_t period, uint16_t duty3, uint16_t duty4){
    // update config structs
    pwm3Config.timerPeriod = period;
    pwm4Config.timerPeriod = period;
    pwm3Config.dutyCycle = period * duty3 / 100;
    pwm4Config.dutyCycle = period * duty4 / 100;

    // set P2.6 and P2.7 to timer mode
    GPIO_setAsPeripheralModuleFunctionOutputPin(GPIO_PORT_P2, GPIO_PIN6 | GPIO_PIN7, GPIO_PRIMARY_MODULE_FUNCTION);

    // Initialize PWM output
    Timer_A_generatePWM(TIMER_A0_BASE, &pwm3Config);
    Timer_A_generatePWM(TIMER_A0_BASE, &pwm4Config);
}

// update duty cycle of PWM output on P2.4
void PWM_Duty1(uint16_t duty1){
    uint16_t newDuty1 = pwm1Config.timerPeriod * duty1 / 100;
    pwm1Config.dutyCycle = newDuty1;
    Timer_A_setCompareValue(TIMER_A0_BASE, TIMER_A_CAPTURECOMPARE_REGISTER_1, newDuty1);
}

// update duty cycle of PWM output on P2.5
void PWM_Duty2(uint16_t duty2){
    uint16_t newDuty2 = pwm2Config.timerPeriod * duty2 / 100;
    pwm2Config.dutyCycle = newDuty2;
    Timer_A_setCompareValue(TIMER_A0_BASE, TIMER_A_CAPTURECOMPARE_REGISTER_2, newDuty2);
}

// update duty cycle of PWM output on P2.6
// (update right motor speed)
void PWM_Duty3(uint16_t duty3){
    uint16_t newDuty3 = pwm3Config.timerPeriod * duty3 / 100;
    pwm3Config.dutyCycle = newDuty3;
    Timer_A_setCompareValue(TIMER_A0_BASE, TIMER_A_CAPTURECOMPARE_REGISTER_3, newDuty3);
}

// update duty cycle of PWM output on P2.7
// (update left motor speed)
void PWM_Duty4(uint16_t duty4){
    uint16_t newDuty4 = pwm4Config.timerPeriod * duty4 / 100;
    pwm4Config.dutyCycle = newDuty4;
    Timer_A_setCompareValue(TIMER_A0_BASE, TIMER_A_CAPTURECOMPARE_REGISTER_4, newDuty4);
}
