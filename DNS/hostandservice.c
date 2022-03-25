#include "unp.h"

int main(int argc, char** argv)
{
    struct servent *serv_ptr;
    struct hostent *host_ptr;
    struct in_addr innet_addr;
    struct in_addr *list_addr[2];
    struct in_addr **pptr;
    struct sockaddr_in servaddr;
    int sockfd;
    int n;
    char buf[MAXLINE];
    if (argc != 3)
    {
        printf("Error usage. Usage: hostandservice <hostname> <servicec>\n");
        exit(0);
    }
    if ((host_ptr = gethostbyname(argv[1])) == NULL)
    {
        if (inet_aton(argv[1], &innet_addr) == 0)
        {
            printf("Invalid hostname\n");
            exit(0);
        }
        else
        {
            list_addr[0] = &innet_addr;
            list_addr[1] = NULL;
            pptr = list_addr;
        }

    }
    else
    {
        pptr = (struct in_addr **)host_ptr->h_addr_list;
    }
    if ((serv_ptr = getservbyname(argv[2], "tcp")) == NULL)
    {
        printf("Can't handle this service \n");
        exit(0);
    }
    for (; *pptr; pptr++)
    {
        sockfd = socket(AF_INET, SOCK_STREAM, 0);

        bzero(&servaddr, sizeof(servaddr));
        servaddr.sin_family = AF_INET;
        servaddr.sin_port = serv_ptr->s_port;
        memcpy(&servaddr.sin_addr.s_addr, *pptr, sizeof(struct in_addr));
        printf("tring connet to %s \n", sock_ntop((SA *)&servaddr, sizeof(servaddr)));
        if (connect(sockfd, (SA *)&servaddr, sizeof(servaddr)) == 0)
            break;

        close(sockfd);
    }
    if (*pptr == NULL)
    {
        printf("Unable to connect \n");
        exit(0);
    }
    while((n = read(sockfd, buf, MAXLINE)))
    {
        buf[n] = 0;
        fputs(buf, stdout);
    }
    return 0;
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