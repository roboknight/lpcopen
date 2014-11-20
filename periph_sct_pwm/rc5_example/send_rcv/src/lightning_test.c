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
#define RC5_DATA0()		   Chip_GPIO_WritePortBit(LPC_GPIO_PORT,0,1,FALSE )
#define RC5_DATA1()		   Chip_GPIO_WritePortBit(LPC_GPIO_PORT,0,1,TRUE  )
#else
#define RC5_DATA0()		   LPC_SCU->SFSP[1][0] = 0x58     // input low (pull down)
#define RC5_DATA1()		   LPC_SCU->SFSP[1][0] = 0x40     // input high (pull up)
#endif

typedef enum {
	BIT_NON	  = 0,
	BIT_SOF,
	BIT_FRAME,
	BIT_EOF
} states_t;

volatile uint8_t  state,x;                                 // driver state
volatile uint8_t  count;                                 // bit counter
volatile uint8_t state_cnt;
volatile uint8_t  flag;                                  // frame send ready flag
volatile uint16_t frame;                                 // frame

volatile uint8_t  r_count;
volatile uint8_t  rcv_flag;
volatile uint8_t  r_byte;
volatile uint8_t  r_byte_cnt;
static   uint8_t  r_bytes[100];
static   uint16_t high_time, low_time;

void RC5_Send(uint16_t fr)
{
    frame = fr;                        
    state = 0;
    flag  = 0;                         // clear ready flag
	state_cnt = 0;
    count = 8;                         // set bit count to 14

    LPC_SCT->EVEN |= (1 << 0);          // event 0 generates interrupt

    while (!flag) __WFI();             // wait for ready flag

    LPC_SCT->EVEN &= ~(1 << 0);
}

void RC5_Recv(void)
{
	while(!rcv_flag) __WFI();
	rcv_flag = 0;
	Board_LED_Toggle(1);
}


void SCT_IRQHandler(void)
{

	if(count) {
		if(LPC_SCT->EVFLAG & (1 << 0)) {
			switch(state) {
			case 0:
				RC5_DATA0();
				state++;
				break;
			case 2:
				RC5_DATA1();
				state++;
				break;
			case 3:
				RC5_DATA0();
				if(frame & 0x01) state = 29;
				state++;
				break;
			case 9:
				frame = frame >> 1;
				RC5_DATA1();
				state++;
				break;
			case 15:
				state = 4;
				count--;
				if(!count) flag = 1;
				break;
			case 30:
				state = 8;
			default:
				state++;
			}
		} 
		#if 0
		if (LPC_SCT->EVFLAG & (1 << 1)) {
			// Falling edge happened
			high_time = LPC_SCT->CAP[1].L;
			low_time  = LPC_SCT->CAP[2].L;
		
			if(low_time > 10) {
				// Reset or MSG start or turn around
				if(high_time > 20) {
					// reset
					r_byte = 0;
					r_count = 0;
					r_byte_cnt = 0;
				} else if(high_time > 10) {
					// Turn-around done
					// MSG complete
					r_byte = 0;
					r_count = 0;
					rcv_flag = 1;
				} else if(high_time > 4) {
					// Byte done.
					r_byte = 0;
					r_count = 0;
				} else {
					// SOF
					r_byte_cnt = 0;
					r_byte = 0;
					r_count = 0;
				}
			} else if(low_time <= 2) {
				// One bit
				r_byte |= (1 << r_count++);
			} else {
				// Zero bit
				r_count++;
			}
		
			if(r_count > 7) {
				r_bytes[r_byte_cnt++] = r_byte;
			}
		}
		#endif
    }
	LPC_SCT->EVFLAG = (3 << 0);          // clear event flags
}

void RC5_Init(void)
{
	Chip_SCU_PinMuxSet(0, 1, SCU_MODE_FUNC0 | SCU_MODE_PULLUP);
    Chip_SCU_PinMuxSet(0, 4, SCU_MODE_FUNC1);
    RC5_DATA1();                                           // sent loopback high
    Chip_GPIO_SetPinDIRInput(LPC_GPIO_PORT, 0, 4);
    Chip_GPIO_SetPinDIROutput(LPC_GPIO_PORT, 0, 1);		   // Loopback set to output.

/********************************************************************
*  SCT_L: low part configuration:
********************************************************************/
                                              
/********************************************************************
*  SCT_H: high part configuration:
********************************************************************/
    //LPC_SCT->CTRL_L |= (SystemCoreClock/500000-1) << 5; (extreme jitter)

	// Xmit code
    LPC_SCT->MATCH[0].L     = (SystemCoreClock/2000000-1); // 1MHz (every 1uS)
    LPC_SCT->MATCHREL[0].L  = (SystemCoreClock/2000000-1);;

    LPC_SCT->EVENT[0].STATE = 0x00000001;           // event 0 happens in state 0
    LPC_SCT->EVENT[0].CTRL  = (0 << 0)  |           // MATCHSEL [3:0]   = related to match_L 0
                              (0 << 4)  |           // HEVENT   [4]     = use L state & match
                              (1 << 12) |           // COMBMODE [13:12] = match condition only
                              (0 << 14) |           // STATELD  [14]    = STATEV is added to state
                              (0 << 15);            // STATEV   [19:15] = no state change

    LPC_SCT->LIMIT_L        = (1 << 0);             // events 0 is used as L counter limit

	// Recv code
	#if 0
	LPC_SCT->REGMODE_L		= (1 << 1) | (1 << 2);	// USE register pair 1&2 for capture.
	
	LPC_SCT->EVENT[1].STATE	= 0x00000001;			// Event 1 happens in all states
	LPC_SCT->EVENT[1].STATE = (3 << 6)   |			// IOSEL   [9:6]   = CTIN_3
							  (2 << 10)  |			// IOCOND  [11:10] = falling edge
							  (2 << 12)  |			// COMBMODE[13:12] = uses IO condition only
							  (1 << 14)  |			// STATELD [14]    = STATEV is loaded into state
							  (0 << 15);
	
	LPC_SCT->EVENT[2].STATE	= 0x00000001;			// Event 1 happens in all states
	LPC_SCT->EVENT[2].STATE = (3 << 6)   |			// IOSEL   [9:6]   = CTIN_3
							  (1 << 10)  |			// IOCOND  [11:10] = rising edge
							  (2 << 12)  |			// COMBMODE[13:12] = uses IO condition only
							  (1 << 14)  |			// STATELD [14]    = STATEV is loaded into state
							  (0 << 15);

    LPC_SCT->CAPCTRL[1].L   = (1 << 1);             // event 1 causes capture 1 to be loaded
    LPC_SCT->CAPCTRL[2].L   = (1 << 2);             // event 2 causes capture 2 to be loaded
    LPC_SCT->LIMIT_L        = 0x0007;               // events 0, 1 and 2 are used as counter limit
	#endif

    NVIC_EnableIRQ(SCT_IRQn);                       // enable SCT interrupt

    LPC_SCT->EVEN = 0;				   			    // clear interrupt until we're ready

    LPC_SCT->CTRL_U &= ~(1 << 2);	                // unhalt low counter
}
