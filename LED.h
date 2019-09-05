#include <avr/io.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>

//----------defines----------
// #define L1 0xD2     // D2 (01) BRN
// #define L2 0xD3     // D3 (02) GRN
// #define L3 0xD4     // D4 (03) PRP
// #define L4 0xD5     // D5 (04) YEL
// #define L5 0xD6     // D6 (05) ORG
// #define L6 0xD7     // D7 (06) RED
// #define L7 0xB0     // B0 (07) PRP
// #define L8 0xB1     // B1 (08) BRN
// #define L9 0xB2     // B2 (09) GRN
// #define L10 0xB6    // B6 (10) WHT
// #define L11 0xB7    // B7 (11) GRY
// #define L12 0xC0    // C0 (12) RED
// #define L1314 0xC1  // C1 (13,14) GRY (inverse)
// #define L1516 0xC2  // C2 (15,16) YEL (inverse)
// #define L1718 0xC3  // C3 (17,18) ORG (inverse)
// #define L1920 0xC4  // C4 (19,20) WHT (inverse)
// #define BRK_SW 0xC5 // C5 BRK input

// #define BSET 0b11000111
// #define CSET 0b00011111
// #define DSET 0b11111110

#define L1 0xD2     // D2 (01) BRN
#define L2 0xD4     // D3 (02) GRN
#define L3 0xB7     // D4 (03) PRP
#define L4 0xD5     // D5 (04) YEL
#define L5 0xD6     // D6 (05) ORG
#define L6 0xB0     // D7 (06) RED
#define L7 0xB1     // B0 (07) PRP
#define L8 0xC0     // B1 (08) BRN
#define L9 0xC1     // B2 (09) GRN
#define L10 0xC2    // B6 (10) WHT
#define L11 0xC3    // B7 (11) GRY
#define L12 0xC5    // C0 (12) RED
#define L1314 0xD3  // C1 (13,14) GRY (inverse)
#define L1516 0xD7  // C2 (15,16) YEL (inverse)
#define L1718 0xB2  // C3 (17,18) ORG (inverse)
#define L1920 0xC4  // C4 (19,20) WHT (inverse)
#define BRK_SW 0xB6 // C5 BRK input

#define BSET 0b10000111
#define CSET 0b00111111
#define DSET 0b11111100

#define REG(x) ((x >> 4) - 0xb)
#define PIN(x) (1 << (x & 0b1111))

//------------------------------------
// initalize the pins to defaults------
//------------------------------------
void pins_init();
//------------------------------------
// resets all the temp registers to 0--
//------------------------------------
void clear_all();
//------------------------------------
// setts all the temp registers to 'on'
// using the b/c/d SET values as mask
//------------------------------------
void set_all();
//------------------------------------
//------turns one pin on at a time----
//------------------------------------
void On(uint8_t v);
//------------------------------------
//-turns multiple pins on via array---
//------------------------------------
void On_ar(uint8_t cnt, uint8_t *arr);
//------------------------------------
//-turns multiple pins on via var arry
//------------------------------------
void On_(uint8_t cnt, ...);
//------------------------------------
//-turns a single pin off-------------
//------------------------------------
void Off(uint8_t v);
//------------------------------------
//-turns multiple pis off via array---
//------------------------------------
void Off_(uint8_t cnt, ...);
//------------------------------------
//-turns multiple pins off via var arr
//------------------------------------
void Off_ar(uint8_t cnt, uint8_t *arr);
//------------------------------------
// ask if a pin is on from the temp reg
//------------------------------------
bool t_value(uint8_t v);
//------------------------------------
//--ask if a pin is on from live reg--
//------------------------------------
bool l_value(uint8_t v);
//------------------------------------
// syncs temp register to live regs----
//------------------------------------
void sync();
