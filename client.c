#include <myhead.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include "myfunc.h"
int main(int argc, const char *argv[])
{
    //客户机
    //获取ip
    u_int32_t ip;
    inet_pton(AF_INET, "192.168.40.148", &ip);
    struct sockaddr_in addr;
    addr.sin_addr.s_addr = ip;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(6666);

    //获取一个套接字
    int sd = socket(AF_INET, SOCK_STREAM, 0);
    //连接服务器
    int ret = connect(sd, (struct sockaddr *)&addr, sizeof(addr));
    printf("%d\n", ret);
    if (ret < 0)
    {
        PRINT_ERR("");
    }
    while (1)
    {
        //登录页面选择
        ChooseLogin(sd,addr);
    }
    close(sd);
    return 0;
}