#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <net/if.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <linux/can.h>
#include <linux/can/raw.h>

int main(void)
{
    int sock = socket(PF_CAN, SOCK_RAW, CAN_RAW);
    struct sockaddr_can addr;
    struct ifreq ifr;
    struct can_frame frame;

    strcpy(ifr.ifr_name, "vcan0");
    ioctl(sock, SIOCGIFINDEX, &ifr);

    addr.can_family = AF_CAN;
    addr.can_ifindex = ifr.ifr_ifindex;
    bind(sock, (struct sockaddr *)&addr, sizeof(addr));

    printf("CAN Monitor\n");

    while (1) {
        if (read(sock, &frame, sizeof(frame)) <= 0)
            continue;

        printf("ID=0x%03X DLC=%d DATA=",
               frame.can_id, frame.can_dlc);

        for (int i = 0; i < frame.can_dlc; i++)
            printf("%02X ", frame.data[i]);

        printf("\n");
    }

    close(sock);
    return 0;
}
