/*****************************************************************************
*  LPC4357 RC5 encoder program
*
*  Use SCT timer to send encoded RC5 frames
*  Send frames as long as switch SWP4_0 is pressed
*  SCT_IN0 (pin P2_4) used as dummy input
*  SCT_OUT0 (pin P2_8) used as RC5 output
*****************************************************************************/

#if defined(NO_BOARD_LIB)
#include "chip.h"
#else
#include "board.h"
#endif
#include "cmsis.h"
#include "mcb4300.h"
#include "global.h"

void SysTick_Handler(void)                         // just used to wake-up from sleep mode
{
	static int x=0;

	if(x++ > 500) {
		Board_LED_Toggle(0);
		x = 0;
	}
}

int main (void) 
{
  static uint8_t  trg = 1;
  static uint8_t  tgl = 1;
  static uint16_t frame;

    SystemCoreClockUpdate ();                      // Update system core clock
    Board_Init();


    LPC_CCU1->CLKCCU[CLK_MX_SCT].CFG = (1 << 0);          // enable SCT clock in CCU1 

    //MCB4300_Init();                                // Initialize MCB4300 board stuff
    RC5_Init();
    /* Enable and setup SysTick Timer at a periodic rate */
    SysTick_Config(SystemCoreClock / 1000);

    while (1)
    {
        frame = 0x41;
               
        RC5_Send(frame);                       // send RC5 frame
        __WFI();                                   // goto sleep
    }
}
