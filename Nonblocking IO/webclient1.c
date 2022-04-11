#include "unp.h"
#define MAXFILE 20
#define SERVPORT 80

struct file
{
    char *fileName;
    char *fileHost;
    int fd;
    int fileFlags;
}file[MAXFILE];
#define GET_CMD "GET %s HTTP/1.0\r\n\r\n"

fd_set rset, wset;

void HomePage(const char *, const char *);
void StartConnect(struct file *);
void WriteGetCmd(struct file *);
