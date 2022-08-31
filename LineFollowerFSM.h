#ifndef LINEFOLLOWERFSM_H_
#define LINEFOLLOWERFSM_H_

#include "PortPins.h"
#include "Reflectance.h"
#include "gpio.h"

// Define constants
#define SPEED 25

// State Type Definitions
typedef enum {
    FSMCentered,
    FSMRight,
    FSMHardRight,
    FSMSharpRight,
    FSMLeft,
    FSMHardLeft,
    FSMSharpLeft,
    FSMCentered2,
    FSMLost,
} LF_FSMState;

// FSM Current State Definition
typedef struct {
    LF_FSMState CurrentState;       // Current state of the FSM
    int32_t     LineSensorPosition; // Current input of the Line Sensor
    float       GSM;                // Current input of the Global Speed Multiplier
} LF_FSMType;

// Function Prototypes

/**
 * Initializes the FSM by setting the current state to the initial state
 * @param FSM a pointer to the FSM instance to initialize
 */
void LF_InitializeFSM(LF_FSMType *FSM);

/**
 * Determine next FSM state
 * @param FSM a pointer to the FSM instance to find the next state of
 * @return The next state of the FSM instance
 */
LF_FSMState LF_NextStateFunction(LF_FSMType *FSM);

/**
 * Determine LED output based on state
 * @param FSM a pointer to the FSM instance to determine the output of
 */
void LF_OutputFunction(LF_FSMType *FSM);

/**
 * Convert a FSMState enum value to its string name
 * @param state the enum value to convert
 * @return The name of the state as a string
 */
char* LF_getStateString(LF_FSMState state);

#endif /* LINEFOLLOWERFSM_H_ */
