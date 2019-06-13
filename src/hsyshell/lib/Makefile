#
# Makefile for hsylib library.
#
ROOT=..
include $(ROOT)/Make.defines.linux

LIBMISC	= libhsy.a
OBJS   = tools.o  error.o hsycmd.o
all:	$(LIBMISC)

$(LIBMISC):  $(OBJS)
	$(AR) rv $(LIBMISC) $?

clean:
	rm -f *.o a.out core temp.* $(LIBMISC)

