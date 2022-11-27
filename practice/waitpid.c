#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(void)
{
	pid_t pid;
	int status, returned;

	if((pid=fork()) == 0)
		exit(7);
	if((pid=fork()) == 0)
		abort();
	if((pid=fork()) == 0)
		status /=0;

	if ((returned = waitpid(pid, &status, 0)) != pid) {
			perror("wait error");
			return -1;
	}
	else if (returned == 0) {
		perror("Child processes do not finished yet.");
		return -1;
	}
	pr_exit(status);

	return 0;
}
