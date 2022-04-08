#include "unp.h"
#include "util.h"

int main(int argc, char **argv)
{
    int sockfd;
    struct sockaddr_un seraddr;
    socklen_t servlen;

    sockfd = socket(AF_LOCAL, SOCK_STREAM, 0);
    bzero(&seraddr, sizeof(seraddr));
    seraddr.sun_family = AF_LOCAL;
    strcpy(seraddr.sun_path, UNIXSTR_PATH);
    servlen = sizeof(seraddr);
    connect(sockfd, (SA *) &seraddr, servlen);

    str_cli1(stdin, sockfd);
    return 0;
}