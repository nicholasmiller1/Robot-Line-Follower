#include <BoosterPackFSM.h>
#include "Clock.h"
#include "UART0.h"

//--------------------------------------------------------------------------
// Initialize FSM
//--------------------------------------------------------------------------
void BP_InitializeFSM(BP_FSMType *FSM)
{
    FSM->CurrentState = FSMPause;
}

//--------------------------------------------------------------------------
// Determine next FSM state (details on transitions can be found in FSM diagram)
//--------------------------------------------------------------------------
BP_FSMState BP_NextStateFunction(BP_FSMType *FSM)
{
    BP_FSMState NextState = FSM->CurrentState;
    switch (FSM->CurrentState) {
        case FSMPause:
            if (FSM->InputS1 == Active) {
                NextState = FSMNormal;
            } else {
                NextState = FSMPause;
            }
            break;
        case FSMNormal:
            if (FSM->InputS2 == Active) {
                NextState = FSMPause;
            } else if (FSM->AccelZ > ACCEL_THRESHOLD) {
                NextState = FSMAccelAbove1;
            } else if (FSM->BumpSensor == Bumped || FSM->LightSensor < LIGHT_DARK_THRESHOLD) {
                NextState = FSMOtherStop;
            } else if (FSM->LightSensor < LIGHT_DIM_THRESHOLD) {
                NextState = FSMSlow;
            } else {
                NextState = FSMNormal;
            }
            break;
        case FSMAccelAbove1:
            if (FSM->InputS2 == Active) {
                NextState = FSMPause;
            } else if (FSM->AccelZ <= ACCEL_THRESHOLD && FSM->AccelZ >= -ACCEL_THRESHOLD) {
                NextState = FSMNearStable1;
            } else {
                NextState = FSMAccelAbove1;
            }
            break;
        case FSMNearStable1:
            if (FSM->InputS2 == Active) {
                NextState = FSMPause;
            } else if (FSM->AccelZ < -ACCEL_THRESHOLD) {
                NextState = FSMAccelBelow1;
            } else if (FSM->AccelZ > ACCEL_THRESHOLD) {
                NextState = FSMNormal;
            } else {
                NextState = FSMNearStable1;
            }
            break;
        case FSMAccelBelow1:
            if (FSM->InputS2 == Active) {
                NextState = FSMPause;
            } else if (FSM->AccelZ <= ACCEL_THRESHOLD && FSM->AccelZ >= -ACCEL_THRESHOLD) {
                NextState = FSMLiftStop;
            } else {
                NextState = FSMAccelBelow1;
            }
            break;
        case FSMLiftStop:
            if (FSM->InputS2 == Active) {
                NextState = FSMPause;
            } else if (FSM->AccelZ < -ACCEL_THRESHOLD) {
                NextState = FSMAccelBelow2;
            } else {
                NextState = FSMLiftStop;
            }
            break;
        case FSMAccelBelow2:
            if (FSM->InputS2 == Active) {
                NextState = FSMPause;
            } else if (FSM->AccelZ <= ACCEL_THRESHOLD && FSM->AccelZ >= -ACCEL_THRESHOLD) {
                NextState = FSMNearStable2;
            } else {
                NextState = FSMAccelBelow2;
            }
            break;
        case FSMNearStable2:
            if (FSM->InputS2 == Active) {
                NextState = FSMPause;
            } else if (FSM->AccelZ > ACCEL_THRESHOLD) {
                NextState = FSMAccelAbove2;
            } else if (FSM->AccelZ < -ACCEL_THRESHOLD) {
                NextState = FSMLiftStop;
            } else {
                NextState = FSMNearStable2;
            }
            break;
        case FSMAccelAbove2:
            if (FSM->InputS2 == Active) {
                NextState = FSMPause;
            } else if (FSM->AccelZ <= ACCEL_THRESHOLD && FSM->AccelZ >= -ACCEL_THRESHOLD) {
                NextState = FSMNormal;
            } else {
                NextState = FSMAccelAbove2;
            }
            break;
        case FSMOtherStop:
            if (FSM->InputS2 == Active) {
                NextState = FSMPause;
            } else if (FSM->AccelZ > ACCEL_THRESHOLD) {
                NextState = FSMAccelAbove1;
            } else if (FSM->BumpSensor == Bumped || FSM->LightSensor < LIGHT_DARK_THRESHOLD) {
                NextState = FSMOtherStop;
            } else if (FSM->LightSensor < LIGHT_DIM_THRESHOLD) {
                NextState = FSMSlow;
            } else {
                NextState = FSMNormal;
            }
            break;
        case FSMSlow:
            if (FSM->InputS2 == Active) {
                NextState = FSMPause;
            } else if (FSM->AccelZ > ACCEL_THRESHOLD) {
                NextState = FSMAccelAbove1;
            } else if (FSM->BumpSensor == Bumped || FSM->LightSensor < LIGHT_DARK_THRESHOLD) {
                NextState = FSMOtherStop;
            } else if (FSM->LightSensor < LIGHT_DIM_THRESHOLD) {
                NextState = FSMSlow;
            } else {
                NextState = FSMNormal;
            }
            break;
    }

    return NextState;
}

//--------------------------------------------------------------------------
// Determine LED output based on state (details on output can be found in FSM diagram)
//--------------------------------------------------------------------------
float BP_OutputFunction(BP_FSMType *FSM)
{
    switch (FSM->CurrentState) {
        case FSMPause:
//            UART0_OutString("Pause\n\r");
            return 0;
        case FSMNormal:
//            UART0_OutString("Normal\n\r");
            return 1;
        case FSMAccelAbove1:
//            UART0_OutString("AccelAbove1\n\r");
            return 0;
        case FSMNearStable1:
//            UART0_OutString("NearStable1\n\r");
            return 0;
        case FSMAccelBelow1:
//            UART0_OutString("AccelBelow1\n\r");
            return 0;
        case FSMLiftStop:
//            UART0_OutString("LiftStop\n\r");
            return 0;
        case FSMAccelBelow2:
//            UART0_OutString("AccelBelow2\n\r");
            return 0;
        case FSMNearStable2:
//            UART0_OutString("NearStable2\n\r");
            return 0;
        case FSMAccelAbove2:
//            UART0_OutString("AccelAbove2\n\r");
            return 0;
        case FSMOtherStop:
//            UART0_OutString("OtherStop\n\r");
            return 0;
        case FSMSlow:
//            UART0_OutString("Slow\n\r");
            return 0.5;
    }

    return 0;
}

// Convert a FSMState enum value to its string name
char* BP_getStateString(BP_FSMState state) {
    switch(state) {
        case FSMPause: return "Pause     ";
        case FSMNormal: return "Normal    ";
        case FSMAccelAbove1: return "Rising    ";
        case FSMNearStable1: return "Slow Risin";
        case FSMAccelBelow1: return "Stop Risin";
        case FSMLiftStop: return "Lifted    ";
        case FSMAccelBelow2: return "Lowering  ";
        case FSMNearStable2: return "Slow Lower";
        case FSMAccelAbove2: return "Stop Lower";
        case FSMOtherStop: return "Stop      ";
        case FSMSlow: return "Slow      ";
    }

    return "Pause";
}
