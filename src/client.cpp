#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

int main(int argc, char const *argv[])
{
    const char *ip = "192.168.10.11";

    // 字符串转int
    int port = 6500;

    // 协议族, PF_INET ,ipv4
    int sock = socket(PF_INET, SOCK_STREAM, 0);
    if (!(sock > 0))
    {
        printf("error\n");
        return -1;
    }

    struct sockaddr_in address;

    bzero(&address, sizeof(address));

    // AF_INET 地址族,ipv4
    address.sin_family = AF_INET;

    // 设置ip地址，转网络字节序
    inet_pton(AF_INET, ip, &address.sin_addr);

    // 设置端口， host to network short
    address.sin_port = htons(port); // 端口号，需要用网络字节序

    int ret = connect(sock, (struct sockaddr *)&address, sizeof(address));
    if (ret != 0)
    {
        printf("connect error %s\n", strerror(errno));
        close(sock);
        return -1;
    }
    else
    {
        const char *data = "hello啊";
        send(sock, data, sizeof(data), 0);
    }

    close(sock);

    return 0;
}
