#!/bin/bash -eux
# -e: Exit immediately if a command exits with a non-zero status.
# -u: Treat unset variables as an error when substituting.
# -x: Display expanded script commands

if test "$CROSS_TOOL" = m68k-atari-mintelf; then
	COMPILE_ELF=Y
else
	COMPILE_ELF=N
fi

make COMPILE_ELF=${COMPILE_ELF} DESTDIR=${INSTALL_DIR} install
