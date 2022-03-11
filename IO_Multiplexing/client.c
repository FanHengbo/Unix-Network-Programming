#include "unp.h"
#include "util.h"
int
main(int argc, char **argv)
{
	int sockfd;
	struct sockaddr_in servaddr;

	if (argc != 2)
	{
		printf("Usage: client <IP Address>\n");
		exit(0);
	}
	
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(SERV_PORT);
	inet_pton(AF_INET, argv[1], &servaddr.sin_addr);

	connect(sockfd, (SA *) &servaddr, sizeof(servaddr));
	str_cli1(stdin, sockfd);

	exit(0);
}


void
str_cli1(FILE* fp, int sockfd)
{
	char sendline[MAXLINE], receiveline[MAXLINE];
	int maxfd1;
	fd_set rset;

	FD_ZERO(&rset);

	for (; ;)
	{
		FD_SET(sockfd, &rset);
		FD_SET(fileno(fp), &rset);
		maxfd1 = max(sockfd, (fileno(fp)));
		select(maxfd1+1, &rset, NULL, NULL, NULL);

		if (FD_ISSET(sockfd, &rset))
		{
			if (readline(sockfd, receiveline, MAXLINE) == 0)
			{
				printf("server terminated\n");
				exit(0);
			}
			fputs(receiveline, stdout);

		}
		if (FD_ISSET(fileno(fp), &rset))
		{
			if (fgets(sendline, MAXLINE, fp) == 0)
			{
				return;
			}
			writen(sockfd, sendline, strlen(sendline));
		}
	}
}