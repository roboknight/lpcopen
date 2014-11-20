/***************************************************************************/
/*  USART3 driver for the NXP LPC43xx microcontrollers.                   */
/***************************************************************************/
#include "chip.h"
#include "uart.h"

const char ascii[] = "0123456789ABCDEF";

char ua_getchar(void)                                  // this function waits for a character
{
    return (char)Chip_UART_ReadByte(LPC_USART0);       // Read character from input buffer
}

void ua_outchar(char c)		
{
    Chip_UART_SendByte(LPC_USART0, (uint8_t)c);
}

void PrintByte(unsigned char b)
{
    ua_outchar(ascii[b >> 4]);
    ua_outchar(ascii[b & 0x0f]);
}

void PrintWord(unsigned short w)
{
    PrintByte((unsigned char)(w>>8));
    PrintByte((unsigned char)w);
}

void PrintBDecimal(unsigned char b)
{
  unsigned char p;

    p = ascii[b / 10];
    if (p != '0')
        ua_outchar(p);
    ua_outchar(ascii[b % 10]);
}

void PrintWDecimal(unsigned short w)
{
    ua_outchar(ascii[w / 10000]);
    w %= 10000;
    ua_outchar(ascii[w / 1000]);
    w %= 1000;
    ua_outchar(ascii[w / 100]);
    w %= 100;
    ua_outchar(ascii[w / 10]);
    ua_outchar(ascii[w % 10]);
}

void PrintString(const char *s)
{
    while (*s)
    {
        if (*s == '\n')
            ua_outchar('\r');                          // output a '\r' first
        ua_outchar(*s);
        s++;
    }
}
