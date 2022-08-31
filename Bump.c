/* DriverLib Includes */
//#include "driverlib.h"
#include "msp.h"

/* Standard Includes */
#include <stdint.h>
#include <stdbool.h>

/* Project Includes */
#include "Bump.h"
#include "gpio.h"
#include "UART0.h"

// Define statements for bump switch pins
#define BUMP_PORT   GPIO_PORT_P4
#define BUMP0       GPIO_PIN0   // P4.0
#define BUMP1       GPIO_PIN2   // P4.2
#define BUMP2       GPIO_PIN3   // P4.3
#define BUMP3       GPIO_PIN5   // P4.5
#define BUMP4       GPIO_PIN6   // P4.6
#define BUMP5       GPIO_PIN7   // P4.7
#define BUMP_PINS   (BUMP0 | BUMP1 | BUMP2| BUMP3| BUMP4 | BUMP5)

/*
 * Initialize bump sensor pins as GPIO inputs with pull up resistors.
 * Bump switches are active low
 * Port 4, Pins 0, 2, 3, 5, 6, and 7
 * @param none
 * @return none
 */
void Bump_Init(void){
    // Uses GPIO to initialize bump switch pins
    GPIO_setAsInputPinWithPullUpResistor(BUMP_PORT, BUMP_PINS);
}

/*
 * Read the state of the bump switches using positive logic
 * @param none
 * @return uint8_t variable, bits 0-5 correspond to
 * the values of bump switches 0-5 (right to left on the robot)
 */
uint8_t Bump_Read(void){
    // Retrieve input from each bump sensor and invert it
    uint8_t b0 = (0x01^GPIO_getInputPinValue(BUMP_PORT, BUMP0));
    uint8_t b1 = (0x01^GPIO_getInputPinValue(BUMP_PORT, BUMP1)) << 1;
    uint8_t b2 = (0x01^GPIO_getInputPinValue(BUMP_PORT, BUMP2)) << 2;
    uint8_t b3 = (0x01^GPIO_getInputPinValue(BUMP_PORT, BUMP3)) << 3;
    uint8_t b4 = (0x01^GPIO_getInputPinValue(BUMP_PORT, BUMP4)) << 4;
    uint8_t b5 = (0x01^GPIO_getInputPinValue(BUMP_PORT, BUMP5)) << 5;

    // Combine each bump switch input
    return (b0 | b1 | b2 | b3 | b4 | b5);
}

/**
 * Determine the bump status
 * If any bump switch is activated the bump status is bumped, unbumped otherwise
 * @param data the read bump data
 * @return result the bump status
 */
BumpStatus Bump_Detect(uint8_t data) {
    if (data == 0x00) {
        return Unbumped;
    }
    return Bumped;
}
