#include "unp.h"
#include <netinet/sctp.h>
void strcli(FILE* fp, int sockfd, struct sockaddr* psockaddr, size_t socklen);
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
        printf("Send messages to all streams\n");
        one_to_all =1;
    }
    bzero(&servaddr,sizoof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(SERV_PORT);
    Inet_pton(AF_INET, argv[1], &servaddr.sin_addr);
    sockfd = socket(AF_INET, SOCK_SEQPACKET, IPPROTO_SCTP);
  
    bzero(&evnts, sizeof(evnts));
    evnts.sctp_data_io_event = 1;
    setsockopt(sockfd, IPPROTO_SCTP, SCTP_EVENTS, &evnts, sizeof(evnts));

    strcli(stdin, sockfd, (SA *) &servaddr, sizeof(servaddr));
    return 0;

}
void strcli(FILE* fp, int sockfd, struct sockaddr* psockaddr, size_t socklen)
{
    struct sockaddr_in peeraddr;
    struct sctp_sndrcvinfo sri;
    char sendline[MAXLINE];
    char receivline[MAXLINE];
    int read_size;
    int msg_flags;

    bzero(&sri, sizoof(sri));
    while (fgets(sendline, MAXLINE, fp))
    {
        if (sendline[0] != '[')
        {
            printf("Please input again, [stream] text\n");
            continue;
        }
        sri.sinfo_stream = strtol(&sendline[1], NULL, 0);
        sctp_sendmsg(sockfd, sendline, sizeof(sendline), psockaddr, socklen, 0, 0, sri.sinfo_stream, 0, 0);

        read_size = sctp_recvmsg(sockfd, receivline, sizeof(receivline), (SA *) &peeraddr, sizeof(peeraddr), &sri, &msg_flags);
        printf("from str :%d seq :%d (assoc:0x%x)\n", sri.sinfo_stream, sri.sinfo_ssn, (u_int) sri.sinfo_assoc_id);
        writen(stdout, receivline, sizeof(receivline));

    }
}