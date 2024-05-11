#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <errno.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <netdb.h>

#include <iostream>
#include <fstream>
using namespace std;

#define BUF_SIZE 1024

const char file_end[] = {0x16, 0x16, 0x16};

void test_read_file(int fd)
{
    ifstream fin("/home/ros/code_/test_socket/2.jpeg", ios::in | ios::binary);
    if (!fin)
    {
        cout << "fopen error." << endl;
        fin.close();
        return;
    }

    // 获取文件长度
    fin.seekg(0, ios::end);
    long length = fin.tellg();
    fin.seekg(0, ios::beg);

    cout << "length:" << length << endl;

    char buf[BUF_SIZE] = {0};

    while (!fin.eof())
    {
        fin.read(buf, sizeof(buf));
        //获取度读取到的字节大小
        std::streamsize bytesRead = fin.gcount();

        send(fd, buf, bytesRead, 0);
        bzero(buf, BUF_SIZE);
    }
    fin.close();
}

void test_gethostbyname()
{
    struct hostent *host_name = gethostbyname("ros");
    char **pptr = NULL;

    // pptr++，指针向后移动一个元素,int**类型的数组也是
    char domain[INET_ADDRSTRLEN];
    for (pptr = host_name->h_addr_list; *pptr != NULL; pptr++)
    {
        printf("address:%s\n", inet_ntop(host_name->h_addrtype, *pptr, domain, INET_ADDRSTRLEN));
    }
    // address.sin_addr = *(struct in_addr *)*host_name->h_addr_list;
}

void test_getAddrInfo()
{
    struct addrinfo hints;
    // getaddrinfo("ros",);
}

#include <iostream>
#include <iomanip>
#include <vector>

int main()
{
    const char *ip = "0.0.0.0";

    // 字符串转int
    int port = atoi("6500");
    int backlog = atoi("64");

    // 协议族, PF_INET ,ipv4
    int sock = socket(PF_INET, SOCK_STREAM, 0);
    if (!(sock > 0))
    {
        printf("create socket error\n");
        return -1;
    }

    /*
        tcp协议族 socket地址 结构体

        struct sockaddr_in
        {
            sa_family_t sin_family;//地址族：AF_INET
            in_port_t sin_port;//端口号，需要网络字节序
            struct in_addr sin_addr;//IPV4地址结构体
        };

        struct in_addr
        {
            u_int32_t saddr;//ipv4地址，需要用网络字节序
        };
    */
    struct sockaddr_in address;

    bzero(&address, sizeof(address));

    // AF_INET 地址族,ipv4
    address.sin_family = AF_INET;

    // 设置ip地址，转网络字节序
    inet_pton(AF_INET, ip, &address.sin_addr);

    // 设置端口， host to network short
    address.sin_port = htons(port); // 端口号，需要用网络字节序

    // 设置为开启状态
    int reuse_addr = 1;
    // socket级别,设置可重用地址
    if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, (const void *)&reuse_addr, sizeof(reuse_addr)) != 0)
    {
        printf("bind error %s\n", strerror(errno));
        close(sock);
        return -1;
    }

    // 绑定
    int ret = bind(sock, (struct sockaddr *)&address, sizeof(address));
    if (ret != 0)
    {
        printf("bind error %s\n", strerror(errno));
        close(sock);
        return -1;
    }

    // 监听
    ret = listen(sock, backlog);
    if (ret != 0)
    {
        printf("listen error\n");
        close(sock);
        return -1;
    }

    // 客户端地址
    struct sockaddr_in client_addr;
    // 客户端地址大小
    socklen_t client_len = sizeof(client_addr);

    int cfd = accept(sock, (struct sockaddr *)&client_addr, &client_len);
    if (cfd < 0)
    {
        printf("accept error\n");
        close(sock);
        return -1;
    }
    else
    {
        char remot[INET_ADDRSTRLEN];
        printf("client ip:%s,port:%d\n", inet_ntop(AF_INET, &client_addr.sin_addr, remot, INET_ADDRSTRLEN), ntohs(client_addr.sin_port));
        char data[4];
        bzero(data, sizeof(data));
        while ((ret = recv(cfd, data, sizeof(data), 0)) > 0)
        {
            printf("%d\n", ret);
            printf("size = %d,data:%x %x %x\n", ret, data[0], data[1], data[2]);
            if (data[0] == 0x16 && data[1] == 0x16 && data[2] == 0x16)
            {
                printf("statr read file...\n");
                test_read_file(cfd);
                sleep(5);
                
                printf("send file_end size = %d.\n",sizeof(file_end));
                send(cfd, file_end, sizeof(file_end), 0);
                printf("send file_end over.\n");
            }
        }
        // close(sock);
    }

    // 关闭,引用计数为0才关闭fd
    // close(sock);
    sleep(100);
    close(sock);
    return 0;
}
