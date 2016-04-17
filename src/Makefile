DEFINES += -D_GNU_SOURCE
CFLAGS = -g -fPIC

OBJS := byte_order


all: $(OBJS)

byte_order: byte_order.o
	$(CC) $(CFLAGS) byte_order.o -o byte_order

clean:
	rm -f *.o $(OBJS)
