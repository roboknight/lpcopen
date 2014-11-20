/*
 * (C) Code Red Technologies Ltd, 2008-2012
 * Linker script memory definitions
 * Created from LinkMemoryTemplate
 * By LPCXpresso v4.2.2 [Build 275] [03/04/2012]  on Wed Apr 11 11:12:41 IST 2012)
*/

MEMORY
{
  /* Define each memory region */
  SPIFIFlash (rx) : ORIGIN = 0x14000000, LENGTH = 0x400000 /* 4096k */
  RamLoc128 (rwx) : ORIGIN = 0x10000000, LENGTH = 0x20000 /* 128k */
  RamLoc72 (rwx) : ORIGIN = 0x10080000, LENGTH = 0x12000 /* 72k */
  RamAHB32 (rwx) : ORIGIN = 0x20000000, LENGTH = 0x8000 /* 32k */
  RamAHB16 (rwx) : ORIGIN = 0x20008000, LENGTH = 0x4000 /* 16k */
  RamAHB_ETB16 (rwx) : ORIGIN = 0x2000c000, LENGTH = 0x4000 /* 16k */

}
  /* Define a symbol for the top of each memory region */
  __top_SPIFIFlash = 0x14000000 + 0x400000;
  __top_RamLoc128 = 0x10000000 + 0x20000;
  __top_RamLoc72 = 0x10080000 + 0x12000;
  __top_RamAHB32 = 0x20000000 + 0x8000;
  __top_RamAHB16 = 0x20008000 + 0x4000;
  __top_RamAHB_ETB16 = 0x2000c000 + 0x4000;

