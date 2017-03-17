#Makefile to build BACnet Application for the Linux Port

# tools - only if you need them.
# Most platforms have this already defined
# CC = gcc

TARGET = bacwi

TARGET_BIN = ${TARGET}$(TARGET_EXT)

SRCS = main.c \
	../object/device-client.c

OBJS = ${SRCS:.c=.o}

all: ${BACNET_LIB_TARGET} Makefile ${TARGET_BIN}



# 2017-03-15 - Ted adding printf statements to following makefile recipe:

${TARGET_BIN}: ${OBJS} Makefile ${BACNET_LIB_TARGET}
	@printf "2017-03-15 - messages from bacwi makefile:\n"
	@printf "2017-03-15 - likely called by make invocation two parent directories above,\n"
	@printf "2017-03-15 - LFLAGS_ADDED_BY_TED holds '$(LFLAGS_ADDED_BY_TED)'.\n"
#	${CC} ${PFLAGS} ${OBJS} ${LFLAGS} -o $@
	${CC} ${PFLAGS} ${OBJS} ${LFLAGS} $(LFLAGS_ADDED_BY_TED) -o $@
	size $@
	cp $@ ../../bin



lib: ${BACNET_LIB_TARGET}

${BACNET_LIB_TARGET}:
	( cd ${BACNET_LIB_DIR} ; $(MAKE) clean ; $(MAKE) )

.c.o:
	${CC} -c ${CFLAGS} $*.c -o $@

depend:
	rm -f .depend
	${CC} -MM ${CFLAGS} *.c >> .depend

clean:
	rm -f core ${TARGET_BIN} ${OBJS} ${BACNET_LIB_TARGET} $(TARGET).map

include: .depend
