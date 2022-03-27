#include "unp.h"
#include "utils.h"
int main(int argc, char **argv)
{
    if (argc != 2)
    {
        printf("Usage daytimeserv <service>\n");
        exit(EXIT_FAILURE);
    }

    int listenfd, connfd;
    int n;
    const int on = 1;
    struct addrinfo hint, *result, *result_tranverse;
    char buf[MAXLINE];
    struct sockaddr_storage cliaddr;
    socklen_t len;
    time_t ticks;

    bzero(&hint, sizeof(hint));
    hint.ai_family = AF_UNSPEC;
    hint.ai_flags = AI_PASSIVE;
    hint.ai_socktype = SOCK_STREAM;
    
    if ((n = getaddrinfo(NULL, argv[1], &hint, &result)) < 0)
    {
        printf("getaddrinfo error for server with service %s, error message: %s\n", argv[1], gai_strerror(n));
        exit(EXIT_FAILURE);
    }
    for (result_tranverse = result; result_tranverse; result_tranverse = result_tranverse->ai_next)
    {
        if ((listenfd = socket(result_tranverse->ai_family, result_tranverse->ai_socktype, result_tranverse->ai_protocol)))
        {
            continue;
        }
        setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));
        if (bind(listenfd, result_tranverse->ai_addr, result_tranverse->ai_addrlen) == 0)
        {
            break;
        }
        close(listenfd);
    }
    if (result_tranverse == NULL)
    {
        printf("Couldn't connect for server with service: %s\n", argv[1]);
        exit(EXIT_FAILURE);
    }
    listen(listenfd, LISTENQ);
    len = result_tranverse->ai_addrlen;
    freeaddrinfo(result_tranverse);

    for (; ;)
    {
        connfd = accept(listenfd, (SA *) &cliaddr, &len);
        printf("Connection from %s\n", sock_ntop((SA *) &cliaddr, len));

        ticks = time(NULL);
        snprintf(buf, sizeof(buf), "%s\r\n", ctime(&ticks));
        write(connfd, buf, sizeof(buf));

        close(connfd);
    }



    return 0;
}