#include	"netp.h"
#include	<time.h>

/* Following could be derived from SOMAXCONN in <sys/socket.h>, but many
 * kernels still #define it as 5, while actually supporting many more */
#define LISTENQ     1024    /* 2nd argument to listen() */
#define MAXLINE     4096    /* max text line length */

static void Listen(int fd, int backlog)
{
	char	*ptr;

	/* use environment variable to override backlog args */
	if ( (ptr = getenv("LISTENQ")) != NULL)
		backlog = atoi(ptr);

	if (listen(fd, backlog) < 0)
		err_sys("listen error");
}

static int Accept(int fd, struct sockaddr *sa, socklen_t *salenptr)
{
	int		n;

again:
	if ( (n = accept(fd, sa, salenptr)) < 0) {
#ifdef	EPROTO
		if (errno == EPROTO || errno == ECONNABORTED)
#else
		if (errno == ECONNABORTED)
#endif
			goto again;
		else
			err_sys("accept error");
	}
	return(n);
}

static void print_client_addr(int family, struct sockaddr_in * cliaddr)
{
	char buff[MAXLINE];

	memset(buff, 0, sizeof(buff));
	if (inet_ntop(family, &(cliaddr->sin_addr), buff, sizeof(buff)) == NULL)
        printf("inet_ntop error\n");     /* sets errno */

	printf("connection from %s, port %d\n", buff, ntohs(cliaddr->sin_port));
}

int main(int argc, char **argv)
{
	time_t ticks;
	char buff[MAXLINE];
	int	listenfd, connfd;
	struct sockaddr_in	servaddr, cliaddr;
	socklen_t len = sizeof(cliaddr);

	listenfd = socket(AF_INET, SOCK_STREAM, 0);
	if (listenfd < 0)
		err_sys("socket error");

	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family      = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port        = htons(13);	/* daytime server */

	/* bind server address */
	if (bind(listenfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0)
		err_sys("bind error");

	Listen(listenfd, LISTENQ);

	for ( ; ; ) {
		int nbytes;

		// accept connect request
		connfd = Accept(listenfd, (struct sockaddr *)&cliaddr, &len);
		print_client_addr(AF_INET, &cliaddr);

		// get the time information
        ticks = time(NULL);
        snprintf(buff, sizeof(buff), "%.24s\r\n", ctime(&ticks));

		// send data to client
		nbytes = strlen(buff);
		if (write(connfd, buff, nbytes) != nbytes)
			err_sys("write error");

		// close connect socket	
		if (close(connfd) == -1)
			err_sys("close error");
	}
}
