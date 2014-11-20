extern void RC5_Init(void);
extern void RC5_Send(uint16_t frame);

extern const char ascii[];

extern void USART0_Init(uint32_t baudrate);
extern char ua_getchar(void);
extern void ua_outchar(char c);
extern char ua_inchar(void);
extern void PrintWord(unsigned short w);
extern void PrintByte(unsigned char b);
extern void PrintString(const char *s);
