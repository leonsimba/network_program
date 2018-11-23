/*
 * An example to show how to use sendfile(), Server use sendfile
 * to send the whole file to client.
 *
 * Usage:
 *     Server: ./sendfile server_ip 12345 path_to_file
 *     Client: telnet server_ip 12345
 */
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <stdio.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/sendfile.h>

int main(int argc, char* argv[])
{
	int ret;
	struct stat stat_buf;
	int listen_fd, conn_fd, file;
	struct sockaddr_in srv_addr, cli_addr;
	socklen_t len = sizeof(cli_addr);

	int port;
	char *ip, *file_name;

	if( argc <= 3 ) {
	    printf("usage: %s ip port filename\n", argv[0]);
	    return 1;
	}
	ip = argv[1];
	port = atoi(argv[2]);
	file_name = argv[3];

	// open the file we want to send
	file = open(file_name, O_RDONLY);
	assert(file > 0);

	fstat(file, &stat_buf);

	listen_fd = socket(PF_INET, SOCK_STREAM, 0);
	assert(listen_fd >= 0);

	bzero(&srv_addr, sizeof(srv_addr));
	srv_addr.sin_family = AF_INET;
	inet_pton(AF_INET, ip, &srv_addr.sin_addr);
	srv_addr.sin_port = htons(port);

	ret = bind(listen_fd, (struct sockaddr *)&srv_addr, sizeof(srv_addr));
	assert(ret != -1);

	ret = listen(listen_fd, 5);
	assert(ret != -1);

	// accept client connection request
	conn_fd = accept(listen_fd, (struct sockaddr *)&cli_addr, &len);
	if (conn_fd < 0) {
		printf("errno is: %d\n", errno);
	} else {
		// send file data to client
		sendfile(conn_fd, file, NULL, stat_buf.st_size);
		close(conn_fd );
    }

    close(listen_fd);
    return 0;
}
