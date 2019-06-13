ROOT = .
include $(ROOT)/Make.defines.linux
OBJS = hsyshell
all : $(OBJS)

% : %.c $(LIBHSY)
	$(CC) $(CFLAGS) $@.c -o $@ $(LDFLAGS) $(LDLIBS)

clean:  
	rm -f $(OBJS) $(TEMPFILES) *.o $(LDFLAGS) $(LDLIBS)

