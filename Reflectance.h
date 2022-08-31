/**
 * @file      Reflectance.h
 * @brief     Provides low-level functions for interfacing with Reflectance sensors
 ******************************************************************************/


/*
 * Reflectance.h
 */
#ifndef REFLECTANCE_H_
#define REFLECTANCE_H_

#include <stdint.h>

// Reflectance Center State Type Definitions
typedef enum {
    ON_LINE,
    MORE_LEFT,
    MORE_RIGHT,
    LEFT,
    RIGHT,
    LOST,
    STOP
} ReflectanceStatus;

/**
 * Initialize reflectance sensor
 * LED Output: P5.3
 * Sensor 1-8 (right to left) P7.0 - P7.7
 * @param none
 * @return none
 */
void Reflectance_Init(void);

/**
 * Determine robot status over the line
 * @param data the read reflectance data
 * @return result the robot location status (LOST/RIGHT/LEFT/ON LINE/STOP/MORE_LEFT/MORE_RIGHT)
 */
ReflectanceStatus Reflectance_Center(uint8_t data);

/**
 * Determine the count of activated reflectance sensors
 * @param data the read reflectance data
 * @return result the count of activated reflectance sensors
 */
int8_t Reflectance_Count(uint8_t data);

/**
 * Determine the sum of activated sensor positions measured from the center
 * @param data the collected sensor data
 * @return result the sum of activated sensor position values (positive - left, negative - right)
 *         A special code of 1000 is given for Lost (i.e. the sensor is all 0s).
 */
int32_t Reflectance_Position(uint8_t data);

/*
 * Start collecting reflectance sensor data by turning on the LEDs, charging the capacitors, waiting,
 * and then set the sensor pins as input
 * @param none
 * @return none
 */
void Reflectance_Start(void);

/* Finish collecting reflectance sensor data by reading the result from the P7 input register and turning off the LEDs
 * @param none
 * @return result the sensor data
 */
uint8_t Reflectance_End(void);


#endif /* REFLECTANCE_H_ */
