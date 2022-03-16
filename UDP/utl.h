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

char *
sock_ntop(const struct sockaddr *sa, socklen_t salen)
{
    char		portstr[8];
    static char str[128];		/* Unix domain is largest */

	switch (sa->sa_family) {
	case AF_INET: {
		struct sockaddr_in	*sin = (struct sockaddr_in *) sa;

		if (inet_ntop(AF_INET, &sin->sin_addr, str, sizeof(str)) == NULL)
			return(NULL);
		if (ntohs(sin->sin_port) != 0) {
			snprintf(portstr, sizeof(portstr), ":%d", ntohs(sin->sin_port));
			strcat(str, portstr);
		}
		return(str);
	}
    }
}
void
str_echo12(int sockfd, struct sockaddr *paddr, socklen_t len)
{
	ssize_t		n;
	char		buf[MAXLINE];

again:
	while ( (n = read(sockfd, buf, MAXLINE)) > 0)
    {
        printf("Receive TCP client message from : %s\n", sock_ntop(paddr, len));
        writen(sockfd, buf, n);

    }
		
	if (n < 0 && errno == EINTR)
		goto again;
	else if (n < 0)
    {
        printf("str_echo: read error\n");
        exit(0);
    }
		
}