#include	"netp.h"

void sigchld_handler_v1(int signo)
{
	pid_t	pid;
	int		stat;

	// handler zombie process
	pid = wait(&stat);

	// we should not call printf here, it's a nonreentrant function
	printf("child %d terminated\n", pid);
	return;
}

void sigchld_handler_v2(int signo)
{
	// handler zombie process
	while (waitpid(-1, NULL, WNOHANG) > 0)
		continue;
	return;
}
