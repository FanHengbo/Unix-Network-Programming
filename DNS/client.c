#include "unp.h"

int main(int argc, char** argv)
{
    char *ptr;
    char **pptr;
    struct hostent *hptr;

    while(--argc > 0)
    {
        ptr = *++argv;
        if (hptr = gethostbyname(ptr) == NULL)
        {
            printf("gethostbyname error\n");
            exit(0);
        }
    }
    printf("official hostname : %s\n", hptr->h_name);
    for (pptr = hptr->h_aliases; *pptr != NULL; pptr++)
    {
        printf("aliases: %s\n", *pptr);
    }
    

    return 0;

}