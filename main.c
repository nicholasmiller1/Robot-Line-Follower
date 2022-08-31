/* DriverLib Includes */
#include "driverlib.h"

/* Standard Includes */
#include <stdint.h>
#include <stdbool.h>

/* Booster Pack Includes */
#include "CortexM.h"
#include "Booster_Pack/opt3001.h"
#include "Booster_Pack/i2c_driver.h"
#include "Booster_Pack/bmi160_support.h"
#include "Booster_Pack/bme280_support.h"
#include <math.h>

/* Project Includes */
#include <LineFollowerFSM.h>
#include <BoosterPackFSM.h>
#include "Clock.h"
#include "TimerA1.h"
#include "Reflectance.h"
#include "Bump.h"
#include "PortPins.h"
#include "Motor.h"
#include "UART0.h"
#include "Nokia5110.h"

/**
 * START: start of code required for the Booster Pack sensors and I2C to work properly.
 * Copied from the provided booster pack GUI main code.
 */
#define CPU_FREQ                    (48000000)
#define USING_BOSCH_BP
#define SAMPLE_TIME_1               (53)
#define SAMPLE_TIME_2               (26)
#define SAMPLE_TIME_4               (13)
#define SAMPLE_TIME_6               (8)
#define SAMPLE_TIME_8               (6)
#define SAMPLE_TIME_10              (5)
#define NUM_AVGR_SUMS               2 //x^2 frames

//***** Function Prototypes *****
void startCrystalOscillator(void);
void setSystemClock(uint32_t CPU_Frequency);
void configureGPIO(void);
void startWakeUpTimerA(uint16_t ulClockMS);
void stopWakeUpTimerA(void);
int32_t movingAvg(int prevAvg, int16_t newValue);


//***** Global Data *****
const uint8_t wdtWakeUpPeriod [8] = {
        WDT_A_CLOCKDIVIDER_2G,
        WDT_A_CLOCKDIVIDER_128M,
        WDT_A_CLOCKDIVIDER_8192K,
        WDT_A_CLOCKDIVIDER_512K,
        WDT_A_CLOCKDIVIDER_32K,
        WDT_A_CLOCKDIVIDER_8192,
        WDT_A_CLOCKDIVIDER_512,
        WDT_A_CLOCKDIVIDER_64,
};

const uint8_t timeSamplesBMI [6] = {
        SAMPLE_TIME_1,      //Sample at 1 time per second
        SAMPLE_TIME_2,      //Sample at 2 times per second
        SAMPLE_TIME_4,      //Sample at 4 times per second
        SAMPLE_TIME_6,      //Sample at 6 times per second
        SAMPLE_TIME_8,      //Sample at 8 times per second
        SAMPLE_TIME_10,     //Sample at 10 times per second
};

const uint8_t timeSamplesBMM [6] = {
        SAMPLE_TIME_1,      //Sample at 1 time per second
        SAMPLE_TIME_2,      //Sample at 2 times per second
        SAMPLE_TIME_4,      //Sample at 4 times per second
        SAMPLE_TIME_6,      //Sample at 6 times per second
        SAMPLE_TIME_8,      //Sample at 8 times per second
        SAMPLE_TIME_10,     //Sample at 10 times per second
};

const uint8_t timeSamplesBME [6] = {
        SAMPLE_TIME_1,      //Sample at 1 time per second
        SAMPLE_TIME_2,      //Sample at 2 times per second
        SAMPLE_TIME_4,      //Sample at 4 times per second
        SAMPLE_TIME_6,      //Sample at 6 times per second
        SAMPLE_TIME_8,      //Sample at 8 times per second
        SAMPLE_TIME_10,     //Sample at 10 times per second
};

const uint8_t timeSamplesTMP [6] = {
        SAMPLE_TIME_1,      //Sample at 1 time per second
        SAMPLE_TIME_2,      //Sample at 2 times per second
        SAMPLE_TIME_4,      //Sample at 4 times per second
        SAMPLE_TIME_6,      //Sample at 6 times per second
        SAMPLE_TIME_8,      //Sample at 8 times per second
        SAMPLE_TIME_10,     //Sample at 10 times per second
};

const uint8_t timeSamplesOPT [6] = {
        SAMPLE_TIME_1,      //Sample at 1 time per second
        SAMPLE_TIME_2,      //Sample at 2 times per second
        SAMPLE_TIME_4,      //Sample at 4 times per second
        SAMPLE_TIME_6,      //Sample at 6 times per second
        SAMPLE_TIME_8,      //Sample at 8 times per second
        SAMPLE_TIME_10,     //Sample at 10 times per second
};

volatile uint8_t wdtWakeUpPeriodIndex = 6;
/**
 * END: end of code required for the Booster Pack sensors and I2C to work properly.
 */

// Global Variables
volatile int interruptCounter = 0;
volatile uint8_t reflectanceData;
volatile uint8_t bumpData;

// Function Declarations
void InitializeLEDPortPins(void);
void InitializeSwitchPortPins(void);

// Interrupt Task Function
void InterruptTask(void){
    // Increment Counter
    interruptCounter++;

    // Call Reflectance_Start() on every tenth interrupt, starting with the first
    if (interruptCounter % 10 == 1) {
        Reflectance_Start();
    }
    // Call Reflectance_End() on every tenth interrupt, starting with the second
    // Store the data in a global variable
    else if (interruptCounter % 10 == 2) {
        reflectanceData = Reflectance_End();
    }
    // Call Bump_Read() on every tenth interrupt, starting with the third
    // Store the data in a global variable
    else if (interruptCounter % 10 == 3) {
        bumpData = Bump_Read();
    }
}

//Calibration off-sets
int8_t accel_off_x;
int8_t accel_off_y;
int8_t accel_off_z;
// BMI160
BMI160_RETURN_FUNCTION_TYPE returnValue;
struct bmi160_accel_t       s_accelXYZ;
int returnRslt;
float convertedLux;
float GSM;

int main(void) {
    // Stop watchdog timer; initialize clock, motors, and interrupts
    DisableInterrupts();
    WDT_A_holdTimer();
    Clock_Init();
    EnableInterrupts();
    Motor_Init();

    // Intialize I2C to communicate with booster pack
    initI2C();

    // Initiliaze Nokia Display and UART0 for output and debugging
    UART0_Init();
    Nokia5110_Init();
    Nokia5110_Clear();


    // Define FSM and switches
    LF_FSMType LineFollower_FSM;
    BP_FSMType BoosterPack_FSM;
    SwitchDefine Switch1, Switch2;

    // Initialize I/O pins
    InitializeLEDPortPins();
    InitializeSwitchPortPins();

    //Initialize switches S1 and S2 to active low,
    //Logic 0  <->  Active   (Switch pressed)
    //Logic 1  <->  Inactive (Switch released)
    InitializeSwitch(&Switch1, (uint8_t *) &(SWITCH1_PORT->IN), (uint8_t) SWITCH1_BIT, Active, Inactive);
    InitializeSwitch(&Switch2, (uint8_t *) &(SWITCH2_PORT->IN), (uint8_t) SWITCH2_BIT, Active, Inactive);

    // Initialize FSM
    LF_InitializeFSM(&LineFollower_FSM);

    BP_InitializeFSM(&BoosterPack_FSM);

    // Initialize Reflectance Sensor, Bump Sensor, and Timer A
    Reflectance_Init();
    Bump_Init();
    TimerA1_Init(&InterruptTask, 500);

    //Intialize opt3001 sensor
    sensorOpt3001Init();
    sensorOpt3001Enable(true);

    //Intialize BMI160 sensor
    bmi160_initialize_sensor();
    returnRslt = bmi160_config_running_mode(APPLICATION_NAVIGATION); //there are several modes to run the BMI sensor in
    bmi160_accel_foc_trigger_xyz(0x03, 0x03, 0x01, &accel_off_x, &accel_off_y, &accel_off_z);

    // Main loop
    while(1) {
        // Read and convert OPT values
        uint16_t rawData;
        sensorOpt3001Read(&rawData);
        sensorOpt3001Convert(rawData, &convertedLux);

        // Read Accel values
        returnValue = bmi160_read_accel_xyz(&s_accelXYZ);

        // Pass inputs to Booster Pack FSM
        BoosterPack_FSM.LightSensor = convertedLux;
        BoosterPack_FSM.AccelZ = s_accelXYZ.z - Z_BASIS;
        BoosterPack_FSM.BumpSensor = Bump_Detect(bumpData);
        BoosterPack_FSM.InputS1 = ReadSwitchStatus(&Switch1);
        BoosterPack_FSM.InputS2 = ReadSwitchStatus(&Switch2);

        // Compute Next State of Booster Pack FSM
        BoosterPack_FSM.CurrentState = BP_NextStateFunction(&BoosterPack_FSM);

        // Calculate Booster Pack FSM Outputs
        GSM = BP_OutputFunction(&BoosterPack_FSM);

        // Pass inputs to Line Follower FSM
        LineFollower_FSM.LineSensorPosition = Reflectance_Position(reflectanceData);
        LineFollower_FSM.GSM = GSM;

        // Compute Next State of Line Follower FSM
        LineFollower_FSM.CurrentState = LF_NextStateFunction(&LineFollower_FSM);

        // Print current conditions to LCD
        // Reflectance Conditions
        Nokia5110_SetCursor(2, 0);
        uint8_t localData = reflectanceData;
        // Convert the reflectance data decimal to binary string
        int i;
        for (i = 0; i < 8; i++) {
            Nokia5110_OutChar(localData & 0x80 ? '1' : '0');
            localData <<= 1;
        }

        // Line Follower State
        Nokia5110_SetCursor(2, 1);
        Nokia5110_OutString(LF_getStateString(LineFollower_FSM.CurrentState));

        // Booster Pack State
        Nokia5110_SetCursor(2,2);
        Nokia5110_OutString(BP_getStateString(BoosterPack_FSM.CurrentState));

        // Light Conditions
        Nokia5110_SetCursor(2,3);
        if (convertedLux < LIGHT_DARK_THRESHOLD) {
            Nokia5110_OutString("Dark  ");
        } else if (convertedLux < LIGHT_DIM_THRESHOLD) {
            Nokia5110_OutString("Dim   ");
        } else {
            Nokia5110_OutString("Bright");
        }

        // Bump Conditions
        Nokia5110_SetCursor(2,4);
        if (BoosterPack_FSM.BumpSensor == Bumped) {
            Nokia5110_OutString("Bumped  ");
        } else {
            Nokia5110_OutString("Unbumped");
        }

        // Produce output of Line Follower FSM
        LF_OutputFunction(&LineFollower_FSM);

        /**
         * START: start of code required for the Booster Pack sensors and I2C to work properly.
         * Copied from the provided booster pack GUI main code.
         */

        // Configure WDT
        // For LPM3 Clock Source should be BCLK or VLOCLK
        MAP_WDT_A_initIntervalTimer(WDT_A_CLOCKSOURCE_BCLK/*WDT_A_CLOCKSOURCE_ACLK*/,
                wdtWakeUpPeriod[wdtWakeUpPeriodIndex]);
        MAP_Interrupt_enableInterrupt(INT_WDT_A);

        // Start WDT
        MAP_WDT_A_startTimer();

        //Go to LPM0 (Cannot use LPM3 because we won't accurately receive UART data)
        MAP_PCM_gotoLPM0();

        /**
         * END: end of code required for the Booster Pack sensors and I2C to work properly.
         */
    }
}

/**
 * Initializes the value and port pin direction of the brake lights and turn signals.
 * Sets brake light and turn signal LEDs as output and starting off.
 */
void InitializeLEDPortPins(void) {
    TURN_OFF_LED_LB;
    SET_LED_LB_AS_AN_OUTPUT;
    TURN_OFF_LED_RB;
    SET_LED_RB_AS_AN_OUTPUT;
    TURN_OFF_LED_LT;
    SET_LED_LT_AS_AN_OUTPUT;
    TURN_OFF_LED_RT;
    SET_LED_RT_AS_AN_OUTPUT;
}

/**
 * Configure port pin for switches.
 * Sets both S1 and S2 as input with pull-up resistors
 */
void InitializeSwitchPortPins(void) {
    ENABLE_PULL_UP_PULL_DOWN_RESISTORS_S1;
    ENABLE_PULL_UP_PULL_DOWN_RESISTORS_S2;
    SELECT_PULL_UP_RESISTORS_S1;
    SELECT_PULL_UP_RESISTORS_S2;
    SET_SWITCH1_TO_AN_INPUT;
    SET_SWITCH2_TO_AN_INPUT;
}

/**
 * START: start of code required for the Booster Pack sensors and I2C to work properly.
 * Copied from the provided booster pack GUI main code.
 */

/***********************************************************
  Function: WDT_A_IRQHandler
 */
void WDT_A_IRQHandler(void)
{
    //MAP_GPIO_setOutputHighOnPin(GPIO_PORT_P1, GPIO_PIN0);
    // Waking up from LMP3 take us to PCM_AM_LDO_VCORE0 instead of PCM_AM_LF_VCORE0
//  MAP_PCM_setPowerState(PCM_AM_LDO_VCORE0);
//    MAP_PCM_setCoreVoltageLevel(PCM_AM_DCDC_VCORE0);

#ifdef USE_LPM
    MAP_Interrupt_disableSleepOnIsrExit();
#endif
}

/**
 * END: end of code required for the Booster Pack sensors and I2C to work properly.
 */
