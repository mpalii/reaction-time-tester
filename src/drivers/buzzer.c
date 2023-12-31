#include "buzzer.h"
#include "gpio.h"

void init_buzzer(void)
{
    /******************************************************************/
    /* Set up Timer/Counter2 for 440Hz frequency generation           */
    /* the passive buzzer will generate sound with 880Hz pitch        */
    /* (256 (prescaler) * 82(OCR2A + 1)) / 18_432_000 Hz = 0.001139s  */
    /******************************************************************/
	
    // TCCR2A - Timer/Counter2 Control Register channel A
    TCCR2A |= _BV(WGM21); // Waveform Generation Mode - CTC

    // OCR2A - Timer/Counter2 Output Compare Register channel A
    OCR2A = 81; //set the compare reg to 82 ticks (zero based)
	
    // TCCR2B - Timer/Counter2 Control Register channel B - clkT2S/256
    TCCR2B |= _BV(CS22) | _BV(CS21);
}

void buzzer_on(void)
{
    // TCCR2A - Timer/Counter2 Control Register channel A
    TCCR2A |= _BV(COM2A0);  // Enable OC2A toggling on compare match
}

void buzzer_off(void)
{
    // TCCR2A - Timer/Counter2 Control Register channel A
    TCCR2A &= ~_BV(COM2A0); // Normal port operation, OC2A disconnected
	
    // System can stop Timer/Counter2 when pin will be driven high
    GPIO_LOW(BUZZER);
}
