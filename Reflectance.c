#include <stdint.h>
/*
 * Reflectance.c
 * Provides functions to
 * 1. Initialize Pololu reflectance sensor
 * 2. Read Pololu reflectance sensor
 * 3. Determine robot position
 * 4. Determine robot status
 *
 */

#include "driverlib.h"

#include "msp.h"
#include "Clock.h"
#include "Reflectance.h"

#define ALLBITS  0xFF
#define BITSHIFT 0x01

//------------Reflectance_Init------------
// Initialize sensor array to GPIO, set LEDs (P5.3 and P9.2)
// as output and sensors (P7.0-P7.7) as output
// Input: none
// Output: none
void Reflectance_Init(void){
    //Initialize even LED on P5.3 as GPIO output
    GPIO_setAsOutputPin(GPIO_PORT_P5, GPIO_PIN3);

    //Initialize odd LED on P9.2 as GPIO output
    GPIO_setAsOutputPin(GPIO_PORT_P9, GPIO_PIN2);

    //Initialize reflectance sensor as GPIO output
    GPIO_setAsOutputPin(GPIO_PORT_P7, PIN_ALL8);
}

//------------Reflectance_Start------------
// Start collecting reflectance sensor data by turning on the LEDs, charging the capacitors, waiting,
// and then set the sensor pins as input
// Input: none
// Output: none
void Reflectance_Start(){
    //Turn on LEDs
    GPIO_setOutputHighOnPin(GPIO_PORT_P5, GPIO_PIN3);
    GPIO_setOutputHighOnPin(GPIO_PORT_P9, GPIO_PIN2);

    //Charge reflectance sensor capacitors
    GPIO_setAsOutputPin(GPIO_PORT_P7, PIN_ALL8);
    GPIO_setOutputHighOnPin(GPIO_PORT_P7, PIN_ALL8);

    //Wait 10us for capacitors to charge
    Clock_Delay1us(10);
    //Set reflectance sensor as input
    GPIO_setAsInputPin(GPIO_PORT_P7, PIN_ALL8);
}

//------------Reflectance_End---------------
// Finish collecting reflectance sensor data by reading the result from the P7 input register and turning off the LEDs
// Input: none
// Output: result the sensor data
uint8_t Reflectance_End() {
    //Read reflectance sensor values
    //Assign to result
    uint8_t result = P7->IN;
    //Turn off LEDs
    GPIO_setOutputLowOnPin(GPIO_PORT_P5, GPIO_PIN3);
    GPIO_setOutputLowOnPin(GPIO_PORT_P9, GPIO_PIN2);
    //Return result, the sensor readings
    return result;
}

//------------Reflectance_Center------------
// Determine robot's status over the line
// Input: the read reflectance data
// Output: result the robot location status (LOST/RIGHT/LEFT/ON LINE/STOP/MORE_LEFT/MORE_RIGHT)
ReflectanceStatus Reflectance_Center(uint8_t data) {
    uint8_t leftBits = (data & 0xF0) >> 4;
    uint8_t rightBits = data & 0x0F;
    if (data == 0b10011001) {
        return STOP;
    } else if (data == 0b00011000) {
        return ON_LINE;
    } else if (leftBits != 0 && rightBits != 0) {
        if (leftBits >= rightBits) {
            return MORE_LEFT;
        } else {
            return MORE_RIGHT;
        }
    } else if(leftBits != 0 && rightBits == 0) {
        return RIGHT;
    } else if(leftBits == 0 && rightBits != 0) {
        return LEFT;
    } else if(leftBits == 0 && rightBits == 0) {
        return LOST; // off road
    }

    return LOST;
}

//------------Reflectance_Position------------
// Determine the sum of activated sensor positions measured from the center
// Input: the collected sensor data
// Output: the sum of activated sensor position values (positive - left, negative - right).
//         A special code of 1000 is given for Lost (i.e. the sensor is all 0s).
int32_t Reflectance_Position(uint8_t data){
    //Initialize weight vector
    //L--->R
    int16_t weight[8] = {334, 238, 142, 48, -48, -142, -238, -334};
    //Initialize bit vector to hold sensor data
    uint8_t bits[8] = {};
    //Initialize other necessary variables
    int16_t num = 0;
    int16_t result = 0;
    uint8_t denom = 0;
    uint8_t i;

    //
    if (data == 0b00000000) {
        return 1000;
    }

    //Fill bit vector with sensor data
    for(i=0; i<8; i++) {
        bits[i] = (data >> i) & BITSHIFT;
    }
    //Num = sum(bit_reading*weight)
    //Denom = sum(bit readings)
    for(i=0; i<8; i++) {
        num += bits[i] * weight[i];
        denom += bits[i];
    }
    if(denom != 0){
        result = num;
    }
    //Return result, the robot position
    return result;
}


/**
 * Determine the count of activated reflectance sensors
 * @param data the read reflectance data
 * @return result the count of activated reflectance sensors
 */
int8_t Reflectance_Count(uint8_t data) {
    uint8_t localData = data;
    int8_t sum = 0;

    int i;
    for (i = 0; i < 8; i++) {
        sum += localData & 0x01;

        localData >>= 1;
    }

    return sum;
}
