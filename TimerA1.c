/* DriverLib Includes */
#include "driverlib.h"

/* Standard Includes */
#include <stdint.h>
#include <stdbool.h>

/* Project Includes */
#include "TimerA1.h"

// Pointer to user function that gets called on timer interrupt
void (*TimerTask)(void);

// Configuration for upMode at 500kHz and with CCR0 interrupt
Timer_A_UpModeConfig config =
{
 TIMER_A_CLOCKSOURCE_SMCLK,
 TIMER_A_CLOCKSOURCE_DIVIDER_24,
 99,
 TIMER_A_TAIE_INTERRUPT_DISABLE,
 TIMER_A_CCIE_CCR0_INTERRUPT_ENABLE,
 TIMER_A_DO_CLEAR
};

/*
 * Initialize Timer A1 at 500 kHz to run a periodic interrupt
 * @param task: function pointer to user function to run on interrupt
 * @param period: Timer period
 * @note Timer source is SMCLK, assumes SMCLK is initialized at 12 MHz
 */
void TimerA1_Init(void(*task)(void), uint16_t period){
    // Set the interrupt task to the global pointer
    TimerTask = task;

    // Set the timer period
    config.timerPeriod = period;

    // Configure TimerA1 in up mode at 500Hz and with CCR0 interrupt
    Timer_A_configureUpMode(TIMER_A1_BASE, &config);

    // Enable the NVIC TimerA1 CCR0 interrupt
    Interrupt_enableInterrupt(INT_TA1_0);

    // Enable NVIC interrupts
    Interrupt_enableMaster();

    // Start TimerA1
    Timer_A_startCounter(TIMER_A1_BASE, TIMER_A_UP_MODE);
}

/*
 * Stops periodic interrupt on Timer A1
 * @param none
 * @return none
 */
void TimerA1_Stop(void){
    // Set the MC bits (4 and 5) of the TimerA1 CTL register to STOP
    uint16_t reg = TIMER_A1->CTL & ~(0x03 << 4);
    TIMER_A1->CTL = reg;
}

/*
 * ISR for Timer A1 Periodic Interrupt from CCR0
 */
void TA1_0_IRQHandler(void){
    // Call the timer task initialized in TimerA1_Init()
    TimerTask();

    // Clear the CRR0 interrupt flag
    Timer_A_clearCaptureCompareInterrupt(TIMER_A1_BASE, TIMER_A_CAPTURECOMPARE_REGISTER_0);
}
