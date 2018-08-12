CFLAGS = -I../../lib/
LDFLAGS = -L../../lib/ -lnetp
PROGS =	echo_tcp_client_01 echo_tcp_server_01

all: ${PROGS}

echo_tcp_client_01: echo_tcp_client_01.o
		${CC} ${CFLAGS} ${LDFLAGS} -o $@ echo_tcp_client_01.o

echo_tcp_server_01: echo_tcp_server_01.o
		${CC} ${CFLAGS} ${LDFLAGS} -o $@ echo_tcp_server_01.o

clean:
		rm -f ${PROGS} *.o
