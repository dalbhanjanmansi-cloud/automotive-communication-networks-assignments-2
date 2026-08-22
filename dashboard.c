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

    printf("Vehicle Dashboard\n");

    while (1) {
        if (read(sock, &frame, sizeof(frame)) <= 0)
            continue;

        if (frame.can_id == 0x100) {
            int rpm = frame.data[0] |
                      (frame.data[1] << 8);
            int temperature = frame.data[2] - 40;

            printf("RPM: %d rpm | Temperature: %d C\n",
                   rpm, temperature);
        }

        if (frame.can_id == 0x200) {
            int raw = frame.data[0] |
                      (frame.data[1] << 8);

            printf("Vehicle Speed: %.2f km/h\n",
                   raw * 0.01);
        }
    }

    close(sock);
    return 0;
}
