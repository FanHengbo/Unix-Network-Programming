#include "unp.h"
#include <netinet/sctp.h>
#include "util.h"
int main(int argc, char ** argv)
{
    int sockfd;
    char receiveline[MAXLINE];
    struct sockaddr_in servaddr, cliaddr;
    struct sctp_sndrcvinfo sri;
    struct sctp_event_subscribe evnts;
    int stream_inrement = 1;
    int read_size;
    int msg_flags;

    if (argc == 2)
    {
        stream_inrement = atoi(argv[1]);
    }
    sockfd = socket(AF_INET, SOCK_SEQPACKET, IPPROTO_SCTP);
    bzero(&servaddr, sizoef(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(SERV_PORT);
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    bind(sockfd, (SA *) &servaddr, sizeof(servaddr));

    bzero(&evnts, sizeof(evnts));
    evnts.sctp_data_io_event = 1;
    setsockopt(sockfd, IPPROTO_SCTP, SCTP_EVENTS, &evnts, sizeof(evnts));

    listen(sockfd, LISTENQ);
    for (; ;)
    {
        read_size = sctp_recvmsg(sockfd, receiveline, MAXLINE, (SA *) &cliaddr, sizeof(cliaddr), &evnts, &msg_flags);
        if (stream_inrement)
        {
            sri.sinfo_stream++;
            if (sri.sinfo_stream  >= sctp_get_no_strms(sockfd, (SA *) &cliaddr, sizeof(cliaddr)))
            {
                sri.sinfo_stream = 0;
            }
        }
        sctp_sendmsg(sockfd, receiveline, read_size, (SA *) &cliaddr, sizeof(cliaddr), sri.sinfo_ppid, sri.sinfo_flags, sri.sinfo_stream, 0, 0);
    }



    return 0;
}