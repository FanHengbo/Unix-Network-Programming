#include "unpifi.h"
int main(int argc, char **argv)
{
    struct ifi_info *ifi, *ifihead;
    struct sockaddr *sa;
    struct sockaddr_in client;
    int family;
    int alias;

    if (argc != 3)
    {
        printf("Usage: client <inet4/inet6> <display alias?>\n");
        exit(EXIT_FAILURE);
    }
    if (strcmp(argv[1], "inet4") == 0)
    {
        family = AF_INET;
    }
    else if(strcmp(argv[1], "inet6") == 0)
    {
        family = AF_INET6;
    }
    else
    {
        printf("Invalid family\n");
        exit(EXIT_FAILURE);
    }
    alias = atoi(argv[2]);
    for (ifi = ifihead = get_ifi_info(family, alias); ifi; ifi = ifi->ifi_next)
    {

        printf("%s\n", ifi->ifi_name);
        
    }

    return 0;
}