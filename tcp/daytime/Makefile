CFLAGS = -I../../lib
LDFLAGS = -L../../lib/ -lnetp
PROGS =	daytime_tcp_client_01 daytime_tcp_server_01

all: ${PROGS}

daytime_tcp_client_01: daytime_tcp_client_01.o
		${CC} ${CFLAGS} ${LDFLAGS} -o $@ daytime_tcp_client_01.o

daytime_tcp_server_01: daytime_tcp_server_01.o
		${CC} ${CFLAGS} ${LDFLAGS} -o $@ daytime_tcp_server_01.o

clean:
		rm -f ${PROGS} *.o
