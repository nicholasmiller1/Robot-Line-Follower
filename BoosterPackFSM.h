#ifndef BOOSTERPACKFSM_H_
#define BOOSTERPACKFSM_H_

#include "Bump.h"
#include "Switch.h"

// Define constants and thresholds
#define Z_BASIS 16300
#define LIGHT_DIM_THRESHOLD 100
#define LIGHT_DARK_THRESHOLD 50
#define ACCEL_THRESHOLD 1000

// State Type Definitions
typedef enum {
    FSMPause,
    FSMNormal,
    FSMAccelAbove1,
    FSMNearStable1,
    FSMAccelBelow1,
    FSMLiftStop,
    FSMAccelBelow2,
    FSMNearStable2,
    FSMAccelAbove2,
    FSMOtherStop,
    FSMSlow,
} BP_FSMState;

// FSM Current State Definition
typedef struct {
    BP_FSMState  CurrentState; // Current state of the FSM
    float        LightSensor;  // Current input of the Light Sensor
    int16_t      AccelZ;       // Current input of the z-axis acceleration
    BumpStatus   BumpSensor;   // Current input of the Bump Sensor
    SwitchStatus InputS1;      // Current input of Switch 1
    SwitchStatus InputS2;      // Current input of Switch 2
} BP_FSMType;

// Function Prototypes

/**
 * Initializes the FSM by setting the current state to the initial state
 * @param FSM a pointer to the FSM instance to initialize
 */
void BP_InitializeFSM(BP_FSMType *FSM);

/**
 * Determine next FSM state
 * @param FSM a pointer to the FSM instance to find the next state of
 * @return The next state of the FSM instance
 */
BP_FSMState BP_NextStateFunction(BP_FSMType *FSM);

/**
 * Determine LED output based on state
 * @param FSM a pointer to the FSM instance to determine the output of
 */
float BP_OutputFunction(BP_FSMType *FSM);

/**
 * Convert a FSMState enum value to its string name
 * @param state the enum value to convert
 * @return The name of the state as a string
 */
char* BP_getStateString(BP_FSMState state);

#endif /* BOOSTERPACKFSM_H_ */
