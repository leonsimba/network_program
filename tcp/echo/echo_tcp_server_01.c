#include "netp.h"
#include "echo.h"

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

void str_echo(int sockfd)
{
	ssize_t		n;
	char		buf[MAXLINE];

again:
	while ( (n = read(sockfd, buf, MAXLINE)) > 0)
		Writen(sockfd, buf, n);

	if (n < 0 && errno == EINTR)
		goto again;
	else if (n < 0)
		err_sys("str_echo: read error");
}

int main(int argc, char **argv)
{
	pid_t childpid;
	int	listenfd, connfd;
	struct sockaddr_in cliaddr, servaddr;
	socklen_t clilen = sizeof(cliaddr);

	listenfd = socket(AF_INET, SOCK_STREAM, 0);
	if (listenfd < 0)
		err_sys("socket error");

	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family      = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port        = htons(SERV_PORT);

	/* bind server address */
	if (bind(listenfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0)
		err_sys("bind error");

	/* listening on listening socket */
	Listen(listenfd, LISTENQ);

	for ( ; ; ) {
		// accept connect request
		connfd = Accept(listenfd, (struct sockaddr *)&cliaddr, &clilen);

		// create a child process
		if ((childpid = fork()) < 0)
			err_sys("fork error");
			
		// child process
		if (childpid == 0) {
			// child closes listening socket
			if (close(listenfd) == -1)
				err_sys("child close listening socket error");

			// process the request
			str_echo(connfd);	
			exit(0);
		}

		// parent closes connected socket
		if (close(connfd) == -1)
			err_sys("parent close connected socket error");
	}
}
