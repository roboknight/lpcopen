/*****************************************************************************
*  MCB4300 specific code
*
*****************************************************************************/
#include "chip.h"
#include "mcb4300.h"

static void scu_pinmux(unsigned port, unsigned pin, unsigned mode, unsigned func)
{
  volatile unsigned int * const scu_base = (unsigned int*)(LPC_SCU_BASE);

    scu_base[(PORT_OFFSET*port+PIN_OFFSET*pin)/4] = mode+func;
}

void MCB4300_Init(void) 
{
    LPC_CCU1->CLKCCU[CLK_MX_GPIO].CFG = (1 << 0);          // enable GPIO clock in CCU1

/**** LEDs ****/
    scu_pinmux(0xD, 10, MD_PUP, FUNC4);            // pin PD_10 = GPIO6[24]
    LPC_GPIO_PORT->DIR[6] |= (1 << 24);            // configure P6.24 (LED) as output

    scu_pinmux(0xD, 11, MD_PUP, FUNC4);            // pin PD_11 = GPIO6[25]
    LPC_GPIO_PORT->DIR[6] |= (1 << 25);            // configure P6.25 (LED) as output

    scu_pinmux(0xD, 12, MD_PUP, FUNC4);            // pin PD_12 = GPIO6[26]
    LPC_GPIO_PORT->DIR[6] |= (1 << 26);            // configure P6.26 (LED) as output

    scu_pinmux(0xD, 13, MD_PUP, FUNC4);            // pin PD_13 = GPIO6[27]
    LPC_GPIO_PORT->DIR[6] |= (1 << 27);            // configure P6.27 (LED) as output

    scu_pinmux(0xD, 14, MD_PUP, FUNC4);            // pin PD_14 = GPIO6[28]
    LPC_GPIO_PORT->DIR[6] |= (1 << 28);            // configure P6.28 (LED) as output

    scu_pinmux(  9,  0, MD_PUP, FUNC0);            // pin P9_0  = GPIO4[12]
    LPC_GPIO_PORT->DIR[4] |= (1 << 12);            // configure P4.12 (LED) as output

    scu_pinmux(  9,  1, MD_PUP, FUNC0);            // pin P9_1  = GPIO4[13]
    LPC_GPIO_PORT->DIR[4] |= (1 << 13);            // configure P4.13 (LED) as output

    scu_pinmux(  9,  2, MD_PUP, FUNC0);            // pin P9_2  = GPIO4[14]
    LPC_GPIO_PORT->DIR[4] |= (1 << 14);            // configure P4.14 (LED) as output

/**** push buttons ****/
    scu_pinmux(  4,  0, MD_PUP, (FUNC0 | EZI));    // pin P4_0  = GPIO2[0]
    LPC_GPIO_PORT->DIR[2] &= ~(1 << 0);            // configure P2.0 (SWP4_0) as input

    scu_pinmux(  4,  3, MD_PUP, (FUNC0 | EZI));    // pin P4_3  = GPIO2[3]
    LPC_GPIO_PORT->DIR[2] &= ~(1 << 3);            // configure P2.3 (SWP4_3) as input

    scu_pinmux(  4,  4, MD_PUP, (FUNC0 | EZI));    // pin P4_4  = GPIO2[4]
    LPC_GPIO_PORT->DIR[2] &= ~(1 << 2);            // configure P2.4 (SWP4_4) as input

/**** joystick ****/
    scu_pinmux(0xC,  9, MD_PUP, (FUNC4 | EZI));    // pin PC_9  = GPIO6[8]
    LPC_GPIO_PORT->DIR[6] &= ~(1 << 8);            // configure P6.8 (JOY_CENTER) as input

    scu_pinmux(0xC, 11, MD_PUP, (FUNC4 | EZI));    // pin PC_11 = GPIO6[10]
    LPC_GPIO_PORT->DIR[6] &= ~(1 << 10);           // configure P6.10 (JOY_UP) as input

    scu_pinmux(0xC, 12, MD_PUP, (FUNC4 | EZI));    // pin PC_12 = GPIO6[11]
    LPC_GPIO_PORT->DIR[6] &= ~(1 << 11);           // configure P6.11 (JOY_DOWN) as input

    scu_pinmux(0xC, 13, MD_PUP, (FUNC4 | EZI));    // pin PC_13 = GPIO6[12]
    LPC_GPIO_PORT->DIR[6] &= ~(1 << 12);           // configure P6.12 (JOY_LEFT) as input

    scu_pinmux(0xC, 14, MD_PUP, (FUNC4 | EZI));    // pin PC_14 = GPIO6[13]
    LPC_GPIO_PORT->DIR[6] &= ~(1 << 13);           // configure P6.13 (JOY_RIGHT) as input

/**** SCT ****/
    scu_pinmux(  2,  8, MD_PLN, FUNC1);            // pin P2_8  is SCT_OUT0
    scu_pinmux(  2,  7, MD_PLN, FUNC1);            // pin P2_7  is SCT_OUT1
    scu_pinmux(  2, 10, MD_PLN, FUNC1);            // pin P2_10 is SCT_OUT2
    scu_pinmux(  2,  9, MD_PLN, FUNC1);            // pin P2_9  is SCT_OUT3

    scu_pinmux(  2,  4, MD_PUP, FUNC3);            // pin P2_4  is SCT_IN0
    scu_pinmux(  2,  3, MD_PDN, FUNC3);            // pin P2_3  is SCT_IN1
    scu_pinmux(  2,  5, MD_PDN, FUNC1);            // pin P2_5  is SCT_IN2
}
