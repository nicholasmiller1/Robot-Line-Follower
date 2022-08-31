#ifndef PORTPINS_H_
#define PORTPINS_H_

//  LED - Left Brake Light
#define LED_LB_PIN                  6
#define LED_LB_BIT                  (0x01 << LED_LB_PIN)
#define LED_LB_PORT                 P8
#define SET_LED_LB_AS_AN_OUTPUT     LED_LB_PORT->DIR |= LED_LB_BIT
#define TURN_ON_LED_LB           LED_LB_PORT->OUT |= LED_LB_BIT
#define TURN_OFF_LED_LB          LED_LB_PORT->OUT &= ~LED_LB_BIT

//  LED - Right Brake Light
#define LED_RB_PIN                  7
#define LED_RB_BIT                  (0x01 << LED_RB_PIN)
#define LED_RB_PORT                 P8
#define SET_LED_RB_AS_AN_OUTPUT     LED_RB_PORT->DIR |= LED_RB_BIT
#define TURN_ON_LED_RB           LED_RB_PORT->OUT |= LED_RB_BIT
#define TURN_OFF_LED_RB          LED_RB_PORT->OUT &= ~LED_RB_BIT

//  LED - Both Brake Lights
#define TURN_ON_LED_B            TURN_ON_LED_RB; TURN_ON_LED_LB
#define TURN_OFF_LED_B           TURN_OFF_LED_RB; TURN_OFF_LED_LB;

//  LED - Left Turn
#define LED_LT_PIN                  0
#define LED_LT_BIT                  (0x01 << LED_LT_PIN)
#define LED_LT_PORT                 P8
#define SET_LED_LT_AS_AN_OUTPUT     LED_LT_PORT->DIR |= LED_LT_BIT
#define TURN_ON_LED_LT           LED_LT_PORT->OUT |= LED_LT_BIT
#define TURN_OFF_LED_LT          LED_LT_PORT->OUT &= ~LED_LT_BIT

//  LED - Right Turn
#define LED_RT_PIN                  5
#define LED_RT_BIT                  (0x01 << LED_RT_PIN)
#define LED_RT_PORT                 P8
#define SET_LED_RT_AS_AN_OUTPUT     LED_RT_PORT->DIR |= LED_RT_BIT
#define TURN_ON_LED_RT           LED_RT_PORT->OUT |= LED_RT_BIT
#define TURN_OFF_LED_RT          LED_RT_PORT->OUT &= ~LED_RT_BIT

// Switch 1
#define SWITCH1_PIN                             1
#define SWITCH1_BIT                             (0x01 << SWITCH1_PIN)
#define SWITCH1_PORT                            P1
#define SET_SWITCH1_TO_AN_INPUT                 SWITCH1_PORT->DIR &= ~SWITCH1_BIT
#define ENABLE_PULL_UP_PULL_DOWN_RESISTORS_S1   SWITCH1_PORT->REN |= SWITCH1_BIT
#define SELECT_PULL_UP_RESISTORS_S1             SWITCH1_PORT->OUT |= SWITCH1_BIT

// Switch 2
#define SWITCH2_PIN                       4
#define SWITCH2_BIT                       (0x01 << SWITCH2_PIN)
#define SWITCH2_PORT                      P1
#define SET_SWITCH2_TO_AN_INPUT           SWITCH2_PORT->DIR &= ~SWITCH2_BIT
#define ENABLE_PULL_UP_PULL_DOWN_RESISTORS_S2   SWITCH2_PORT->REN |= SWITCH2_BIT
#define SELECT_PULL_UP_RESISTORS_S2             SWITCH2_PORT->OUT |= SWITCH2_BIT

#endif /* PORTPINS_H_ */
