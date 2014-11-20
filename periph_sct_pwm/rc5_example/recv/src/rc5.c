/********************************************************************
*
*  LPC4357 - RC5 decoder
*
*  Use SCT_IN0 input (capture events and interrupts on both edges)
*  Reference: AN10722
*  RC5 format:
*
*      | S | F | C |   5 sytem bits    |    6 command bits     |
*      | 1 | 1 | 0 | 0 | 0 | 0 | 0 | 1 | 1 | 0 | 1 | 1 | 1 | 1 |
*
*   -----+ +-+   +-+ +-+ +-+ +-+ +---+ +-+   +---+ +-+ +-+ +-+ +----
*        | | |   | | | | | | | | |   | | |   |   | | | | | | | |
*        +-+ +---+ +-+ +-+ +-+ +-+   +-+ +---+   +-+ +-+ +-+ +-+
*
*      |2HB|2HB|2HB|2HB|2HB|2HB|2HB|2HB|2HB|2HB|2HB|2HB|2HB|2HB|
*
*  2HB = 1778 usec
*
********************************************************************/
#include "chip.h"

#define HB             889                                 // half bit time = 1778/2 = 889 usec
#define MIN_HB         640                                 // minimum half bit time
#define MAX_HB         1140                                // maximum half bit time
#define MIN_2HB        1340                                // minimum full bit time
#define MAX_2HB        2220                                // maximum full bit time

uint8_t  RC5_System;                                       // format: 1 F C s4 s3 s3 s1 s0
uint8_t  RC5_Command;                                      // format: 0 0 c5 c4 c3 c2 c1 c0
uint8_t  RC5_flag;

static int32_t   low_time;                                 // to store and save captured low time
static int32_t   high_time;                                // to store and save captured high time
static uint8_t   prev;                                     // to save the previous half bit value
static uint16_t  frame;                                    // rc5 frame (max 14 bits)

static void RC5_Shift_Bit(uint8_t val)
{
    if (frame & 0x2000)                                    // frame full ?
        frame = 0;                                         // yes, ERROR
    else
        frame = (frame << 1) | val;                        // shift frame bit
}

/************************************************************************
; RC5_Decode (we only take action at a rising edge)
;
;  prev        Low Time        High Time     Action      new prev
; -------------------------------------------------------------------
;   0             0               0          Shift 0         0
;   0             0               1          Shift 1         1
;   0             1               0          -ERROR-         *
;   0             1               1          Shift 1,0       0
;   1             0               0          Shift 1         1
;   1             0               1          -ERROR-         *
;   1             1               0          Shift 1,0       0
;   1             1               1          -ERROR-         *
;
***********************************************************************/
static void RC5_Decode(void)
{
  uint8_t action;

    action = prev << 2;

// Check High Time

    if ((high_time > MIN_2HB) && (high_time < MAX_2HB))
        action = action | 1;                               // high_time = long
    else if (!((high_time > MIN_HB) && (high_time < MAX_HB)))
    {
        frame = 0;                                         // RC5 ERROR
        return;
    }

// Check Low Time

    if ((low_time > MIN_2HB) && (low_time < MAX_2HB))
        action = action | 2;                               // low_time = long
    else if (!((low_time > MIN_HB) && (low_time < MAX_HB)))
    {
        frame = 0;                                         // RC5 ERROR
        return;
    }

    switch (action)
    {
      case 0:  RC5_Shift_Bit(0);                           // short low, short high, shift 0
               break;
      case 1:  RC5_Shift_Bit(1);                           // short low, long high, shift 1
               prev = 1;                                   // new half bit is true
               break;
      case 2:  frame = 0;                                  // long low, short high, ERROR
      case 3:  RC5_Shift_Bit(1);                           // long low, long high, shift 1,0
               RC5_Shift_Bit(0);
               break;
      case 4:  RC5_Shift_Bit(1);                           // short low, short high, shift 1
               break;
      case 5:  frame = 0;                                  // short low, long high, ERROR
               break;
      case 6:  RC5_Shift_Bit(1);                           // long low, short high, shift 1,0
               RC5_Shift_Bit(0);
               prev = 0;                                   // new half bit is false
               break;
      case 7:  frame = 0;                                  // long low, long high, ERROR
      default: break;                                      // invalid
    }
}

void SCT_IRQHandler(void)
{
    if (LPC_SCT->EVFLAG & (1 << 0))                        // Event 0 ? (timeout)
    {                                                      // to guarantee a 12 msec idle time after last RC5 pulse
        if (frame & 0x2000)                                // frame (14 bits) full ?
        {
            RC5_Command = frame & 0x3F;                    // OK ! Save command byte
            RC5_System  = frame >> 6;                      // save system byte
            RC5_flag = 1;                                  // set event to trigger application
        }
        frame = 0;
        LPC_SCT->EVFLAG = (1 << 0);                        // clear event 0 flag
    }
    else                                                   // Event 2 (CTIN_0 rising edge)
    {
        if (frame)                                         // first pulse ?
        {
            high_time = LPC_SCT->CAP[1].L;                 // no, capture high time
            low_time  = LPC_SCT->CAP[2].L;                 // and capture low time
            RC5_Decode();                                  // and decode bit
        }
        else
        {
            prev  = 1;                                     // yes, assume half bit is true
            frame = 0x0001;                                // start bit received
        }
        LPC_SCT->EVFLAG = (1 << 2);                        // clear event 2 flag
    }
}

void RC5_Init(void)
{
    LPC_SCT->CTRL_L |= (SystemCoreClock/1000000-1) << 5;   // set prescaler, SCT clock = 1 MHz
                                              
    LPC_SCT->REGMODE_L      = (1 << 1) | (1 << 2);         // register pair 1 and 2 are capture
    LPC_SCT->MATCH[0].L     = 12000;                       // match 0 @ 12000/1MHz = 12 msec (timeout)
    LPC_SCT->MATCHREL[0].L  = 12000;

    LPC_SCT->EVENT[0].STATE = 0x00000001;                  // event 0 only happens in state 0
    LPC_SCT->EVENT[0].CTRL  = (0 << 0)  |                  // MATCHSEL[3:0]   = related to match 0
                              (1 << 12) |                  // COMBMODE[13:12] = uses match condition only
                              (1 << 14) |                  // STATELD [14]    = STATEV is loaded into state
                              (0 << 15);                   // STATEV  [15]    = new state is 0

    LPC_SCT->EVENT[1].STATE = 0x00000001;                  // event 1 only happens in state 0
    LPC_SCT->EVENT[1].CTRL  = (0 << 6)  |                  // IOSEL   [9:6]   = CTIN_0
                              (2 << 10) |                  // IOCOND  [11:10] = falling edge
                              (2 << 12) |                  // COMBMODE[13:12] = uses IO condition only
                              (1 << 14) |                  // STATELD [14]    = STATEV is loaded into state
                              (0 << 15);                   // STATEV[ 15]     = new state is 0

    LPC_SCT->EVENT[2].STATE = 0x00000001;                  // event 2 only happens in state 0
    LPC_SCT->EVENT[2].CTRL  = (0 << 6)  |                  // IOSEL   [9:6]   = CTIN_0
                              (1 << 10) |                  // IOCOND  [11:10] = rising edge
                              (2 << 12) |                  // COMBMODE[13:12] = uses IO condition only
                              (1 << 14) |                  // STATELD [14]    = STATEV is loaded into state
                              (0 << 15);                   // STATEV  [15]    = new state is 0

    LPC_SCT->CAPCTRL[1].L   = (1 << 1);                    // event 1 causes capture 1 to be loaded
    LPC_SCT->CAPCTRL[2].L   = (1 << 2);                    // event 2 causes capture 2 to be loaded
    LPC_SCT->LIMIT_L        = 0x0007;                      // events 0, 1 and 2 are used as counter limit
    LPC_SCT->EVEN           = 0x00000005;                  // events 0 and 2 generate interrupts

    NVIC_EnableIRQ(SCT_IRQn);                              // enable SCT interrupt

    LPC_SCT->CTRL_L &= ~(1 << 2);                          // unhalt it by clearing bit 2 of the CTRL register
}
