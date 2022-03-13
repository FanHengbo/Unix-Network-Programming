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
	int stdeof = 0;
	int n;

	FD_ZERO(&rset);

	for (; ;)
	{
		FD_SET(sockfd, &rset);
		if (stdeof == 0)
			FD_SET(fileno(fp), &rset);
		maxfd1 = max(sockfd, (fileno(fp)));
		select(maxfd1+1, &rset, NULL, NULL, NULL);

		if (FD_ISSET(sockfd, &rset))
		{
			if ((n = read(sockfd, receiveline, MAXLINE)) == 0)
			{
				if (stdeof == 1)
					return;
				else
				{
					printf("server terminated\n");
					exit(0);
				}
			}
			else
				writen(fileno(stdout), receiveline, strlen(receiveline));
		}
		if (FD_ISSET(fileno(fp), &rset))
		{
			if (read(fileno(fp), sendline, MAXLINE) == 0)
			{
				shutdown(sockfd, SHUT_WR);
				stdeof = 1;
				FD_CLR(fileno(fp), &rset);
				continue;
			}
			writen(sockfd, sendline, strlen(sendline));
		}
	}
}