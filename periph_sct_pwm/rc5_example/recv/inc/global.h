extern void RC5_Init(void);

extern unsigned char RC5_flag;
extern unsigned char RC5_System;
extern unsigned char RC5_Command;

extern const char ascii[];

extern void USART3_Init(uint32_t baudrate);
extern char ua_getchar(void);
extern void ua_outchar(char c);
extern char ua_inchar(void);
extern void PrintWord(unsigned short w);
extern void PrintByte(unsigned char b);
extern void PrintString(const char *s);
