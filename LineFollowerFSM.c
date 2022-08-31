#include <LineFollowerFSM.h>
#include "Clock.h"
#include "Motor.h"
#include "UART0.h"

// Define thresholds
#define TURN_THRESHOLD 48
#define HARD_TURN_THRESHOLD 200
#define SHARP_TURN_THRESHOLD 600

//--------------------------------------------------------------------------
// Initialize FSM
//--------------------------------------------------------------------------
void LF_InitializeFSM(LF_FSMType *FSM)
{
    FSM->CurrentState = FSMCentered;
}

//--------------------------------------------------------------------------
// Determine next FSM state (details on transitions can be found in FSM diagram)
//--------------------------------------------------------------------------
LF_FSMState LF_NextStateFunction(LF_FSMType *FSM)
{
    LF_FSMState NextState = FSM->CurrentState;
    switch (FSM->CurrentState) {
        case FSMCentered:
            if (FSM->LineSensorPosition >= -TURN_THRESHOLD && FSM->LineSensorPosition <= TURN_THRESHOLD) {
                NextState = FSMCentered;
            } else if (FSM->LineSensorPosition == 1000) {
                NextState = FSMLost;
            } else if (FSM->LineSensorPosition > TURN_THRESHOLD) {
                if (FSM->LineSensorPosition <= HARD_TURN_THRESHOLD) {
                    NextState = FSMRight;
                } else if (FSM->LineSensorPosition <= SHARP_TURN_THRESHOLD) {
                    NextState = FSMHardRight;
                } else {
                    NextState = FSMSharpRight;
                }
            } else if (FSM->LineSensorPosition < -TURN_THRESHOLD) {
                if (FSM->LineSensorPosition >= -HARD_TURN_THRESHOLD) {
                    NextState = FSMLeft;
                } else if (FSM->LineSensorPosition >= -SHARP_TURN_THRESHOLD) {
                    NextState = FSMHardLeft;
                } else {
                    NextState = FSMSharpLeft;
                }
            }
            break;
        case FSMRight:
            if (FSM->LineSensorPosition >= -TURN_THRESHOLD && FSM->LineSensorPosition <= TURN_THRESHOLD) {
                NextState = FSMCentered;
            } else if (FSM->LineSensorPosition == 1000) {
                NextState = FSMHardRight;
            } else if (FSM->LineSensorPosition > TURN_THRESHOLD) {
                if (FSM->LineSensorPosition <= HARD_TURN_THRESHOLD) {
                    NextState = FSMRight;
                } else if (FSM->LineSensorPosition <= SHARP_TURN_THRESHOLD) {
                    NextState = FSMHardRight;
                } else {
                    NextState = FSMSharpRight;
                }
            } else if (FSM->LineSensorPosition < -TURN_THRESHOLD) {
                if (FSM->LineSensorPosition >= -HARD_TURN_THRESHOLD) {
                    NextState = FSMLeft;
                } else if (FSM->LineSensorPosition >= -SHARP_TURN_THRESHOLD) {
                    NextState = FSMHardLeft;
                } else {
                    NextState = FSMSharpLeft;
                }
            }
            break;
        case FSMHardRight:
            if (FSM->LineSensorPosition >= -TURN_THRESHOLD && FSM->LineSensorPosition <= TURN_THRESHOLD) {
                NextState = FSMCentered;
            } else if (FSM->LineSensorPosition == 1000) {
                NextState = FSMSharpRight;
            } else if (FSM->LineSensorPosition > TURN_THRESHOLD) {
                if (FSM->LineSensorPosition <= HARD_TURN_THRESHOLD) {
                    NextState = FSMRight;
                } else {
                    NextState = FSMHardRight;
                }
            } else if (FSM->LineSensorPosition < -TURN_THRESHOLD) {
                if (FSM->LineSensorPosition >= -HARD_TURN_THRESHOLD) {
                    NextState = FSMLeft;
                } else {
                    NextState = FSMHardLeft;
                }
            }
            break;
        case FSMSharpRight:
            if (FSM->LineSensorPosition >= -TURN_THRESHOLD && FSM->LineSensorPosition <= TURN_THRESHOLD) {
                NextState = FSMCentered;
            } else if (FSM->LineSensorPosition == 1000) {
                NextState = FSMCentered2;
            } else if (FSM->LineSensorPosition > TURN_THRESHOLD) {
                if (FSM->LineSensorPosition <= HARD_TURN_THRESHOLD) {
                    NextState = FSMRight;
                } else {
                    NextState = FSMHardRight;
                }
            } else if (FSM->LineSensorPosition < -TURN_THRESHOLD) {
                if (FSM->LineSensorPosition >= -HARD_TURN_THRESHOLD) {
                    NextState = FSMLeft;
                } else {
                    NextState = FSMHardLeft;
                }
            }
            break;
        case FSMLeft:
            if (FSM->LineSensorPosition >= -TURN_THRESHOLD && FSM->LineSensorPosition <= TURN_THRESHOLD) {
                NextState = FSMCentered;
            } else if (FSM->LineSensorPosition == 1000) {
                NextState = FSMHardLeft;
            } else if (FSM->LineSensorPosition > TURN_THRESHOLD) {
                if (FSM->LineSensorPosition <= HARD_TURN_THRESHOLD) {
                    NextState = FSMRight;
                } else if (FSM->LineSensorPosition <= SHARP_TURN_THRESHOLD) {
                    NextState = FSMHardRight;
                } else {
                    NextState = FSMSharpRight;
                }
            } else if (FSM->LineSensorPosition < -TURN_THRESHOLD) {
                if (FSM->LineSensorPosition >= -HARD_TURN_THRESHOLD) {
                    NextState = FSMLeft;
                } else if (FSM->LineSensorPosition >= -SHARP_TURN_THRESHOLD) {
                    NextState = FSMHardLeft;
                } else {
                    NextState = FSMSharpLeft;
                }
            }
            break;
        case FSMHardLeft:
            if (FSM->LineSensorPosition >= -TURN_THRESHOLD && FSM->LineSensorPosition <= TURN_THRESHOLD) {
                NextState = FSMCentered;
            } else if (FSM->LineSensorPosition == 1000) {
                NextState = FSMSharpLeft;
            } else if (FSM->LineSensorPosition > TURN_THRESHOLD) {
                if (FSM->LineSensorPosition <= HARD_TURN_THRESHOLD) {
                    NextState = FSMRight;
                } else {
                    NextState = FSMHardRight;
                }
            } else if (FSM->LineSensorPosition < -TURN_THRESHOLD) {
                if (FSM->LineSensorPosition >= -HARD_TURN_THRESHOLD) {
                    NextState = FSMLeft;
                } else {
                    NextState = FSMHardLeft;
                }
            }
            break;
        case FSMSharpLeft:
            if (FSM->LineSensorPosition >= -TURN_THRESHOLD && FSM->LineSensorPosition <= TURN_THRESHOLD) {
                NextState = FSMCentered;
            } else if (FSM->LineSensorPosition == 1000) {
                NextState = FSMCentered2;
            } else if (FSM->LineSensorPosition > TURN_THRESHOLD) {
                if (FSM->LineSensorPosition <= HARD_TURN_THRESHOLD) {
                    NextState = FSMRight;
                } else {
                    NextState = FSMHardRight;
                }
            } else if (FSM->LineSensorPosition < -TURN_THRESHOLD) {
                if (FSM->LineSensorPosition >= -HARD_TURN_THRESHOLD) {
                    NextState = FSMLeft;
                } else {
                    NextState = FSMHardLeft;
                }
            }
            break;
        case FSMCentered2:
            if (FSM->LineSensorPosition >= -TURN_THRESHOLD && FSM->LineSensorPosition <= TURN_THRESHOLD) {
                NextState = FSMCentered;
            } else if (FSM->LineSensorPosition == 1000) {
                NextState = FSMLost;
            } else if (FSM->LineSensorPosition > TURN_THRESHOLD) {
                if (FSM->LineSensorPosition <= HARD_TURN_THRESHOLD) {
                    NextState = FSMRight;
                } else {
                    NextState = FSMHardRight;
                }
            } else if (FSM->LineSensorPosition < -TURN_THRESHOLD) {
                if (FSM->LineSensorPosition >= -HARD_TURN_THRESHOLD) {
                    NextState = FSMLeft;
                } else {
                    NextState = FSMHardLeft;
                }
            }
            break;
        case FSMLost:
            if (FSM->LineSensorPosition >= -TURN_THRESHOLD && FSM->LineSensorPosition <= TURN_THRESHOLD) {
                NextState = FSMCentered;
            } else {
                NextState = FSMLost;
            }
            break;
    }

    return NextState;
}

//--------------------------------------------------------------------------
// Determine LED output based on state (details on outputs can be found in FSM diagram)
//--------------------------------------------------------------------------
void LF_OutputFunction(LF_FSMType *FSM)
{
    switch (FSM->CurrentState) {
        case FSMCentered:
//            UART0_OutString("Centered");
            Motor_Forward(SPEED * FSM->GSM, SPEED * FSM->GSM);
            TURN_ON_LED_LT;
            TURN_ON_LED_RT;
            TURN_OFF_LED_B;
            break;
        case FSMRight:
//            UART0_OutString("Right");
            Motor_Forward(SPEED * FSM->GSM, (SPEED - 10) * FSM->GSM);
            TURN_OFF_LED_LT;
            TURN_ON_LED_RT;
            TURN_OFF_LED_B;
            break;
        case FSMHardRight:
//            UART0_OutString("HardRight");
            Motor_Right(SPEED * FSM->GSM, (SPEED/2) * FSM->GSM);
            TURN_OFF_LED_LT;
            TURN_ON_LED_RT;
            TURN_OFF_LED_B;
            break;
        case FSMSharpRight:
//            UART0_OutString("SharpRight");
            Motor_Right(SPEED * FSM->GSM, SPEED * FSM->GSM);
            TURN_OFF_LED_LT;
            TURN_ON_LED_RT;
            TURN_OFF_LED_B;
            Clock_Delay1ms(400);
            break;
        case FSMLeft:
//            UART0_OutString("Left");
            Motor_Forward((SPEED - 10) * FSM->GSM, SPEED * FSM->GSM);
            TURN_ON_LED_LT;
            TURN_OFF_LED_RT;
            TURN_OFF_LED_B;
            break;
        case FSMHardLeft:
//            UART0_OutString("HardLeft");
            Motor_Left((SPEED/2) * FSM->GSM, SPEED * FSM->GSM);
            TURN_ON_LED_LT;
            TURN_OFF_LED_RT;
            TURN_OFF_LED_B;
            break;
        case FSMSharpLeft:
//            UART0_OutString("SharpLeft");
            Motor_Left(SPEED * FSM->GSM, SPEED * FSM->GSM);
            TURN_ON_LED_LT;
            TURN_OFF_LED_RT;
            TURN_OFF_LED_B;
            Clock_Delay1ms(400);
            break;
        case FSMCentered2:
//            UART0_OutString("Centered2");
            Motor_Forward(SPEED * FSM->GSM, SPEED * FSM->GSM);
            TURN_ON_LED_LT;
            TURN_ON_LED_RT;
            TURN_OFF_LED_B;
            Clock_Delay1ms(200);
            break;
        case FSMLost:
//            UART0_OutString("Lost");
            Motor_Stop();
            TURN_OFF_LED_LT;
            TURN_OFF_LED_RT;
            TURN_ON_LED_B;
            break;
    }
}

// Convert a FSMState enum value to its string name
char* LF_getStateString(LF_FSMState state) {
    switch(state) {
        case FSMCentered: return "Centered  ";
        case FSMRight: return "Right     ";
        case FSMHardRight: return "HardRight ";
        case FSMSharpRight: return "SharpRight";
        case FSMLeft: return "Left      ";
        case FSMHardLeft: return "HardLeft  ";
        case FSMSharpLeft: return "SharpLeft ";
        case FSMCentered2: return "Centered2 ";
        case FSMLost: return "Lost      ";
    }

    return "Lost";
}
