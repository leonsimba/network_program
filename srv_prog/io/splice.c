/*
 * An example to show how to use splice(), Server use splice()
 * to send the data which receivced from client before to client.
 *
 * Usage:
 *     Server: ./splice server_ip 12345
 *     Client: telnet server_ip 12345
 */
#define _GNU_SOURCE
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#define DATA_LEN		32768

int main( int argc, char* argv[] )
{
	int ret;
	int on = 1;
	int listen_fd, conn_fd;
    struct sockaddr_in srv_addr, cli_addr;
	socklen_t len = sizeof(cli_addr);

	char * ip;
	int port;

    if (argc <= 2) {
        printf( "usage: %s server_ip port\n", argv[0]);
        return 1;
    }
    ip = argv[1];
    port = atoi(argv[2]);

    listen_fd = socket(PF_INET, SOCK_STREAM, 0);
	if (listen_fd < 0) {
		perror("socket failed");
		exit(-1);
	}

	ret = setsockopt(listen_fd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on) );
	if (ret < 0) {
		perror("setsocketopt failed");
		exit(-1);
	}

    bzero( &srv_addr, sizeof( srv_addr ) );
    srv_addr.sin_family = AF_INET;
    inet_pton( AF_INET, ip, &srv_addr.sin_addr);
    srv_addr.sin_port = htons( port );

    ret = bind(listen_fd, (struct sockaddr *)&srv_addr, sizeof(srv_addr));
	if (ret < 0) {
		perror("bind failed");
		exit(-1);
	}

    ret = listen(listen_fd, 5);
	if (ret < 0) {
		perror("listen failed");
		exit(-1);
	}

	// accept client connection request
    conn_fd = accept(listen_fd, (struct sockaddr *)&cli_addr, &len);
    if (conn_fd < 0) {
        printf("accept failed, err = %d\n", errno);
    } else {
        int pipefd[2];

        ret = pipe( pipefd );
        assert( ret != -1 );

		// read data from client, and write to pipefd[1]
        ret = splice(conn_fd, NULL, pipefd[1], NULL, 32768, SPLICE_F_MORE | SPLICE_F_MOVE);
        assert(ret != -1);

		// read data from pipe, and send to client through socket
        ret = splice(pipefd[0], NULL, conn_fd, NULL, 32768, SPLICE_F_MORE | SPLICE_F_MOVE);
        assert(ret != -1);

        close(conn_fd);
    }

    close(listen_fd);
    return 0;
}
