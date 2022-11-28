#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#define TCP_PORT 5100

int main(int argc, char **argv)
{
	int ssock;
	struct sockaddr_in servaddr;
	char mesg[BUFSIZ];
	int maxfd;

	if(argc <2) {
		printf("usage: %s <IP address>\n", argv[0]);
		return -1;
	}

	if((ssock = socket(AF_INET, SOCK_STREAM, 0)) <0) {
		perror("socket()");
		return -1;
	}

	memset(&servaddr, 0, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	inet_pton(AF_INET, argv[1], &(servaddr.sin_addr.s_addr));
	servaddr.sin_port = htons(TCP_PORT);

	if(connect(ssock, (struct sockaddr*)&servaddr,
	   sizeof(servaddr)) < 0)
	{	perror("connect()");
		return -1;
	}
	do {
		FD_ZERO(&read_fd);
		FD_SET(0, &read_fd);
		FD_SET(ssock, &read_fd);

		select(ssock+1, &readfd, NULL, NULL, NULL);
		memset(mesg, 0, BUFSIZ);

		if(FD_ISSET(0, &readfd)!=0) {

			fgets(mesg, BUFSIZ, stdin);
			write(ssock, mesg, strlen(mesg));
		}

		if(FD_ISSET(ssock, &readfd)!=0) {
			int n = read(ssock, mesg, BUFSIZ);
			if (n <=0) break;
			printf("Received data : %s ", mesg);
			FD_CLR(ssock, &read_fd);
		}

	} while(strncmp(mesg, "q", 1));

	close(ssock);

	return 0;
}

