#include "LED.h"

extern uint8_t outer[];
extern uint8_t inner[]; // invered outputs
extern uint8_t tReg[];

//--------------------------
// initalize the pins
//--------------------------
void pins_init() {
  // set inital input/output
  DDRB |= BSET;
  DDRC |= CSET;
  DDRD |= DSET;

  // create the temporary registers
  tReg[0] = PORTB;
  tReg[1] = PORTC;
  tReg[2] = PORTD;
}

//--------------------------
// resets all temp registers
// to 0
//--------------------------
void clear_all() {
  tReg[0] = 0;
  tReg[1] = 0;
  tReg[2] = 0;
}

//--------------------------
// sets all the output pins on
//--------------------------
void set_all() {
  tReg[0] = BSET;
  tReg[1] = CSET;
  tReg[2] = DSET;
}

//--------------------------
//------turns an pin on-----
//--------------------------
void On(uint8_t v) { tReg[REG(v)] |= PIN(v); }

//--------------------------
//-turns multiple pins on---
//--------------------------
void On_(uint8_t num, ...) {
  va_list valist;

  va_start(valist, num);

  /* access all the arguments assigned to valist */
  for (int i = 0; i < num; i++) {
    uint8_t v = va_arg(valist, int);
    tReg[REG(v)] |= PIN(v);
  }
  va_end(valist);
}

//--------------------------
//-turns multiple pins on---
//--------------------------
void On_ar(uint8_t cnt, uint8_t *arr) {
  /* access all the arguments assigned to valist */
  for (int i = 0; i < cnt; i++) {
    uint8_t v = *(arr + i);
    tReg[REG(v)] |= PIN(v);
  }
}

//--------------------------
//------turns an pin off----
//--------------------------
void Off(uint8_t v) {
  // if(t_value(v)) tReg[REG(v)] ^= PIN(v);
  tReg[REG(v)] &= ~PIN(v);
}

void Off_ar(uint8_t cnt, uint8_t *arr) {
  for (int i = 0; i < cnt; i++) {
    uint8_t v = *(arr + i);
    tReg[REG(v)] &= ~PIN(v);
  }
}

//--------------------------
//-turns multiple pins off--
//--------------------------
void Off_(uint8_t num, ...) {
  va_list valist;

  va_start(valist, num);

  for (int i = 0; i < num; i++) {
    uint8_t v = va_arg(valist, int);
    tReg[REG(v)] &= ~PIN(v);
  }
  va_end(valist);
}

//--------------------------
//--gets a temporary value--
//--------------------------
bool t_value(uint8_t v) { return tReg[REG(v)] & PIN(v); }

//--------------------------
//---gets a current value---
//--------------------------
bool l_value(uint8_t v) {
  switch (REG(v)) {
  case 0:
    return PINB & PIN(v);
    break;
  case 1:
    return PINC & PIN(v);
    break;
  case 2:
    return PIND & PIN(v);
    break;
  default:
    return false;
    break;
  }
}

//--------------------------
// syncs the temp to current-
//--------------------------
void sync() {
  // keep the brake input always on
  // On(BRK_SW);

  // clear only mask items and re-set the bits
  PORTB = (PORTB & ~BSET) | tReg[0];
  PORTC = (PORTC & ~CSET) | tReg[1];
  PORTD = (PORTD & ~DSET) | tReg[2];
}