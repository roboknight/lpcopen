1) Make sure you are pointing to you toolchain
	TOOLCHAIN=arm-none-eabi-
	for example.
2) make
3) This build is currently for FLASH ONLY.  The ram linker file
   doesn't yet work properly.  Therefore you need to find a way
   to flash this to your device.  lpcdfu works well here combined
   with dfu-utils.
4) Set your LPC4330-Xplorer as follows:
   1 2 3 4
   -------
   O O O O
   F N F N
   F   F

   At this point you'll be able to use lpcdfu to flash your board.

5) Set your LPC4330-Xplorer as follows:
   1 2 3 4
   -------
   O O O O
   F N N N
   F

6) Press RESET button.  You may need to press it twice as the board
   has an error that causes the reset to be required 2 times for
   most boots.
