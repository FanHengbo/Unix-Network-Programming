#include "unpifi.h"
#include "utils.h"

int main(int argc, char **argv)
{
    struct ifi_info *ifi, *ifihead;
    struct sockaddr *sa;
    struct sockaddr_in client;
    int family;
    int alias;
    int index;
    int len;

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
        if ((index = ifi->ifi_index) != 0)
        {
            printf("Index %d", index);
        }
        printf("<");
        if (ifi->ifi_flags & IFF_UP)
            printf("UP ");
        if (ifi->ifi_flags & IFF_BROADCAST)
            printf("BROADCAST ");
        if (ifi->ifi_flags & IFF_MULTICAST)
            printf("MULTICAST ");
        if (ifi->ifi_flags & IFF_LOOPBACK)
            printf("LO ");
        if (ifi->ifi_flags & IFF_POINTOPOINT)
            printf("P2P");
        printf(">\n");
        if ((len = ifi->ifi_hlen) > 0)
        {
            for (int i = 0; i < len; i++)
            {
                printf("%s%x", (i == len-1) ? " " : ":", ifi->ifi_haddr[i]);
            }
            printf("\n");
        }
        if (ifi->ifi_mtu)
        {
            printf("MTU : %d\n", ifi->ifi_mtu);
        }
        if ((sa = ifi->ifi_addr))
        {
            printf("IP address is %s\n", sock_ntop(sa, sizeof(*sa)));
        }
        if ((sa = ifi->ifi_brdaddr))
        {
            printf("Broadcast address is %s\n", sock_ntop(sa, sizeof(*sa)));
        }
        if ((sa = ifi->ifi_dstaddr))
        {
            printf("Destination address is %s\n", sock_ntop(sa, sizeof(*sa)));
        }
    }
    free_ifi_info(ifihead);

    return 0;
}