#Makefile to build BACnet Application using GCC compiler

# tools - only if you need them.
# Most platforms have this already defined
# CC = gcc
# AR = ar
# MAKE = make
# SIZE = size
#
# Assumes rm and cp are available

# Executable file name
TARGET = bacrp

TARGET_BIN = ${TARGET}$(TARGET_EXT)

SRCS = main.c \
	../object/device-client.c

OBJS = ${SRCS:.c=.o}


// 2017-03-09 THU - added by Ted to support some of his local diagnostics routines:
CFLAGS_ADDED_BY_TED=-I/usr/local/include/libtestlib-0p1

LFLAGS_ADDED_BY_TED=-L/usr/local/lib/libtestlib-0p1 -ltestlib-0p1



all: ${BACNET_LIB_TARGET} Makefile ${TARGET_BIN}

${TARGET_BIN}: ${OBJS} Makefile ${BACNET_LIB_TARGET}
#	${CC} ${PFLAGS} ${OBJS} ${LFLAGS} -o $@
	@printf "# Compiling and linking with variable $${LDFLAGS} set to '${LDFLAGS}',\n"
#	${CC} ${PFLAGS} ${OBJS} ${LFLAGS} -L/usr/local/lib/libtestlib-0p1 -ltestlib-0p1 -o $@
	${CC} ${PFLAGS} ${OBJS} ${LFLAGS} ${LFLAGS_ADDED_BY_TED} -o $@
	size $@
	cp $@ ../../bin

lib: ${BACNET_LIB_TARGET}

${BACNET_LIB_TARGET}:
	( cd ${BACNET_LIB_DIR} ; $(MAKE) clean ; $(MAKE) )

.c.o:
#	${CC} -c ${CFLAGS} $*.c -o $@
	${CC} -c ${CFLAGS} ${CFLAGS_ADDED_BY_TED} $*.c -o $@

depend:
	rm -f .depend
	${CC} -MM ${CFLAGS} *.c >> .depend

clean:
	rm -rf core ${TARGET_BIN} ${OBJS} ${BACNET_LIB_TARGET}

include: .depend
