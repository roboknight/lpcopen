/*
 * (C) Code Red Technologies Ltd, 2008-2012
 * Created from generic_c.ld (vLPCXpresso v4.2 (2 [Build 275] [03/04/2012] ))
 * By LPCXpresso v4.2.2 [Build 275] [03/04/2012]  on Wed Apr 11 11:12:41 IST 2012
 */


MEMORY
{
  /* Define each memory region */
  SPIFIFlash (rx) : ORIGIN = 0x14000000, LENGTH = 0x400000 /* 4096k */
  flash (rx) : ORIGIN = 0x14000000, LENGTH = 0x400000 /* 4096k */
  RamLoc128 (rwx) : ORIGIN = 0x10000000, LENGTH = 0x20000 /* 128k */
  RamLoc72 (rwx) : ORIGIN = 0x10080000, LENGTH = 0x12000 /* 72k */
  RamAHB32 (rwx) : ORIGIN = 0x20000000, LENGTH = 0x8000 /* 32k */
  RamAHB16 (rwx) : ORIGIN = 0x20008000, LENGTH = 0x4000 /* 16k */
  RamAHB_ETB16 (rwx) : ORIGIN = 0x2000c000, LENGTH = 0x4000 /* 16k */

}
  /* Define a symbol for the top of each memory region */
  __top_SPIFIFlash = 0x14000000 + 0x400000;
  __top_flash	 = 0x14000000 + 0x400000;
  __top_RamLoc128 = 0x10000000 + 0x20000;
  __top_RamLoc72 = 0x10080000 + 0x12000;
  __top_RamAHB32 = 0x20000000 + 0x8000;
  __top_RamAHB16 = 0x20008000 + 0x4000;
  __top_RamAHB_ETB16 = 0x2000c000 + 0x4000;

SECTIONS
{

	/* MAIN TEXT SECTION */	
	.text : ALIGN(4)
	{
		FILL(0xff)
		KEEP(*(.isr_vector))
		
		/* Global Section Table */
		. = ALIGN(4) ;
		__section_table_start = .;
		__data_section_table = .;
		LONG(LOADADDR(.data));
		LONG(    ADDR(.data)) ;
		LONG(  SIZEOF(.data));
		LONG(LOADADDR(.data_RAM2));
		LONG(    ADDR(.data_RAM2)) ;
		LONG(  SIZEOF(.data_RAM2));
		LONG(LOADADDR(.data_RAM3));
		LONG(    ADDR(.data_RAM3)) ;
		LONG(  SIZEOF(.data_RAM3));
		LONG(LOADADDR(.data_RAM4));
		LONG(    ADDR(.data_RAM4)) ;
		LONG(  SIZEOF(.data_RAM4));
		LONG(LOADADDR(.data_RAM5));
		LONG(    ADDR(.data_RAM5)) ;
		LONG(  SIZEOF(.data_RAM5));
		__data_section_table_end = .;
		__bss_section_table = .;
		LONG(    ADDR(.bss));
		LONG(  SIZEOF(.bss));
		LONG(    ADDR(.bss_RAM2));
		LONG(  SIZEOF(.bss_RAM2));
		LONG(    ADDR(.bss_RAM3));
		LONG(  SIZEOF(.bss_RAM3));
		LONG(    ADDR(.bss_RAM4));
		LONG(  SIZEOF(.bss_RAM4));
		LONG(    ADDR(.bss_RAM5));
		LONG(  SIZEOF(.bss_RAM5));
		__bss_section_table_end = .;
		__section_table_end = . ;
		/* End of Global Section Table */
		

		*(.after_vectors*)
		
		*(.text*)
		*(.rodata .rodata.*)
		. = ALIGN(4);
		
	} > flash

	/*
	 * for exception handling/unwind - some Newlib functions (in common
	 * with C++ and STDC++) use this.
	 */
	.ARM.extab : ALIGN(4)
	{
		*(.ARM.extab* .gnu.linkonce.armextab.*)
	} > flash
	__exidx_start = .;
	
	.ARM.exidx : ALIGN(4)
	{
		*(.ARM.exidx* .gnu.linkonce.armexidx.*)
	} > flash
	__exidx_end = .;
	
	_etext = .;
		
	
	.data_RAM2 : ALIGN(4)
	{
	   FILL(0xff)
		*(.data.$RAM2*)
		*(.data.$RamLoc72*)
	   . = ALIGN(4) ;
	} > RamLoc72 AT>flash
	
	.data_RAM3 : ALIGN(4)
	{
	   FILL(0xff)
		*(.data.$RAM3*)
		*(.data.$RamAHB32*)
	   . = ALIGN(4) ;
	} > RamAHB32 AT>flash
	
	.data_RAM4 : ALIGN(4)
	{
	   FILL(0xff)
		*(.data.$RAM4*)
		*(.data.$RamAHB16*)
	   . = ALIGN(4) ;
	} > RamAHB16 AT>flash
	
	.data_RAM5 : ALIGN(4)
	{
	   FILL(0xff)
		*(.data.$RAM5*)
		*(.data.$RamAHB_ETB16*)
	   . = ALIGN(4) ;
	} > RamAHB_ETB16 AT>flash
	
	/* MAIN DATA SECTION */

	.uninit_RESERVED : ALIGN(4)
	{
		KEEP(*(.bss.$RESERVED*))
	} > RamLoc128

	.data : ALIGN(4)
	{
		FILL(0xff)
		_data = .;
		*(vtable)
		*(.data*)
		. = ALIGN(4) ;
		_edata = .;
	} > RamLoc128 AT>flash

	
	.bss_RAM2 : ALIGN(4)
	{
		*(.bss.$RAM2*)
		*(.bss.$RamLoc72*)
	   . = ALIGN(4) ;
	} > RamLoc72
	
	.bss_RAM3 : ALIGN(4)
	{
		*(.bss.$RAM3*)
		*(.bss.$RamAHB32*)
	   . = ALIGN(4) ;
	} > RamAHB32
	
	.bss_RAM4 : ALIGN(4)
	{
		*(.bss.$RAM4*)
		*(.bss.$RamAHB16*)
	   . = ALIGN(4) ;
	} > RamAHB16
	
	.bss_RAM5 : ALIGN(4)
	{
		*(.bss.$RAM5*)
		*(.bss.$RamAHB_ETB16*)
	   . = ALIGN(4) ;
	} > RamAHB_ETB16

	/* MAIN BSS SECTION */
	.bss : ALIGN(4)
	{
		_bss = .;
		*(.bss*)
		*(COMMON)
		. = ALIGN(4) ;
		_ebss = .;
		PROVIDE(end = .);
	} > RamLoc128
	
	PROVIDE(_pvHeapStart = .);
	PROVIDE(_pvHeapEnd = __top_RamLoc128 - 0x10000);
	PROVIDE(_vStackTop = __top_RamLoc128 - 0);
}
