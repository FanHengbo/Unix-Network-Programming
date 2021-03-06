#include "unp.h"

int main(int argc, char** argv)
{
    char *ptr;
    char **pptr;
    struct hostent *hptr;
    struct hostent *hostname_ptr;
    char buf[INET_ADDRSTRLEN];

    while(--argc > 0)
    {
        ptr = *++argv;
        if ((hptr = gethostbyname(ptr)) == NULL)
        {
            printf("gethostbyname error\n");
            continue;
        }
    
        printf("official hostname : %s\n", hptr->h_name);
        for (pptr = hptr->h_aliases; *pptr != NULL; pptr++)
        {
            printf("aliases: %s\n", *pptr);
        }
        switch(hptr->h_addrtype)
        {
            case AF_INET:
            for (pptr =hptr->h_addr_list; *pptr; pptr++)
            {
                printf("addresss : %s\n", inet_ntop(AF_INET, *pptr, buf, sizeof(buf)));
                hostname_ptr = gethostbyaddr(*pptr, hptr->h_length, hptr->h_addrtype);
                printf("name is  : %s\n", hostname_ptr->h_name);
            }
            break;
        }
    
    }
    return 0;

}