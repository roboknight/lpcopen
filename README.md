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

== Have fun ==
