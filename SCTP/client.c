#include "unp.h"
#include <linux/sctp.h>

int main(int argc, char** argv)
{
    struct sctp_event_subscribe evnts;
    int sockfd;
    struct sockaddr_in servaddr;
    int one_to_all = 0;

    if (argc < 2)
    {
        printf("Usage: client <IP address> ...if any...<IP address>");
        exit(0);
    }
    if (argc > 2)
    {
        one_to_all =1;
    }
    bzero(&servaddr,sizoof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(SERV_PORT);
    Inet_pton(AF_INET, argv[1], &servaddr.sin_addr);
    sockfd = socket(AF_INET, SOCK_SEQPACKET, IPPROTO_SCTP);


    return 0;

}