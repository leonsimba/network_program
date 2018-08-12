/* the header file of net programe */
#ifndef	__netp_h
#define	__netp_h

/* If anything changes in the following list of #includes, must change
   acsite.m4 also, for configure's tests. */

#include	<sys/types.h>	/* basic system data types */
#include	<sys/socket.h>	/* basic socket definitions */
#if TIME_WITH_SYS_TIME
#include	<sys/time.h>	/* timeval{} for select() */
#include	<time.h>		/* timespec{} for pselect() */
#else
#if HAVE_SYS_TIME_H
#include	<sys/time.h>	/* includes <time.h> unsafely */
#else
#include	<time.h>		/* old system? */
#endif
#endif
#include	<netinet/in.h>	/* sockaddr_in{} and other Internet defns */
#include	<arpa/inet.h>	/* inet(3) functions */
#include	<errno.h>
#include	<fcntl.h>		/* for nonblocking */
#include	<netdb.h>
#include	<signal.h>
#include	<stdio.h>
#include	<stdlib.h>
#include	<string.h>
#include	<sys/stat.h>	/* for S_xxx file mode constants */
#include	<sys/uio.h>		/* for iovec{} and readv/writev */
#include	<unistd.h>
#include	<sys/wait.h>
#include	<sys/un.h>		/* for Unix domain sockets */

#ifdef	HAVE_STRINGS_H
# include	<strings.h>		/* for convenience */
#endif

// wrap stdio handle
void Fclose(FILE *);
FILE * Fopen(const char *, const char *);
FILE * Fdopen(int , const char *);
char * Fgets(char *, int, FILE *);
void Fputs(const char *, FILE *);

ssize_t	writen(int , const void *, size_t);
void Writen(int, void *, size_t);

ssize_t readlinebuf(void **);
ssize_t readline(int , void *, size_t);
ssize_t	Readline(int, void *, size_t);

// error handle
void err_dump(const char *, ...);
void err_msg(const char *, ...);
void err_quit(const char *, ...);
void err_ret(const char *, ...);
void err_sys(const char *, ...);

#endif	/* __netp_h */
