#include <avr/interrupt.h>
#include <stdbool.h>
#include <stdint.h>

//------------------------------------
// Starts the timer for ~1ms resolution
//------------------------------------
void timer1B_start(void);
//------------------------------------
// Stops the timer
//------------------------------------
void timer1B_stop(void);
//------------------------------------
// resets the current timer counts
//------------------------------------
void timer1B_reset();
//------------------------------------
// only returns the last 16bit count
// this resets every 64 seconds
//------------------------------------
uint16_t timer1B_ticks(void);
//------------------------------------
// returns a value for a ms time in
// the future, max future 64 seconds
//------------------------------------
uint32_t timer1B_next(uint16_t ms);
//------------------------------------
// test to see if a value has already
// past
//------------------------------------
bool timer1B_past(uint32_t value);
//------------------------------------
// returns the current 32bit counter
// value rools over after 49.7 days
//------------------------------------
uint32_t timer1B();