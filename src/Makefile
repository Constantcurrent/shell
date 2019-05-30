ROOT=.
PLATFORM=$(shell $(ROOT)/systype.sh)
include $(ROOT)/Make.defines.$(PLATFORM)

ifeq "$(PLATFORM)" "linux"
        ZAP = zap
else
        ZAP =
endif


PROGS = main

all:    $(PROGS) 

%: %.c $(LIBAPUE) 
	$(CC) $(CFLAGS) $@.c -o $@ $(LDFLAGS) $(LDLIBS)

clean:  
	rm -f $(PROGS) $(TEMPFILES) *.o $(ZAP)

include $(ROOT)/Make.libapue.inc

