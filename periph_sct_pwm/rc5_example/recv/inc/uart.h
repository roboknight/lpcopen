typedef unsigned char          BYTE;
typedef unsigned short         WORD;
typedef unsigned long          LONG;

extern const char ascii[];

extern void USART0_Init(LONG baudrate);
extern char ua_getchar(void);
extern void ua_outchar(char c);
extern char ua_inchar(void);
extern void PrintWord(unsigned short w);
extern void PrintByte(unsigned char b);
extern void PrintString(const char *s);
