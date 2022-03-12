#include "unp.h"
#include "util.h"
#include <limits.h>

int 
main(int argc, char** argv)
{
    int listenfd, connfd;
    int maxi;   //Maximum current client index
    int nready; //return value of pull(), number oof ready fd 
    int i;
    int OPEN_MAX = sysconf(_SC_OPEN_MAX);

    //printf("OPEN_MAX is %d\n", OPEN_MAX);
   
    struct sockaddr_in cliaddr, servaddr;
    struct pollfd client[OPEN_MAX];
    
    listenfd = socket(AF_INET, SOCK_STREAM, 0);
    bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(SERV_PORT);
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);

    bind(listenfd, (SA *) &servaddr, sizeof(servaddr));
    listen(listenfd, LISTENQ);
    client[0].fd = listenfd;
    client[0].events = POLLRDNORM;
    for (i = 1;i < OPEN_MAX;i++)
    {
        client[i].fd = -1; //Initialization
    }
    maxi = 0;

    for ( ; ;)
    {
        nready = poll(client, maxi+1, INFTIM);

        if (client[0].revents & POLLRDNORM)
        {
            size_t clilen = sizeof(cliaddr);
            connfd = accept(listenfd, (SA *) &cliaddr, clilen);

            for (i = 1; i < OPEN_MAX; i++)
            {
                if (client[i].fd < 0)
                {
                    client[i].fd = connfd;
                    break;
                }
            }
            if (i == OPEN_MAX)
            {
                printf("Too many clients\n");
                exit(0);
            }
            if (i > maxi)
                maxi = i;
            if (--nready <= 0)
                continue;

        }
        for (i = 1; i <= maxi; i++) //check if any client's data is ready
        {
            if (client[i].fd < 0)
                continue;
            if (client[i].revents & (POLLRDNORM | POLLERR)
            {

            }    
        }
    }
    return 0;
}