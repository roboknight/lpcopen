/********************************************************************
*
*  LPC4357 - RC5 encoder (transmitter)
*
*  Use SCT_OUT0 to send modulated (36 KHz carrier) RC5 frames
*  Reference: AN10210 and AN10722
*  RC5 format:
*
*      | S | F | C |   5 sytem bits    |    6 command bits     |
*      | 1 | 1 | 0 | 0 | 0 | 0 | 0 | 1 | 1 | 0 | 1 | 1 | 1 | 1 |
*
*        +-+ +---+ +-+ +-+ +-+ +-+   +-+ +---+   +-+ +-+ +-+ +-+
*        | | |   | | | | | | | | |   | | |   |   | | | | | | | |
*   -----+ +-+   +-+ +-+ +-+ +-+ +---+ +-+   +---+ +-+ +-+ +-+ +----
*
*        | |
*        | +--------------------------------------------------+
*        |                                                    |
*        +-+     +-+     +-+                  +-+     +-+
*        | |     | |     | |      32 x        | |     | |
*   -----+ +-----+ +-----+ +- - - - - - - - - + +-----+ +-------
*
*
*
********************************************************************/
#include "chip.h"

#if 1
#define RC5_DATA0()		   Chip_GPIO_WritePortBit(LPC_GPIO_PORT,0,4,FALSE )
#define RC5_DATA1()		   Chip_GPIO_WritePortBit(LPC_GPIO_PORT,0,4,TRUE  )
#else
#define RC5_DATA0()		   LPC_SCU->SFSP[1][0] = 0x58     // input low (pull down)
#define RC5_DATA1()		   LPC_SCU->SFSP[1][0] = 0x40     // input high (pull up)
#endif

static   uint8_t  state;                                 // driver state
static   uint8_t  count;                                 // bit counter
volatile uint8_t  flag;                                  // frame send ready flag
static   uint16_t frame;                                 // frame

void RC5_Send(uint16_t fr)
{
    frame = fr;                                        // MSB first, 14 bits shift left 2
    state = 0;
    flag  = 0;                                         // clear ready flag
    count = 8;                                        // set bit count to 14

    LPC_SCT->EVEN = (1 << 0);                          // event 4 generates interrupt

    while (!flag) __WFI();                             // wait for ready flag

    Board_LED_Toggle(1);

    LPC_SCT->EVEN = (0 << 0);
}

void SCT_IRQHandler(void)
{

    if(count) {  // bits to send
	switch(state) {
	case  0:
		state++;
		RC5_DATA0();
		break;
	case  1:
	case  2:
	case  3:
	case  4:
	case  5:
	case  6:
	case  7:
	case  8:
	case  9:
	case 10:
	case 12:
	case 13:
	case 14:
	case 16:
	case 18:
	case 19:
	case 20:
	case 21:
	case 23:
		state++;
		break;
	case 11:
		RC5_DATA1();
		state++;
		break;
	case 15:
		// Start the bit
		RC5_DATA0();
		state++;
		break;
	case 17:
		if(frame & 0x01) 
			// Finish the bit
			RC5_DATA1();
		state++;
		break;
	case 22:
		RC5_DATA1();
		state++;
		break;
	case 24:
		frame = frame >> 1;
		--count;
		state = 15;
		break;
	default:
		;
	}
    } else {
	// Wait 10uS...
	switch(state) {
	case 0:
		flag = 1;
	case 1:
	case 2:
	case 3:
	case 4:
	case 5:
	case 6:
	case 7:
	case 8:
	case 9:
		state--;
		break;
	case 15:
		state = 9;
		break;
	default:
		;
	}
    }
    LPC_SCT->EVFLAG = (1 << 0);                            // clear event 4 flag
}

void RC5_Init(void)
{
    Chip_SCU_PinMuxSet(0, 4, SCU_MODE_FUNC0);
    RC5_DATA1();                                           // SCT_IN0 high
    Chip_GPIO_SetPinDIROutput(LPC_GPIO_PORT, 0, 4);

/********************************************************************
*  SCT_L: low part configuration:
********************************************************************/
                                              
/********************************************************************
*  SCT_H: high part configuration:
********************************************************************/
    //LPC_SCT->CTRL_L |= (SystemCoreClock/500000-1) << 5; (extreme jitter)

    LPC_SCT->MATCH[0].L     = (SystemCoreClock/1000000-1); // 1MHz (every 1uS)
    LPC_SCT->MATCHREL[0].L  = (SystemCoreClock/1000000-1);;

    LPC_SCT->EVENT[0].STATE = 0x00000001;                  // event 0 happens in all states
    LPC_SCT->EVENT[0].CTRL  = (0 << 0)  |                  // MATCHSEL [3:0]   = related to match_L 0
                              (0 << 4)  |                  // HEVENT   [4]     = use L state & match
                              (1 << 12) |                  // COMBMODE [13:12] = match condition only
                              (0 << 14) |                  // STATELD  [14]    = STATEV is added to state
                              (0 << 15);                   // STATEV   [19:15] = no state change

    LPC_SCT->LIMIT_L        = (1 << 0);                    // events 0 is used as L counter limit

    NVIC_EnableIRQ(SCT_IRQn);                              // enable SCT interrupt

    //LPC_SCT->EVEN = 0;				   // clear interrupt until we're ready

    LPC_SCT->CTRL_U &= ~(1 << 2);	                   // unhalt low counter
}
