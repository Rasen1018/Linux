#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

int system(const char *cmd)
{
	pid_t pid;
	int status;

	if((pid = fork()) < 0) {
		exit(127);
	} else if (pid ==0) {
		return 127;
	} else {
		waitpid(pid, &status, 0);
		printf("Exit Value : %d\n", WEXITSTATUS(status));
	}
	return status;

}

int main(int argc, char **argv, char **envp)
{
	while(*envp)
		printf("%s\n", *envp++);

	system("who");
	system("nocommand");
	system("ls -2");

	return 0;

}
