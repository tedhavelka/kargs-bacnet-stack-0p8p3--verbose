#Makefile to build test case
CC      = gcc
BASEDIR = .
# -g for debugging with gdb
DEFINES = -DBIG_ENDIAN=0 -DBACDL_MSTP=1 -DTEST_DLMSTP
INCLUDES = -I. -I../../ 
CFLAGS  = -Wall $(INCLUDES) $(DEFINES) -g

SRCS = rs485.c \
	dlmstp.c \
	../../mstp.c \
	../../crc.c

OBJS = ${SRCS:.c=.o}



## - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
##  2017-03-13 THU - added by Ted to support some of his local
##    diagnostics routines:
## - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

CFLAGS_ADDED_BY_TED=-I/usr/local/include/libtestlib-0p1

LFLAGS_ADDED_BY_TED=-L/usr/local/lib/libtestlib-0p1 -ltestlib-0p1





TARGET = dlmstp

all: ${TARGET}
	@printf "#\n# makefile for 'dlmstp' linux port program starting,\n#\n "
 

#
## 2017-03-13 - Ted modifying TARGET rule to include local C libraries:
## - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

${TARGET}: ${OBJS}
#	${CC} -pthread -o $@ ${OBJS} 
	${CC} -pthread ${LFLAGS_ADDED_BY_TED} -o $@ ${OBJS} 




#
## 2017-03-13 - Ted modifying "dot c dot o" rule to include local C libraries:
## - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

.c.o:
#	${CC} -c ${CFLAGS} $*.c -o $@
	${CC} -c ${CFLAGS} ${CFLAGS_ADDED_BY_TED} $*.c -o $@



	
depend:
	rm -f .depend
	${CC} -MM ${CFLAGS} *.c >> .depend
	
clean:
	rm -rf core ${TARGET} $(OBJS) *.bak *.1 *.ini

include: .depend
