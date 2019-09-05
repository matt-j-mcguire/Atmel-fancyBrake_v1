#include "LED.h"
#include <stdbool.h>
#include <stdint.h>
#include <util/delay.h>
//#include "USART.h"
#include "timer.h"
#include <avr/wdt.h>

#define STATE_BOOT 0
#define STATE_NORMAL 1
#define STATE_ONBRAKE 2
#define STATE_BRAKE 3
#define STATE_OFFBRAKE 4
#define MAX 11

bool isBrakeing();
void Sequence_Boot();
void Sequence_Normal();
void Sequence_Brake();
void Sequence_OnBrake(bool brkstate);
void Sequence_OffBrake(bool brkstate);

uint8_t outer[12] = {L1, L2, L3, L4, L5, L6, L7, L8, L9, L10, L11, L12};
uint8_t inner[4] = {L1314, L1516, L1718, L1920}; // invered outputs
uint8_t tReg[3];
uint8_t state;
bool _last_brake;


int main(void) {
  wdt_disable();
  // initUSART();
  pins_init();
  timer1B_start();
  state = STATE_BOOT;

  wdt_enable(WDTO_500MS);

  while (1) {
    wdt_reset();
    bool isBrake = isBrakeing();

    if (state != STATE_BOOT) {

      if (isBrake && !_last_brake) {
        state = STATE_ONBRAKE;
      } else if (!isBrake && _last_brake) {
        state = STATE_OFFBRAKE;
      }
    }
    _last_brake = isBrake;

    switch (state) {
    case STATE_BOOT:
      Sequence_Boot();
      break;
    case STATE_ONBRAKE:
      Sequence_OnBrake(isBrake);
      break;
    case STATE_BRAKE:
      Sequence_Brake();
      break;
    case STATE_OFFBRAKE:
      Sequence_OffBrake(isBrake);
      break;
    default: // STATE_NORMAL
      Sequence_Normal();
      break;
    }
  }
  return 0;
}

//------------------------------------
// return the brake switch is triggered
//------------------------------------
bool isBrakeing() {
  static uint32_t lasttm;
  static bool last;
  bool state = l_value(BRK_SW);
  static bool ret = false;

  if (last != state) {
    lasttm = timer1B_next(100);
    last = state;
  }

  if (timer1B_past(lasttm)) {
    ret = !last;
  }

  return ret;
}

//------------------------------------
// Boot animation
//------------------------------------
void Sequence_Boot() {
  static uint16_t value;
  static uint32_t next;

  On_ar(4, inner); // turn off the center
  if (timer1B_past(next)) {

    // fill pattern
    if (value == 4095) { // 12bits filled
      value = 0;
      state = STATE_NORMAL;
      for (int i = 0; i < 6; i++) {
        if (i & 1) {
          Off_ar(4, inner);
          On_ar(12, outer);
        } else {
          On_ar(4, inner);
          Off_ar(12, outer);
        }
        sync();
        _delay_ms(100);
        wdt_reset();
      }

      return;
    }

    bool fnd = false;
    for (int i = MAX; i > -1; i--) {
      if (1 << i & value && i < MAX) {
        if ((1 << (i + 1) & value) == 0) {
          fnd = true;
          // remove the old bit
          value = value ^ (1 << i);
          // place it in a higher spot
          value = value | (1 << (i + 1));
          i = -1;
        }
      }
    }

    if (fnd == false)
      value++;

    for (int i = 0; i <= MAX; i++) {
      if (1 << i & value)
        On(outer[i]);
      else
        Off(outer[i]);
    }
    wdt_reset();
    sync();
    next = timer1B_next(25);
  }
}

//------------------------------------
// normal operation
//------------------------------------
void Sequence_Normal() {
  static uint32_t next;
  static uint8_t inx;

  if (timer1B_past(next)) {
    On_ar(4, inner); // keep the center off (inverted)
    inx = (inx + 1) % 12;
    uint8_t inx2 = (inx + 1) % 12;
    uint8_t sinx = (inx + 6) % 12;
    uint8_t sinx2 = (sinx + 1) % 12;

    for (int i = 0; i < 12; i++) {
      if (i == inx || i == sinx || i == inx2 || i == sinx2)
        On(outer[i]);
      else
        Off(outer[i]);
    }

    sync();
    next = timer1B_next(50);
  }
}

//------------------------------------
// Brakeing operation
//------------------------------------
void Sequence_Brake() {
  Off_ar(4, inner);
  On_ar(12, outer);
  sync();
}

//------------------------------------
// Brakeing just starting operation
//------------------------------------
void Sequence_OnBrake(bool brkstate) {
  static uint32_t next;
  static uint8_t cnt = 0;

  if (!brkstate) { // brake state changed, exit the sequence
    state = STATE_NORMAL;
    cnt = 0;
    return;
  }

  if (timer1B_past(next)) {
    if (cnt & 1) {
      On_ar(4, inner);
      On_ar(12, outer);
    } else {
      Off_ar(4, inner);
      Off_ar(12, outer);
    }
    sync();
    next = timer1B_next(100);
    cnt++;

    if (cnt == 6) { // sequence is done, goto brake mode
      state = STATE_BRAKE;
      cnt = 0;
    }
  }
}

//------------------------------------
// Brakeing just stopped operation
//------------------------------------
void Sequence_OffBrake(bool brkstate) {
  static uint32_t next;
  static uint8_t cnt = 0;

  if (brkstate) { // brake state changed, exit the sequence
    state = STATE_NORMAL;
    cnt = 0;
    return;
  }

  if (timer1B_past(next)) {
    On_ar(4, inner);
    uint16_t tmr = 100;
    Off_ar(12, outer);

    switch (cnt) {
    case 0:
      On_(3, L1, L11, L6);
      break;
    case 1:
      On_(5, L1, L11, L5, L6, L7);
      break;
    case 2:
      On_(7, L1, L11, L4, L5, L6, L7, L8);
      tmr = 300;
      break;
    case 3:
      On_(7, L2, L11, L4, L5, L6, L7, L8);
      tmr = 300;
      break;
    case 4:
    case 5:
      On_(7, L1, L11, L4, L5, L6, L7, L8);
      tmr = 300;
      break;
    default:
      break;
    }

    sync();
    cnt++;
    next = timer1B_next(tmr);
    if (cnt == 6) {
      state = STATE_NORMAL;
      cnt = 0;
    }
  }
}
