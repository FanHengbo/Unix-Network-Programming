#include "unp.h"
#include "utils.h"
int main(int argc, char **argv)
{
    if (argc != 3)
    {
        printf("Usage: udpcli <hostname> <service>\n");
        exit(EXIT_FAILURE);
    }
    int sockfd;
    int n;
    int recesize;
    char receiveline[MAXLINE];
    //char sendline[MAXLINE];
    struct addrinfo *result, *result_tranverse;
    struct addrinfo hint;
    char *hostname = argv[1];
    char *service = argv[2];
    struct sockaddr servaddr, cliaddr;
    socklen_t len, len_cli;

    
    bzero(&hint, sizeof(hint));
    hint.ai_family = AF_UNSPEC;
    hint.ai_socktype = SOCK_DGRAM;
    if ((n = getaddrinfo(hostname, service, &hint, &result)) < 0)
    {
        printf("getaddrinfo error for host: %s with service: %s, with error messsage: %s\n", hostname, service, gai_strerror(n));
        exit(EXIT_FAILURE);
    }

    for (result_tranverse = result; result_tranverse; result_tranverse = result_tranverse->ai_next)
    {
        if ((sockfd = socket(result_tranverse->ai_family, result_tranverse->ai_socktype, result_tranverse->ai_protocol)) >= 0)
        {
            break;           
        }

    }
    if (result_tranverse == NULL)
    {
        printf("udp connection error for host %s with service %s\n", hostname, service);
        exit(EXIT_FAILURE);
    }
    memcpy(&servaddr, result_tranverse->ai_addr, result_tranverse->ai_addrlen);
    freeaddrinfo(result);

    len = sizeof(servaddr);
    len_cli = sizeof(cliaddr);
    getsockname(sockfd, (SA *) &cliaddr, &len_cli);
    printf("local address is %s\n", sock_ntop((SA *) &cliaddr, len_cli));
    printf("sending information to %s\n", sock_ntop((SA *) &servaddr, len));
    
    //sendto(sockfd, "", 1, 0 ,(SA *) &servaddr, len);
    recesize = recvfrom(sockfd, receiveline, MAXLINE, 0, NULL, NULL);
    receiveline[recesize] = 0;
    fputs(receiveline, stdout);
    

    return 0;
}