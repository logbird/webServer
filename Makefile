PROFILES=server.c
PROOBJS=$(PROFILES:.c=.o)
PROFLAGS=
TARGET=running

all:$(TARGET)


$(TARGET) : $(PROOBJS)
	gcc -o $(TARGET) $(PROOBJS)

%.o:%.c
	gcc -c -o $@ $<

clean:
	rm -rf *.o
