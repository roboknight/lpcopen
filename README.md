lpcopen
=======

This is a fork of the LPCOpen provided by NXP.  This one has some linker scripts and Makefiles that allow you to build this with a toolchain like Yagarto.

To build an example:

1) pushd syscalls; make; popd
2) pushd lpc_chip_43xx; make; popd
3) pushd lpc_board_ngx_xplorer_4330; make; popd
4) change to the blinky example directory (periph_blinky/example/src)
5) make

To build an 'unprepped' example:

1) copy Makefile from periph_blinky/example/src into <target_example>/example/src
2) copy the linker file from periph_blinky/example/src to <target_example>/example/src/target_example.ld
3) make

A few useful environment variables:

TOOLCHAIN -- Set this to the prefix of your ARM toolchain (i.e. arm-none-eabi- )
PATH -- Make sure to include your TOOLCHAIN location in your path.

Currently the linker scripts are set up for the lpc4330-xplorer demo board.

To "blow" the binary into any of the boards, the simplest way is to use dfu-util.  If you get the latest
copy of dfu-util, it will come with 2 other tools: dfu-suffix and dfu-prefix.  If you use these, you
will be able to create RAMable binaries simply on Linux, OSX, or even Windows (If you are using Windows,
you can also use NXP's toolset.  Don't ask me what it was.  I've forgotten.  But Google hasn't.)

1) dfu-suffix -a <binary> -v 0x1fc9 -p 0x000c
   0x1fc9 = VID of lpc4330-xplorer (and probably NXP)
   0x000c = PID of lpc4330-xplorer

   replace the VID and PID if you have different ones.

2) dfu-prefix -a <binary> -L
   This will add a special LPC header onto the binary.
   This will allow the board to load the file.

NOTE:  These step should ONLY be performed *IF* you are using DFU mode directly.
       This means you've configured the dip switches to [OFF ON OFF ON] (should be DFU mode)
       and have just reset the board.

==================== FLASHING FIRMWARE =====================

In order to do this, you will likely need another tool, lpcdfu.
The reason is that the dfu protocol NXP uses AFTER its normal dfu
(i.e. to load the flasher code into RAM), isn't quite dfu-util
compatible.  Therefore, you'll likely need lpcdfu.  However,
nothing need be done to binaries that you wish to flash.

Don't flash: .elf files (these aren't binary files)
             The only time these will be truly useful is if you
             are connecting to JTAG, which you can now do
             with SWD and OpenOCD!  Olimex sells a nice dongle
             that has two adapters you can buy which, when
             combined, give you an OK little SWD dongle.  You
             will need to be familiar with compiling OpenOCD
             from the repository.  If you are not, older
             versions will likely only support a JTag configuration.
             As of this writing, OpenOCD is around version 0.9.0
             in the repository.  Hopefully this will be released
             in the near future, but until then, get it from the repo.

NOTE: currently, only the linker script for "flashable" binaries is
      in here.  It is trivial to change the script and is a TODO,
      not an exercise for the reader, unless you like linker scripts.

== Have fun ==

