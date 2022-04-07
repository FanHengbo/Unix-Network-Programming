#include "unp.h"
void sig_child();
int main(int argc, char **argv)
{
    int listefd, connfd;
    struct sockaddr_un servaddr, cliaddr;
    
    listefd = socket(AF_LOCAL, SOCK_STREAM, 0);
    
    unlink(UNIXSTR_PATH);

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sun_family = AF_LOCAL;
    strcpy(servaddr.sun_path, UNIXSTR_PATH);
    
    listen(listefd, LISTENQ);
    signal(SIGCHLD, sig_child);
    return 0;
}
void sig_child()
{
    
}