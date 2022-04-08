#include "unp.h"
ssize_t						/* Write "n" bytes to a descriptor. */
writen(int fd, const void *vptr, size_t n)
{
	size_t		nleft;
	ssize_t		nwritten;
	const char	*ptr;

	ptr = vptr;
	nleft = n;
	while (nleft > 0) {
		if ( (nwritten = write(fd, ptr, nleft)) <= 0) {
			if (nwritten < 0 && errno == EINTR)
				nwritten = 0;		/* and call write() again */
			else
				return(-1);			/* error */
		}

		nleft -= nwritten;
		ptr   += nwritten;
	}
	return(n);
}
void str_echo(int sock)
{
    ssize_t n;
    char buf[MAXLINE];
    
again:
    while((n = read(sock, buf, MAXLINE)) > 0)
    {
        writen(sock, buf, n);
    }
    if (n < 0 && errno == EINTR)
        goto again;
    else if (n < 0)
    {
        printf("str_echo read error\n");
        exit(EXIT_FAILURE);
    }
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