#
# Library: libcrc
# Version: 2.00
#
# This file is licensed under the MIT License as stated below
#
#
#
# License
# -------
#
# Copyright (c) 1999-2016 Lammert Bies
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.
#
#
#
# Description
# -----------
#
# This Makefile is used to build the libcrc library. The only action you should
# normally have to do is to run the make program from the command line,
# independent on the Operating System you are working on.
#
# This Makefile is manually maintained. No autoconf or automake involved. This
# was a deliberate decision due to the absense of these tools on some systems,
# in particular in Windows environments.
#
#
#
# Dependencies
# ------------
#
# This Makefile is known to be functional with GNU Make. Other make utilities
# may work or may have problems. GNU Make is available both in source code
# and precompiled binaries for a large number of environments and therefore
# you are recommended to install a version of GNU Make if you encounter
# problems with the default make utility in your development chain.
#
# Aside from GNU Make and the standard C compiler headers and libraries which
# should have been installed already together with your compiler there are no
# other known dependencies.
#
#
#
# Library Type
# ------------
#
# The generated library is a library useable for static linking in this
# source directory structure. The decision for a static linkable library
# was deliberate because of the relatively small size of the library and the
# routines and to avoid version and dependency issues when distributing the
# end application to different environments.
#



INCDIR = include/
LIBDIR = lib/
OBJDIR = obj/
SRCDIR = src/
TSTDIR = test/

ifeq ($(OS),Windows_NT)

CC     = cl
LINK   = link
RM     = del /q
STRIP  = dir
OBJEXT = .obj
LIBEXT = .lib
EXEEXT = .exe
OFLAG  = -Fo
XFLAG  = /NOLOGO /OUT:
AR     = lib
ARQC   = /NOLOGO /OUT:
ARQ    = /NOLOGO
RANLIB = dir
CFLAGS = -Ox -Ot -MT -GT -volatile:iso -I${INCDIR} -nologo -J -sdl -Wall -WX \
	-wd4464 -wd4710 -wd4711 -wd4201 -wd4820

else

CC     = cc
LINK   = cc
RM     = /bin/rm -f
STRIP  = strip
OBJEXT = .o
LIBEXT = .a
EXEEXT =
OFLAG  = -o
XFLAG  = -o
AR     = ar
ARQC   = qc
ARQ    = q
RANLIB = ranlib
CFLAGS = -Wall -Wextra -Wstrict-prototypes -Wshadow -Wpointer-arith \
	-Wcast-qual -Wcast-align -Wwrite-strings -Wredundant-decls \
	-Wnested-externs -Werror -O3 -funsigned char -I${INCDIR}

endif



#
# Default compile commands for the source files
#

${OBJDIR}%${OBJEXT} : ${SRCDIR}%.c
	${CC} -c ${CFLAGS} ${OFLAG}$@ $<

${TSTDIR}${OBJDIR}%${OBJEXT} : ${TSTDIR}%.c
	${CC} -c ${CFLAGS} ${OFLAG}$@ $<




# The make file is used to compile both the library, and a test program to
# verify the functionality of the checksum algorithms after compilation.
#

all:							\
	${LIBDIR}libcrc${LIBEXT}			\
	testall${EXEEXT}



#
# This target cleans up all files created in the compilation phase.
#

clean:
	${RM} ${OBJDIR}*${OBJEXT}
	${RM} ${TSTDIR}${OBJDIR}*${OBJEXT}
	${RM} ${LIBDIR}libown${LIBEXT}
	${RM} testall${EXEEXT}



#
# The testall program can be run after compilation to verify the checksum
# routines. The extension of the program depends on the operating system used.
#

testall${EXEEXT} :					\
		${TSTDIR}${OBJDIR}testall${OBJEXT}	\
		${TSTDIR}${OBJDIR}testcrc${OBJEXT}	\
		${TSTDIR}${OBJDIR}testnmea${OBJEXT}	\
		${LIBDIR}libcrc${LIBEXT}		\
		Makefile
	${LINK} ${XFLAG}testall${EXEEXT}		\
		${TSTDIR}${OBJDIR}testall${OBJEXT}	\
		${TSTDIR}${OBJDIR}testcrc${OBJEXT}	\
		${TSTDIR}${OBJDIR}testnmea${OBJEXT}	\
		${LIBDIR}libown${LIBEXT}		\
		${LIBSYS}
	${STRIP} testcrc${EXEEXT}



#
# libcrc is the library which can be linked with other applications. The
# extension of the library depends on the operating system used.
#

${LIBDIR}libcrc${LIBEXT} :			\
	${OBJDIR}crc8${OBJEXT}			\
	${OBJDIR}crc16${OBJEXT}			\
	${OBJDIR}crc32${OBJEXT}			\
	${OBJDIR}crcccitt${OBJEXT}		\
	${OBJDIR}crcdnp${OBJEXT}		\
	${OBJDIR}crckrmit${OBJEXT}		\
	${OBJDIR}crcsick${OBJEXT}		\
	${OBJDIR}nmea-chk${OBJEXT}		\
	Makefile
		${RM}        ${LIBDIR}libcrc${LIBEXT}
		${AR} ${ARQC}${LIBDIR}libcrc${LIBEXT} ${OBJDIR}crc16${OBJEXT}
		${AR} ${ARQ} ${LIBDIR}libcrc${LIBEXT} ${OBJDIR}crc32${OBJEXT}
		${AR} ${ARQ} ${LIBDIR}libcrc${LIBEXT} ${OBJDIR}crc8${OBJEXT}
		${AR} ${ARQ} ${LIBDIR}libcrc${LIBEXT} ${OBJDIR}crcccitt${OBJEXT}
		${AR} ${ARQ} ${LIBDIR}libcrc${LIBEXT} ${OBJDIR}crcdnp${OBJEXT}
		${AR} ${ARQ} ${LIBDIR}libcrc${LIBEXT} ${OBJDIR}crckrmit${OBJEXT}
		${AR} ${ARQ} ${LIBDIR}libcrc${LIBEXT} ${OBJDIR}crcsick${OBJEXT}
		${AR} ${ARQ} ${LIBDIR}libcrc${LIBEXT} ${OBJDIR}nmea-chk${OBJEXT}
		${RANLIB}    ${LIBDIR}libcrc${LIBEXT}



#
# Individual source files with their header file dependencies
#

${OBJDIR}crc8${OBJEXT}			: ${SRCDIR}crc8.c ${INCDIR}checksum.h

${OBJDIR}crc16${OBJEXT}			: ${SRCDIR}crc16.c ${INCDIR}checksum.h

${OBJDIR}crc32${OBJEXT}			: ${SRCDIR}crc32.c ${INCDIR}checksum.h

${OBJDIR}crcccitt${OBJEXT}		: ${SRCDIR}crcccitt.c ${INCDIR}checksum.h

${OBJDIR}crcdnp${OBJEXT}		: ${SRCDIR}crcdnp.c ${INCDIR}checksum.h

${OBJDIR}crckrmit${OBJEXT}		: ${SRCDIR}crckrmit.c ${INCDIR}checksum.h

${OBJDIR}crcsick${OBJEXT}		: ${SRCDIR}crcsick.c ${INCDIR}checksum.h

${OBJDIR}nmea-chk${OBJEXT}		: ${SRCDIR}nmea-chk.c ${INCDIR}checksum.h

${TSTDIR}${OBJDIR}testall${OBJEXT}	: ${TSTDIR}testall.c ${TSTDIR}testall.h

${TSTDIR}${OBJDIR}testcrc${OBJEXT}	: ${TSTDIR}testcrc.c ${TSTDIR}testall.h ${INCDIR}checksum.h

${TSTDIR}${OBJDIR}testnmea${OBJEXT}	: ${TSTDIR}testnmea.c ${TSTDIR}testall.h ${INCDIR}checksum.h