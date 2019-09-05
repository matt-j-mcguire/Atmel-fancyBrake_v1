#include "timer.h"

uint16_t _last; // last 16bit read from the timer
uint32_t _curr; // current 32bit value

// CS12  CS11    CS10
// 0     0       0       no clock source
// 0     0       1       clk/1   ~1/1000 milliseconds
// 0     1       0       clk/8   ~1/125 milliseconds
// 0     1       1       clk/64  ~1/15.6 milliseconds
// 1     0       0       clk/256 ~1/4 milliseconds
// 1     0       1       clk/1024 ~1 millisecond

//------------------------------------
// Starts the timer for ~1ms resolution
//------------------------------------
void timer1B_start() {
  TCCR1B |= (1 << CS12) | (1 << CS10);
  _last = 0;
  _curr = 0;
}

//------------------------------------
// Stops the timer
//------------------------------------
void timer1B_stop() { TCCR1B = (TCCR1B & ~((1 << CS12) | (1 << CS10))); }

//------------------------------------
//(private) reads the 16bit register
// monitors for roll over, and puts the
// result in to a 32bit uint, that will
// roll over at 49.7 days
//------------------------------------
uint32_t Read1() {
  uint8_t sreg = SREG; // Save global interrupt flag
  cli();               // Disable interrupts
  uint16_t i = TCNT1;  // read the current value
  SREG = sreg;         // Restore global interrupt flag

  if (i < _last) // roll over
    _curr += (0xffff - _last) + i;
  else
    _curr += (i - _last);

  _last = i;
  return _curr;
}

//------------------------------------
//(private) sets the 16bit register,
//------------------------------------
void write1(uint16_t i) {
  uint8_t sreg = SREG; // Save global interrupt flag
  cli();               // Disable interrupts
  TCNT1 = i;           // set the value
  SREG = sreg;         // Restore global interrupt flag
}

//------------------------------------
// resets the current timer counts
//------------------------------------
void timer1B_reset() {
  write1(0);
  _last = 0;
  _curr = 0;
}

//------------------------------------
// only returns the last 16bit count
// this resets every 64 seconds
//------------------------------------
uint16_t timer1B_ticks() { return (uint16_t)Read1(); }

//------------------------------------
// returns a value for a ms time in
// the future, max future 64 seconds
//------------------------------------
uint32_t timer1B_next(uint16_t ms) {
  uint32_t c = Read1();
  c += ms;
  return c;
}

//------------------------------------
// test to see if a value has already
// past
//------------------------------------
bool timer1B_past(uint32_t value) { return (Read1() > value); }

//------------------------------------
// returns the current 32bit counter
// value rools over after 49.7 days
//------------------------------------
uint32_t timer1B() { return Read1(); }