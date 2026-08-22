#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <string.h>
#include <net/if.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <linux/can.h>
#include <linux/can/raw.h>

int main(void)
{
    int sock;
    struct sockaddr_can addr;
    struct ifreq ifr;

    sock = socket(PF_CAN, SOCK_RAW, CAN_RAW);
    if (sock < 0) {
        perror("socket");
        return 1;
    }

    strcpy(ifr.ifr_name, "vcan0");
    ioctl(sock, SIOCGIFINDEX, &ifr);

    addr.can_family = AF_CAN;
    addr.can_ifindex = ifr.ifr_ifindex;
    bind(sock, (struct sockaddr *)&addr, sizeof(addr));

    printf("Engine ECU started.\n");

    int rpm = 2500;
    int temperature = 90;
    int speed = 65;

    while (1) {
        struct can_frame engine;
        engine.can_id = 0x100;
        engine.can_dlc = 8;

        engine.data[0] = rpm & 0xFF;
        engine.data[1] = (rpm >> 8) & 0xFF;
        engine.data[2] = temperature + 40;

        for (int i = 3; i < 8; i++)
            engine.data[i] = 0;

        write(sock, &engine, sizeof(engine));

        struct can_frame vehicle;
        vehicle.can_id = 0x200;
        vehicle.can_dlc = 8;

        int speed_raw = speed * 100;
        vehicle.data[0] = speed_raw & 0xFF;
        vehicle.data[1] = (speed_raw >> 8) & 0xFF;

        for (int i = 2; i < 8; i++)
            vehicle.data[i] = 0;

        write(sock, &vehicle, sizeof(vehicle));

        printf("Sent: RPM=%d Temp=%d C Speed=%d km/h\n",
               rpm, temperature, speed);

        sleep(1);
    }

    close(sock);
    return 0;
}
