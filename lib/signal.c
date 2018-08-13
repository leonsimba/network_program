/* include signal */
#include "netp.h"

int register_signal_handler(int signo, sa_handler_t func)
{
	int ret;
	struct sigaction act, oact;

	act.sa_handler = func;
	sigemptyset(&act.sa_mask);
	act.sa_flags = 0;

	if (signo == SIGALRM) {
#ifdef	SA_INTERRUPT
		act.sa_flags |= SA_INTERRUPT;	/* SunOS 4.x */
#endif
	} else {
#ifdef	SA_RESTART
		act.sa_flags |= SA_RESTART;		/* SVR4, 44BSD */
#endif
	}

	ret = sigaction(signo, &act, &oact);
	if (ret < 0) {
		err_sys("sigaction error :%s", strerror(errno));
	}

	return ret;
}
