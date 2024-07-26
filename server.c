#include <myhead.h>
#include "myfunc.h"
extern char clientname[999][128];

int main(int argc, const char *argv[])
{
    //获取一个网络地址
    u_int32_t ip;
    inet_pton(AF_INET, "192.168.40.148", &ip);

    struct sockaddr_in addr;
    addr.sin_addr.s_addr = ip;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(6666);
    // 1、获取一个socket的id
    int Listenid = socket(AF_INET, SOCK_STREAM, 0);
    int opt = 1;
    if (setsockopt(Listenid, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0)
    {
        perror("");
    }
    //绑定网络地址
    bind(Listenid, (struct sockaddr *)&addr, sizeof(addr));
    //让套接字ID可以重复绑定同一个地址

    //监听连接
    listen(Listenid, 5);
    //客户端信息
    struct sockaddr_in clientaddr;
    int clientaddrLen = sizeof(clientaddr);

    // select等待连接
    fd_set set1, Saveset,set2,set3,set4;
    FD_ZERO(&set2);//已经登录的套接字集合
    FD_ZERO(&set1);//所有发信息的套接字集合
    FD_ZERO(&set3);//在群聊的套接字集合
    FD_ZERO(&set4);//单聊在线的套接字集合
    FD_ZERO(&Saveset);
    FD_SET(Listenid, &set1);
    Saveset = set1;
    int maxFD = Listenid;
    while (1)
    {

        int ret = select(maxFD + 1, &set1, 0, 0, 0);
        if (ret == -1)
        {
            perror("select");
        }
        else
        {
            for (int i = 3; i < maxFD + 1; i++)
            {
                int val = FD_ISSET(i, &set1);
                if (val == 1)
                {
                    if (i == Listenid) //监听到了有客户端要进行数据连接
                    {
                        int CLientId = accept(i, (struct sockaddr *)&clientaddr, &clientaddrLen);
                        if (CLientId < 0)
                        {
                            perror("11");
                        }
                        maxFD = CLientId;
                        FD_SET(CLientId, &Saveset);
                    }
                    else //有客户端发信息过来
                    {
                        Boat buf;
                        int ret_r = read(i, &buf, sizeof(buf));
                        //正常收到
                        if (0 < ret_r)
                        {
                            //注册
                            if (buf.type == REG)
                            {
                                zc *Pzc = (zc *)buf.data;
                                printf("收到了注册信息\n");
                                Myregister_SER(Pzc, i, clientaddr,&set2);
                            }
                            //登录
                            else if(buf.type == LOGIN)
                            {
                                dl *Pdl = (dl *)buf.data;
                                printf("收到了登录信息%s %s\n",Pdl->name,Pdl->passwd);
                                Mylogin_SER(Pdl,i,maxFD,clientaddr,&set2);
                            }
                             //退出登录
                            else if(buf.type == EXIT_LOGIN)
                            {     
                                printf("收到退出信息\n用户%s退出登录\n",clientname[i]);                         
                                FD_CLR(i,&Saveset);
                            }
                            //找回密码
                            else if(buf.type == PAWD_FIND)
                            {
                                zc *Pfdw = (zc *)buf.data;
                                printf("收到了找回密码信息%s\n",Pfdw->name);
                                MyFindpw_SER(Pfdw,i,clientaddr);
                            }
                            //修改密码
                            else if(buf.type == CHANGE_PSD)
                            {
                                zc *Pfdw = (zc *)buf.data;
                                printf("收到了修改密码信息%s %s %s\n",Pfdw->name,Pfdw->passwd,Pfdw->tel);
                                Change_pw_SER(Pfdw, i);
                            }
                            //进入群聊
                            else if(buf.type == ENTRE_QUNLIAO)
                            {
                                FD_SET(i, &set3);
                                printf("用户%s进入群聊\n",clientname[i]);                                         
                            }
                            //群聊
                            else if(buf.type == QUNLIAO)
                            {
                                FD_SET(i, &set3);
                                printf("收到群聊信息\n");
                                comu * com = (comu *)buf.data;
                                His *Phis = (His *)malloc(sizeof(His));
                                
                                Group_His_Enter_file(com,Phis,i);
                                Ser_Qunliao(com, i, maxFD, &set3);                                           
                            }
                            //添加好友
                            else if(buf.type == ADD_FRIENDS)
                            {    
                                add_fri * Pfr = (add_fri *)buf.data;
                                printf("收到申请添加好友信息,%s %s\n",clientname[i],Pfr->Destname);
                                Friend_Add_SER(Pfr,i);
                            }
                             //申请添加好友信息
                            else if(buf.type == ASKSERVER_FRI)
                            {    
                                printf("%s询问申请添加好友信息\n",clientname[i]);
                                Query_Server_SER(i, &buf);
                            }
                             //处理添加好友信息
                            else if(buf.type == HANDLE)
                            {    
                                add_fri *phandle = (add_fri *)buf.data;
                                printf("%s %s处理添加好友信息\n",clientname[i],phandle->Myname);
                                Handle_Addfriends_SER(i,phandle);
                            }
                            //查看好友列表
                            else if(buf.type == LIST_SHOW)
                            {     
                                printf("收到查看好友列表信息\n");   
                                Friend_List_show_SER(i);                                                         
                            }
                            //进入私聊
                            else if(buf.type == ENTRE_PRICHAT)
                            {
                                FD_SET(i, &set4);
                                printf("用户%s进入单聊\n",clientname[i]);                                         
                            }
                            //私聊
                            else if(buf.type == PRI_CHAT)
                            {     
                                FD_SET(i, &set4);
                                Pri_chat_st *Ppr_st = ( Pri_chat_st *)buf.data;
                                printf("收到私聊信息%s to %s:%s\n",clientname[i],Ppr_st->Destname,Ppr_st->data); 
                                His *Phis = (His *)malloc(sizeof(His));
                                Pri_His_Enter_file(Ppr_st, Phis, i);
                                Pri_chat_SER(i, maxFD, Ppr_st, &set4);                                
                            }
                            //查看群聊记录
                            else if(buf.type == SHOW_GROUP_HISCHAT)
                            {     
                                printf("收到查看群聊记录信息\n");    
                                Show_Group_chat_history_SER(i);                           
                            }
                            //查看私聊记录
                            else if(buf.type == SHOW_PRI_HISCHAT)
                            {     
                                printf("收到查看私聊记录信息\n");    
                                Show_Pri_chat_history_SER(i,buf);                           
                            }
                            //上传文件
                            else if(buf.type == UPLOAD_FILE)
                            {     
                                file_st *pflie = (file_st *)buf.data;
                                printf("收到上传文件信息,文件名%s\n",pflie->file_name); 
                                Uploat_file_SER(i,pflie);                                                     
                            }
                            //转发文件
                            else if(buf.type == TRANSPORD_FILE)
                            {     
                                file_st *pflie = (file_st *)buf.data;
                                printf("收到转发文件信息,文件名%s,转发对象%s\n",pflie->file_name,pflie->username);
                                Transpond_file_SER(i,&buf,maxFD);                                                   
                            }
                            
                        }
                        //客户端退出
                        else
                        {        
                            char arr[128];     
                            sprintf(arr,"有用户%s下线了",clientname[i]);                                
                            printf("%s\n",arr);
                            Boat chuan1;
                            chuan1.type = EXIT_SER;
                            strcpy(chuan1.data,arr);
                            for(int j = 4;j < maxFD+1;j++)
                                {
                                    int val = FD_ISSET(j, &set2);
                                    if(j != i && val == 1)
                                    {       
                                        printf("%s\n",chuan1.data);                                                               
                                        int ret = write(j,&chuan1,sizeof(Boat));                                       
                                    }
                                }                                
                            FD_CLR(i,&Saveset);
                            FD_CLR(i,&set2);  
                            FD_CLR(i,&set3);       
                            FD_CLR(i,&set4);                 
                        }
                    }
                }
            }
            set1 = Saveset;
        }
    }
    close(Listenid);
    return 0;
}