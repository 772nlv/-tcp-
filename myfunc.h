#ifndef __FUNC_H_
#define __FUNC_H_
#include <myhead.h>
#define REG 1 //登录信息
#define LOGIN 2  //注册信息

#define QUNLIAO 3 //群聊信息
#define EXIT_SER 4 //下线
#define EXIT_QUNL 5 //退出群聊
#define PAWD_FIND 6 //找回密码信息

#define ADD_FRIENDS 7 //申请加好友信息

#define CHANGE_PSD 8 //修改密码
#define CHANGE_PSD_OK 99//修改密码成功

#define LIST_SHOW 9 //好友列表展示
#define LIST_SHOW_OK 10//好友列表展示成功

#define FRE_ERROR 11 //好友不存在
#define EXIT_LOGIN 12//退出群聊

#define ON_LINE 13  //用户在线

#define PRI_CHAT 14 //私聊请求

#define EXIT_PRI 15//退出私聊

#define SHOW_GROUP_HISCHAT 16 //查看群聊记录
#define SHOW_GROUP_HISCHAT_OK 17

#define SHOW_PRI_HISCHAT 18 //查看私聊记录
#define SHOW_PRI_HISCHAT_OK 19

#define UPLOAD_FILE 20  //上传文件
#define TRANSPORD_FILE 21 //转发文件
#define TRANSPORD_FILE_OK 22

#define ENTRE_QUNLIAO 23    //进入群聊
#define ENTRE_PRICHAT 24    //进入私聊

#define ASKSERVER_FRI 25  //询问服务器有没有好友申请
#define UNHANDLE 26     //申请好友信息未处理
#define OVER 27         //申请好友信息_读到末尾

#define AGREE 28        //同意添加好友
#define OBJECT 29
#define HANDLE 30       //处理好友请求


#define OK 111
#define NO 222

//定义结构体来存放客户机的信息
typedef struct user_info
{
    int sd;
    struct sockaddr_in Addr;
} Addr;

//定义一个结构体数组来记录来登录用户的网络信息
// Addr ClientInfo[999];
// int ClientIndex = 0;

//客户端注册用户信息结构体
typedef struct zhuce
{
    char name[128];
    char passwd[32];
    char tel[12];
} zc;

//客户端用户信息结构体
typedef struct denglu
{
    char name[128];
    char passwd[32];
    char tel[12];
} dl;

//加好友功能信息结构体
typedef struct Friend
{
    char Myname[128];
    char Destname[128];
    char data[256];
} frd;

//定义结构体来存放客户机和服务器传输的文件内容信息
typedef struct Air
{
    unsigned char type; //数据类型标志
    int count;           //好友的个数
    char data[1023*100];  
} Aircraft;


//定义结构体来存放客户机和服务器传输的信息
typedef struct buf
{
    unsigned char type; //数据类型标志
    char data[1023];    //传输数据
} Boat;

//群聊聊天信息的内容
typedef struct com
{
    unsigned char type; //消息类型标志
    char name[128];     //发送消息的用户名
    char data[256];     //消息的内容

} comu;

//申请加好友的结构体
typedef struct add
{
    unsigned char type;   //好友请求的状态
    char Myname[128];     //自己的用户名
    char Destname[128];    //加好友对象的用户名
}add_fri;

//私聊聊天信息的内容
typedef struct chat
{
    unsigned char type; //消息类型标志
    char Myname[128];     //发送消息的用户名
    char Destname[128];    //私聊对象用户名
    char data[256];     //消息的内容

} Pri_chat_st;

//聊天记录结构体
typedef struct His_
{
    char name[129];
    char data[256];
    char time[129];
    int count;
} His;

//上传文件(转发，接收)结构体
typedef struct file
{
    char username[128];       //用户名
    char file_name[256];     //文件名
    char data[512];     //文件内容传输的缓冲区，大小为512

} file_st;


//显示欢迎界面
void ShowMenu();

//登录成功后 显示用户功能列表；
void ShowFunction();

//登录页面选择
void ChooseLogin(int sd,struct sockaddr_in addr);

//客户端退出
void Cli_Eixt(int sd, int maxFD,fd_set *set_1, fd_set *set_2,fd_set *set_3);

//群聊功能
void Qunliao_chat(int sd);

//服务器转发群聊信息
int Ser_Qunliao(comu *com, int sd, int maxFD, fd_set *set);

//客户端群聊子线程工作函数
void *cli_qunliao_work(void *p);

//客户端子线程工作函数
void *cli_work(void *p);

//登录
void Mylogin(int sd, struct sockaddr_in addr);

//注册
void Myregister(int sd, struct sockaddr_in addr);

//服务器注册验证
int Myregister_SER(zc *Pzc, int sd, struct sockaddr_in addr, fd_set *set);

//服务器登录验证
int Mylogin_SER(dl *pdl, int sd, int maxFD,struct sockaddr_in addr, fd_set *set1);

//接收来自服务器登录、注册回应信息
int RecloginAndReg(struct sockaddr_in addr, int sd);

//功能选择
void ChooseFunction(int sd,struct sockaddr_in addr);

//找回密码
int Find_pawd(int sd,struct sockaddr_in addr);

//找回密码 服务器
int MyFindpw_SER(zc *Pdfw, int sd, struct sockaddr_in addr);

//修改密码 客户端
int Change_pw(zc * fpw,int sd,struct sockaddr_in addr);

//修改密码 服务器
int Change_pw_SER(zc * fpw,int sd);

//好友功能选项页面
void ShowFriendsfunc();

//好友功能 客户机
int Friends_func(int sd,struct sockaddr_in addr);

//加好友 客户机
int Friend_Add(struct sockaddr_in addr,int sd);

//加好友 服务器
int Friend_Add_SER(add_fri *Pfr, int sd);

//查看好友列表
void Friend_List_show(int sd);

//查看好友列表 服务器
void Friend_List_show_SER(int sd);

//通过名字找用户的套接字
int Name_Find_Id(char *tname,int maxFD);

//私聊
void Pri_chat(int sd);

//私聊 服务器
int Pri_chat_SER(int sd,int maxFD,Pri_chat_st *Ppr_st,fd_set *set);

//私聊——客户机收信息
void *Pri_work(void *p);

//获取当前时间
char *Gettime();

//按任意键退出
void AnyQuit();

//查看聊天记录功能选项
int Show_chat_history(int sd, struct sockaddr_in addr);

//将群聊天记录写进文档
int Group_His_Enter_file(void *p, His *Phis, int sd);

//将私聊记录写进文档
int Pri_His_Enter_file(void *p, His *Phis, int sd);

//查看群聊天记录 客户端
int Show_Group_chat_history(int sd);

//查看群聊天记录 服务器
int Show_Group_chat_history_SER(int sd);

//查看私聊记录 服务器
int Show_Pri_chat_history_SER(int sd,Boat buf);

//查看私聊天记录 客户端
int Show_Pri_chat_history(int sd);

//聊天记录选项页面
void ShowHisChatChoose();

//文件功能选项页面
void ShowFilefuncChoose();

//文件功能选择
int File_func(int sd, struct sockaddr_in addr);

//上传文件功能
int Uploat_file(int sd);

//上传文件功能——服务器
int Uploat_file_SER(int sd, file_st *pfile);

//转发文件功能——客户机
int Transpond_file(int sd);

//转发文件功能——服务器
int Transpond_file_SER(int sd,Boat * buf,int maxID);

//接收文件——客户机
int Receive_file(int sd);

//询问服务器有没有好友申请
void Query_Server(int sd);

//查看有没有好友申请———服务器
int Query_Server_SER(int sd, Boat *buf);

//处理新朋友信息
int Handle_Addfriends(int sd);

//处理新朋友信息————服务器
int Handle_Addfriends_SER(int sd,add_fri *phandle);
#endif