#ifndef PWM_H
#define PWM_H

/*
 * Initializes PWM output on P2.4 using Timer A0 CCR1 interrupt
 * Assumes SMCLK has been initialized to 12 MHz
 * @param period: the period of the PWM signal in 83.3 us units
 * @param duty: the desired duty cycle as a percent (from 0-100)
 * @return none
 */
void PWM_Init1(uint16_t period, uint16_t duty);


/*
 * Initializes PWM output on P2.4 using Timer A0 CCR1 interrupt
 * and on P2.5 using the Timer A0 CCR1 interrupt
 * Assumes SMCLK has been initialized to 12 MHz
 * @param period: the period of the PWM signal in 83.3 us units
 * @param duty1: the desired duty cycle on P2.4 (TA0 CCR1) as a percent (from 0-100)
 * @param duty2: the desired duty cycle on P2.4 (TA0 CCR2) as a percent (from 0-100)
 * @return none
 */
void PWM_Init12(uint16_t period, uint16_t duty1, uint16_t duty2);


/*
 * Initializes PWM output on P2.6 (right motor) using Timer A0 CCR3 interrupt
 * and on P2.7 (left motor) using the Timer A0 CCR4 interrupt
 * Assumes SMCLK has been initialized to 12 MHz
 * @param period: the period of the PWM signal in 83.3 us units
 * @param duty3: the desired duty cycle on P2.6 (TA0 CCR3) as a percent (from 0-100)
 * @param duty4: the desired duty cycle on P2.7 (TA0 CCR4) as a percent (from 0-100)
 * @return none
 */
void PWM_Init34(uint16_t period, uint16_t duty3, uint16_t duty4);


/*
 * Sets the duty cycle of the PWM output on P2.4
 * @param duty1: the desired duty cycle on P2.4 (changes TA0 CCR1)
 * @return none
 */
void PWM_Duty1(uint16_t duty1);


/*
 * Sets the duty cycle of the PWM output on P2.5
 * @param duty2: the desired duty cycle on P2.5 (changes TA0 CCR2)
 * @return none
 */
void PWM_Duty2(uint16_t duty2);


/*
 * Sets the duty cycle of the PWM output on P2.6 (right motor)
 * @param duty3: the desired duty cycle on P2.6 (changes TA0 CCR3)
 * @return none
 */
void PWM_Duty3(uint16_t duty1);


/*
 * Sets the duty cycle of the PWM output on P2.7 (left motor)
 * @param duty4: the desired duty cycle on P2.7 (changes TA0 CCR4)
 * @return none
 */
void PWM_Duty4(uint16_t duty4);


#endif
