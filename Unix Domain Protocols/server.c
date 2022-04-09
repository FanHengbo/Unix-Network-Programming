#include "unp.h"
#include "util.h"

void sig_child();
int main(int argc, char **argv)
{
    int listefd, connfd;
    struct sockaddr_un servaddr, cliaddr;
    socklen_t clialen;
    
    listefd = socket(AF_LOCAL, SOCK_STREAM, 0);
    
    unlink(UNIXSTR_PATH);

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sun_family = AF_LOCAL;
    strcpy(servaddr.sun_path, UNIXSTR_PATH);

    bind(listefd, (SA *) &servaddr, sizeof(servaddr));
    
    listen(listefd, LISTENQ);
    signal(SIGCHLD, sig_child);

    for (; ;)
    {
        clialen = sizeof(cliaddr);
        connfd = accept(listefd, (SA *) &cliaddr, &clialen);
        if (connfd  < 0)
        {
            if (errno == EINTR)
            {
                continue;
            }
            else
            {
                printf("accept failed\n");
                exit(EXIT_FAILURE);
            }
        }
        if (fork() == 0) //Child
        {
            close(listefd);
            str_echo(connfd);
            exit(0);
        }
        close(connfd);
    }
    return 0;
}
void sig_child()
{
    pid_t child_pid;
    int stat;

    while((child_pid = waitpid(-1, &stat, WNOHANG)) > 0)
    {
        printf("Child %d terminated\n", child_pid);
    }
    return;
}