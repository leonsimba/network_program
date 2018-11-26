/*
 * A simple realization of tee Program
 *
 * Description :
 *
 *		  splice
 *  stdin -------> pipefd_stdout[1]
 *  			   pipefd_stdout[0] ------> filefd
 *                       | tee      splice
 *   					 V
 *  			   pipefd_file[1]
 *  			   pipefd_file[0]   ------> stdout
 *  			   					splice
 *
 */
#define _GNU_SOURCE
#include <stdio.h>
#include <errno.h>
#include <fcntl.h>
#include <assert.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

int main( int argc, char* argv[] )
{
	int ret;
	int filefd;
	int pipefd_stdout[2], pipefd_file[2];

	if (argc != 2) {
		printf("Usage: %s <file>\n", argv[0]);
		return 1;
	}

	filefd = open(argv[1], O_CREAT | O_WRONLY | O_TRUNC, 0666);
	if (filefd < 0) {
		printf("open %s faile, err = %d\n", argv[1], errno);
		exit(-1);
	}

	ret = pipe(pipefd_stdout);
	assert(ret != -1);

    ret = pipe(pipefd_file);
	assert(ret != -1);

	// stdin --> pipefd_stdout[1]: read data frome stdin, and write to pipefd_stdout[1]
	ret = splice(STDIN_FILENO, NULL, pipefd_stdout[1], NULL, 32768, SPLICE_F_MORE | SPLICE_F_MOVE);
	assert(ret != -1);

	// pipefd_stdout[0] --> pipefd_file[1]: read data frome stdin, and write to pipefd_stdout[1]
	ret = tee(pipefd_stdout[0], pipefd_file[1], 32768, SPLICE_F_NONBLOCK);
	assert(ret != -1);

	// pipefd_stdout[0] --> filefd: write data to file
	ret = splice(pipefd_file[0], NULL, filefd, NULL, 32768, SPLICE_F_MORE | SPLICE_F_MOVE);
	assert(ret != -1);

	// pipefd_stdout[0] --> stdout: write data to stdout
	ret = splice(pipefd_stdout[0], NULL, STDOUT_FILENO, NULL, 32768, SPLICE_F_MORE | SPLICE_F_MOVE);
	assert(ret != -1);

	close(filefd);
	close(pipefd_stdout[0]);
	close(pipefd_stdout[1]);
	close(pipefd_file[0]);
	close(pipefd_file[1]);

	return 0;
}
