/*****************************************************************************
*  MCB4300 specific code
*
*****************************************************************************/

#define SWP4_0             LPC_GPIO_PORT->PIN[2] & (1 << 0)
#define SWP4_3             LPC_GPIO_PORT->PIN[2] & (1 << 3)
#define SWP4_4             LPC_GPIO_PORT->PIN[2] & (1 << 4)

#define JOY_CENTER         LPC_GPIO_PORT->PIN[6] & (1 << 8)
#define JOY_UP             LPC_GPIO_PORT->PIN[6] & (1 << 10)
#define JOY_DOWN           LPC_GPIO_PORT->PIN[6] & (1 << 11)
#define JOY_LEFT           LPC_GPIO_PORT->PIN[6] & (1 << 12)
#define JOY_RIGHT          LPC_GPIO_PORT->PIN[6] & (1 << 13)

#define PORT_OFFSET        0x80
#define PIN_OFFSET         0x04
#define MD_PUP             (0 << 3)            // enable pull-up  and disable pull-down
#define MD_BUK             (1 << 3)            // enable pull-up  and enable pull-down
#define MD_PLN             (2 << 3)            // disable pull-up and disable pull-down
#define MD_PDN             (3 << 3)            // disable pull-up and enable pull-down

#define EZI                (1 << 6)            // enable input buffer

#define FUNC0              0x0
#define FUNC1              0x1
#define FUNC2              0x2
#define FUNC3              0x3
#define FUNC4              0x4
#define FUNC5              0x5
#define FUNC6              0x6
#define FUNC7              0x7

extern void MCB4300_Init(void); 
