/*****************************************************************************
*  LPC4357 RC5 decoder program
*
*  1. Use SCT timer to capture and decode RC5 messages
*  2. Use UART to print received RC5 frames to PC terminal
*  3. SCT_IN0 (pin P2_4) is RC5 signal input
*****************************************************************************/
#include "chip.h"
//#include "mcb4300.h"
#include "global.h"

int main (void) 
{
    SystemCoreClockUpdate ();                      // Update system core clock
    Board_Init();

    LPC_CCU1->CLKCCU[CLK_MX_SCT].CFG = (1 << 0);   // enable SCT clock in CCU1

    //MCB4300_Init();                                // Initialize MCB4300 board stuff
    //USART3_Init(19200);
    Chip_UART_Init(LPC_USART0);
    Chip_UART_SetBaud(LPC_USART0,19200);
    RC5_Init();

    PrintString("\f\nLPC800 SCT-RC5 test Januari 2013\n\n");

    while (1)
    {
        if (RC5_flag)                                  // wait for RC5 code
        {
            RC5_flag = 0;                              // clear flag
            PrintString("RC5 = ");                     // and print it
            PrintByte(RC5_System);
            PrintString(" ");
            PrintByte(RC5_Command);
            PrintString("\n");
        }
    }
}
