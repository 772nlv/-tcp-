#include <myhead.h>
#include "myfunc.h"

//登录状态值
int temp_status = 0;

//修改密码状态值
int Find_status = 0;
//用来存放以登录用户的名字
char clientname[999][128] = {0};
// extern char *clientname[999];qq

sem_t sem1;
sem_t sem2;
int Pri_statu = 0;
//用来记录已登录用户的socket。
Addr clientIdArr[999]; //定义一个结构体数组来接收客户端socket号以及网络地址，用来创建多线程
int clientIndex = 0;   //用来记录数组的下标

//记录申请添加好友的名字
char Requestname[100][128] = {0};
int RequesIndex = 0; //记录个数

//客户机登录注册页面
//显示欢迎界面
void ShowMenu()
{
    system("clear");
    printf("         这是一个聊天室        \n");
    printf("******************************\n");
    printf("*          1.登录             *\n");
    printf("*          2.注册             *\n");
    printf("*        3、找回密码          *\n");
    printf("*          4.退出             *\n");
    printf("******************************\n");
    printf("* 请输入你的选择序号>>>>>   \n");
    fflush(stdout);
}
//登录成功后 客户机显示用户功能列表；
void ShowFunction()
{
    system("clear");
    printf("                              \n");
    printf("          用户功能列表         \n");
    printf("******************************\n");
    printf("*          a.群聊             *\n");
    printf("*          b.好友             *\n");
    printf("*          c.聊天记录         *\n");
    printf("*          d.文件             *\n");
    printf("*          e.退出             *\n");
    printf("*******************************\n");
    printf("* 请输入你的选择序号>>>>>   \n");
    return;
}
//登录页面选择
void ChooseLogin(int sd, struct sockaddr_in addr)
{
    while (1)
    {
        printf("hello\n");
        ShowMenu(); //展示登录页面
        int sel;
        scanf("%d", &sel);
        getchar();
        //登陆区
        switch (sel)
        {
        case 1:
            //登录
            while (1)
            {
                Mylogin(sd, addr);
            }

            break;

        case 2:
            //注册
            while (1)
            {
                Myregister(sd, addr);
            }

            break;
        case 3:
            //找回密码
            while (1)
            {
                if (1 == Find_pawd(sd, addr))
                {
                    break;
                }
            }

            break;
            ;
        case 4:
            //退出
            exit(0);
            break;
        }
    }
}

//功能选择 客户机
void ChooseFunction(int sd, struct sockaddr_in addr)
{

    ShowFunction();
    char choose[10];
    getchar();
    scanf("%s", choose);
    switch (*choose)
    {
    case 'a':
        //群聊
        {
            Qunliao_chat(sd);
        }
        break;
    case 'b':
        // 好友
        {
            Friends_func(sd, addr);
        }
        break;
    case 'c':
        //聊天记录
        {
            Show_chat_history(sd, addr);
        }
        break;

    case 'd':
        //文件
        {
            File_func(sd, addr);
        }
        break;
    case 'e':
        //退出(退出登录)
        {
            // Boat exlog_st = {EXIT_LOGIN, "0"};
            // write(sd, &exlog_st, sizeof(Boat));
            // //返回上一级目录
            ChooseLogin(sd, addr);
        }

        break;
    }
    ChooseFunction(sd, addr);
}
//文件功能选项
void ShowFilefuncChoose()
{
    system("clear");
    printf("          文件功能选项         \n");
    printf("******************************\n");
    printf("*          a.上传文件         *\n");
    printf("*          b.转发文件         *\n");
    printf("*          c.接收文件         *\n");
    printf("*          d.退出             *\n");
    printf("*******************************\n");
    printf("* 请输入你的选择序号>>>>>   \n");
    return;
}
//文件功能选择
int File_func(int sd, struct sockaddr_in addr)
{
    while (1)
    {
        ShowFilefuncChoose();
        char choose[10];
        getchar();
        scanf("%s", choose);
        switch (*choose)
        {
        case 'a':
            //上传文件
            {
                Uploat_file(sd);
            }
            break;

        case 'b':
            //转发文件
            {
                Transpond_file(sd);
            }
            break;
        case 'c':
            //接收文件
            {
                Receive_file(sd);
            }
            break;
        case 'd':
        {
            //退出(返回上一级目录)
            ChooseFunction(sd, addr);
        }
        break;
        default:
            break;
        }
    }
}
//上传文件功能——客户端
int Uploat_file(int sd)
{
    file_st pfile;
    //输入想要传输文件的路径和名字
    printf("你要上传文件的路径:\n");
    char temp_path[64];
    scanf("%s", temp_path);

    printf("请输入你要上传的文件名:\n");
    scanf("%s", pfile.file_name);

    //查看该目录下有没有这个文件
    DIR *dir;
    dir = opendir(temp_path);
    struct dirent *dt;
    if (NULL == dir)
    {
        perror("");
    }
    int val = 0;
    while ((dt = readdir(dir)) != NULL && (0 == errno)) // 没有读到末尾，并且没有出错，就继续读取
    {
        if (strcmp(pfile.file_name, dt->d_name) == 0)
        {
            val++;
            break;
        }
    }
    if (val != 0)
    {
        char pathAndName[320]; //存放文件的路径和名字
        sprintf(pathAndName, "%s%s", temp_path, pfile.file_name);
        Boat buf;
        buf.type = UPLOAD_FILE;
        int fd = open(pathAndName, O_RDWR, 0666);
        if (fd < 0)
        {
            PRINT_ERR("");
        }
        char read_file[512];

        //边读边传
        int ret = 0;
        char Clear[512] = {0};
        while ((ret = read(fd, read_file, sizeof(read_file))) > 0)
        {
            memcpy(pfile.data, read_file, ret);
            memcpy(buf.data, &pfile, sizeof(file_st));
            write(sd, &buf, sizeof(buf));
            memset(pfile.data, 0, sizeof(pfile.data));
        }
        printf("\n上传文件成功\n");
        AnyQuit();
    }
    else if (val == 0)
    {
        printf("该路径下没有该文件\n");
    }
}
//上传文件功能——服务器
int Uploat_file_SER(int sd, file_st *pfile)
{
    char temp_path[320];
    sprintf(temp_path, "./user_upload_file/%s", pfile->file_name);
    int fd = open(temp_path, O_CREAT | O_RDWR | O_APPEND, 0666);
    if (fd < 0)
    {
        PRINT_ERR("");
    }
    write(fd, pfile->data, strlen(pfile->data));
}

//转发文件功能——客户机
int Transpond_file(int sd)
{
    file_st pfile;
    //输入想要传输文件的路径和名字，以及转发文件的对象
    printf("你要转发文件的对象:\n");
    scanf("%s", pfile.username);

    printf("你要转发文件的路径:\n");
    char temp_path[64];
    scanf("%s", temp_path);

    printf("请输入你要转发的文件名:\n");
    scanf("%s", pfile.file_name);

    //查看该目录下有没有这个文件
    DIR *dir;
    dir = opendir(temp_path);
    struct dirent *dt;
    if (NULL == dir)
    {
        perror("");
    }
    int val = 0;
    while ((dt = readdir(dir)) != NULL && (0 == errno)) // 没有读到末尾，并且没有出错，就继续读取
    {
        if (strcmp(pfile.file_name, dt->d_name) == 0)
        {
            val++;
            break;
        }
    }
    if (val != 0)
    {
        char pathAndName[320]; //存放文件的路径和名字
        sprintf(pathAndName, "%s%s", temp_path, pfile.file_name);
        Boat buf;
        buf.type = TRANSPORD_FILE;
        int fd = open(pathAndName, O_RDWR, 0666);
        if (fd < 0)
        {
            PRINT_ERR("");
        }
        char read_file[512];

        //边读边传
        int ret = 0;
        char Clear[512] = {0};
        while ((ret = read(fd, read_file, sizeof(read_file))) > 0)
        {
            memcpy(pfile.data, read_file, ret);
            memcpy(buf.data, &pfile, sizeof(file_st));
            write(sd, &buf, sizeof(buf)); //先把文件发给服务器
            memset(pfile.data, 0, sizeof(pfile.data));
        }
        printf("\n文件转发成功\n");
        AnyQuit();
    }
    else if (val == 0)
    {
        printf("该路径下没有该文件\n");
    }
}

//转发文件功能——服务器
int Transpond_file_SER(int sd, Boat *buf, int maxID)
{
    file_st *pflie = (file_st *)buf->data;
    int Destsd = Name_Find_Id(pflie->username, maxID);
    buf->type = TRANSPORD_FILE_OK;
    write(Destsd, buf, sizeof(Boat));
}

//接收文件——客户机
int Receive_file(int sd)
{
    printf("开始接收文件\n");
    Boat buf;
    int ret = 0;
    while (1)
    {
        ret = read(sd, &buf, sizeof(Boat));
        printf("%d\n", ret);
        file_st *pflie = (file_st *)buf.data;
        char temp_path[320];
        sprintf(temp_path, "./user_upload_file/%s", pflie->file_name);
        int fd = open(temp_path, O_CREAT | O_RDWR | O_APPEND, 0666);
        if (fd < 0)
        {
            PRINT_ERR("");
        }

        write(fd, pflie->data, strlen(pflie->data));
        printf("11111\n");
        close(fd);
        if (strlen(pflie->data) < sizeof(pflie->data))
        {
            break;
        }
    }
    printf("\n文件接收成功\n");
    AnyQuit();
}

//聊天记录选项页面
void ShowHisChatChoose()
{
    system("clear");
    printf("          聊天记录选项         \n");
    printf("******************************\n");
    printf("*          a.群聊记录         *\n");
    printf("*          b.私聊记录         *\n");
    printf("*          c.退出             *\n");
    printf("*******************************\n");
    printf("* 请输入你的选择序号>>>>>   \n");
    return;
}
//查看聊天记录
int Show_chat_history(int sd, struct sockaddr_in addr)
{
    while (1)
    {
        ShowHisChatChoose();
        char choose[10];
        getchar();
        scanf("%s", choose);
        switch (*choose)
        {
        case 'a':
            //群聊
            {
                Show_Group_chat_history(sd);
            }
            break;

        case 'b':
            //单聊
            {
                Show_Pri_chat_history(sd);
            }
            break;
        case 'c':
            ChooseFunction(sd, addr);
            break;
        default:
            break;
        }
    }
}
//查看私聊天记录 客户端
int Show_Pri_chat_history(int sd)
{
    printf("好友列表\n");
    Friend_List_show(sd);
    printf("\n请输入用户名:\n");
    Boat buf;
    scanf("%s", buf.data);
    buf.type = SHOW_PRI_HISCHAT;
    write(sd, &buf, sizeof(buf));
    Aircraft air;
    if (0 >= read(sd, &air, sizeof(Aircraft)))
    {
        printf("服务器异常\n");
        exit(0);
    }
    His *phis = (His *)air.data;
    if (air.type == SHOW_PRI_HISCHAT_OK)
    {
        for (int j = 0; j < air.count; j++)
        {
            printf("%s：%s\n%s\n", (phis + j)->name, (phis + j)->data, (phis + j)->time);
        }
        AnyQuit();
    }
    if (air.type == NO)
    {
        printf("该用户不是你的好友\n");
        AnyQuit();
    }
}
//查看私聊记录 服务器
int Show_Pri_chat_history_SER(int sd, Boat buf)
{
    char tempFr[128];
    sprintf(tempFr, "./friend_info/%s.txt", clientname[sd]);
    char tempname[128];
    strcpy(tempname, buf.data);
    int fd1 = open(tempFr, O_CREAT | O_RDWR | O_APPEND, 0666);
    if (fd1 < 0)
    {
        PRINT_ERR("");
    }
    lseek(fd1, 0, SEEK_SET);
    int statu = 0;
    //遍历一次，看好友列表里面有没有该用户
    char file_temp[128];
    while (0 < read(fd1, file_temp, sizeof(file_temp)))
    {
        if (strcmp(file_temp, tempname) == 0)
        {
            //有这个好友
            statu++;
            char HisFilename[32];
            sprintf(HisFilename, "./Chat_History/%s_His/%s.dat", clientname[sd], tempname);
            Aircraft air;
            air.type = SHOW_PRI_HISCHAT_OK;
            int fd = open(HisFilename, O_RDWR, 0666);
            if (fd < 0)
            {
                PRINT_ERR(" ");
            }

            His file_his;
            His tmep_hisarr[80];
            int i = 0;
            while (read(fd, &file_his, sizeof(His)) > 0)
            {
                memcpy(&tmep_hisarr[i], &file_his, sizeof(His));
                i++;
            }
            memcpy(air.data, tmep_hisarr, sizeof(tmep_hisarr));
            air.count = i;

            write(sd, &air, sizeof(air));
        }
    }
    if (statu == 0)
    {
        // fflush(fp);
        Boat air = {NO, "该用户不是你的好友"};
        write(sd, &air, sizeof(air));
    }
    close(fd1);
}
//将私聊记录写进文档
int Pri_His_Enter_file(void *p, His *Phis, int sd)
{
    Pri_chat_st *Ppr_st = (Pri_chat_st *)p;
    strcpy(Ppr_st->Myname, clientname[sd]); //把消息来源用户名保存
    strcpy(Phis->name, Ppr_st->Myname);     //写入聊天记录结构体
    strcpy(Phis->data, Ppr_st->data);
    strcpy(Phis->time, Gettime());
    printf("%s %s %s\n", Phis->name, Phis->data, Phis->time);
    char HisFilename[32];
    char HisFilename_1[32];
    sprintf(HisFilename, "./Chat_History/%s_His/%s.dat", Ppr_st->Myname, Ppr_st->Destname);
    sprintf(HisFilename_1, "./Chat_History/%s_His/%s.dat", Ppr_st->Destname, Ppr_st->Myname);
    int fd = open(HisFilename, O_CREAT | O_RDWR | O_APPEND, 0666);
    if (fd < 0)
    {
        PRINT_ERR(" ");
    }
    int fd1 = open(HisFilename_1, O_CREAT | O_RDWR | O_APPEND, 0666);
    if (fd1 < 0)
    {
        PRINT_ERR(" ");
    }
    write(fd, Phis, sizeof(His));
    write(fd1, Phis, sizeof(His));
    close(fd);
    close(fd1);
    free(Phis);
}
//将群聊记录写进文档
int Group_His_Enter_file(void *p, His *Phis, int sd)
{
    comu *com = (comu *)p;
    strcpy(com->name, clientname[sd]); //把消息来源用户名保存
    strcpy(Phis->name, com->name);     //写入聊天记录结构体
    strcpy(Phis->data, com->data);
    strcpy(Phis->time, Gettime());
    printf("%s %s %s\n", Phis->name, Phis->data, Phis->time);
    char HisFilename[32];
    sprintf(HisFilename, "./Chat_History/Group_His.dat");
    int fd = open(HisFilename, O_CREAT | O_RDWR | O_APPEND, 0666);
    if (fd < 0)
    {
        PRINT_ERR(" ");
    }
    write(fd, Phis, sizeof(His));
    close(fd);
    free(Phis);
}
//查看群聊天记录 客户端
int Show_Group_chat_history(int sd)
{
    Boat buf;
    buf.type = SHOW_GROUP_HISCHAT;
    write(sd, &buf, sizeof(buf));
    Aircraft air;
    if (0 >= read(sd, &air, sizeof(Aircraft)))
    {
        printf("服务器异常\n");
        exit(0);
    }
    His *phis = (His *)air.data;
    if (air.type == SHOW_GROUP_HISCHAT_OK)
    {
        for (int j = 0; j < air.count; j++)
        {
            printf("%s：%s\n%s\n", (phis + j)->name, (phis + j)->data, (phis + j)->time);
        }
        AnyQuit();
    }
}
//查看群聊天记录 服务器
int Show_Group_chat_history_SER(int sd)
{
    Aircraft air;
    air.type = SHOW_GROUP_HISCHAT_OK;
    char HisFilename[32];
    sprintf(HisFilename, "./Chat_History/Group_His.dat");
    int fd = open(HisFilename, O_RDWR, 0666);
    if (fd < 0)
    {
        PRINT_ERR(" ");
    }

    His file_his;
    His tmep_hisarr[80];
    int i = 0;
    while (read(fd, &file_his, sizeof(His)) > 0)
    {
        memcpy(&tmep_hisarr[i], &file_his, sizeof(His));

        i++;
    }
    memcpy(air.data, tmep_hisarr, sizeof(tmep_hisarr));
    air.count = i;

    write(sd, &air, sizeof(air));
}
//群聊功能
void Qunliao_chat(int sd)
{
    Boat temp_Enter;
    temp_Enter.type = ENTRE_QUNLIAO;
    write(sd, &temp_Enter, sizeof(Boat));
    printf("进入群聊,输入quit即可脱出\n");
    pthread_t ptid;
    pthread_create(&ptid, NULL, cli_qunliao_work, &sd);
    while (1)
    {
        comu com;
        scanf("%s", com.data);
        char *time = Gettime();
        printf("%s\n", time);
        Boat qunl;
        qunl.type = QUNLIAO;
        // getchar();
        //创建一个群聊线程,接收信息，退出群聊时关闭
        if (strcmp(com.data, "quit") == 0)
        {
            memcpy(qunl.data, &com, sizeof(com));
            write(sd, &qunl, sizeof(qunl));
            pthread_cancel(ptid);
            break;
        }
        memcpy(qunl.data, &com, sizeof(com));
        write(sd, &qunl, sizeof(qunl));
    }
}
//服务器转发群聊信息
int Ser_Qunliao(comu *com, int sd, int maxFD, fd_set *set)
{
    Boat buf;
    strcpy(com->name, clientname[sd]);
    buf.type = QUNLIAO;
    if (strcmp(com->data, "quit") == 0)
    {
        strcpy(com->data, "quit");
        buf.type = EXIT_QUNL;
        FD_CLR(sd, set);
    }
    memcpy(buf.data, com, sizeof(comu));
    for (int j = 4; j < maxFD + 1; j++)
    {
        int val = FD_ISSET(j, set);
        if (j != sd && val == 1)
        {
            int ret = write(j, &buf, sizeof(Boat));
        }
    }
}
//客户端群聊子线程工作函数  客户机
void *cli_qunliao_work(void *p)
{
    int sd = *(int *)p;
    while (1)
    {
        Boat chuan;
        int ret = read(sd, &chuan, sizeof(chuan));
        if (ret > 0)
        {
            if (chuan.type == QUNLIAO)
            {
                comu *c = (comu *)chuan.data;
                printf("%s：%s\n", c->name, c->data);
                printf("%s\n", Gettime());
            }
            if (chuan.type == EXIT_QUNL)
            {
                comu *c = (comu *)chuan.data;
                printf("%s 退出群聊\n", c->name);
                printf("%s\n", Gettime());
            }
            else if (chuan.type == EXIT_SER)
            {
                printf("%s\n", chuan.data);
            }
        }
        if (ret <= 0)
        {
            printf("服务器异常，程序退出\n");
            exit(0);
        }
    }
}

//好友功能选项页面
void ShowFriendsfunc()
{
    system("clear");
    printf("                              \n");
    printf("          好友功能列表         \n");
    printf("******************************\n");
    printf("*          a.好友列表         *\n");
    printf("*          b.添加好友         *\n");
    printf("*          c.私聊好友         *\n");
    printf("*          d.新朋友(%d)       *\n", RequesIndex);
    printf("*          e.退出             *\n");
    printf("*******************************\n");
    printf("* 请输入你的选择序号>>>>>   \n");
    return;
}

//好友功能 客户机
int Friends_func(int sd, struct sockaddr_in addr)
{
    Query_Server(sd);
    ShowFriendsfunc(); //展示好友功能页面
    char choose[10];
    scanf("%s", choose);
    switch (*choose)
    {
    case 'a':
    {
        //查看好友列表
        printf("好友列表\n");
        Friend_List_show(sd);
        AnyQuit();
        break;
    }

    case 'b':
    {
        //添加好友
        Friend_Add(addr, sd);
        break;
    }

    case 'c':
        //私聊好友
        Pri_chat(sd);
        break;

    case 'd':
        //私聊好友
        Handle_Addfriends(sd);
        break;
    case 'e':
        //退出（返回上一级）
        ChooseFunction(sd, addr);
        break;
    }
    Friends_func(sd, addr);
}
//私聊
void Pri_chat(int sd)
{
    Boat temp_Enter;
    temp_Enter.type = ENTRE_PRICHAT;
    write(sd, &temp_Enter, sizeof(Boat));
    int ret1 = sem_init(&sem1, 0, 1);
    int ret2 = sem_init(&sem2, 0, 0);
    printf("quit即可退出\n好友列表\n");
    Friend_List_show(sd);
    pthread_t ptid;
    pthread_create(&ptid, NULL, Pri_work, &sd);
    Boat buf;
    Pri_chat_st pri_st;
    printf("请输入你要私聊的对象：\n");
    scanf("%s", pri_st.Destname);

    while (1)
    {

        scanf("%s", pri_st.data);
        if (Pri_statu == 1)
        {
            pthread_cancel(ptid);
            Pri_statu = 0;
            Pri_chat(sd);
            break;
        }
        printf("%s\n", Gettime());
        memcpy(buf.data, &pri_st, sizeof(pri_st));
        buf.type = PRI_CHAT;
        if (strcmp(pri_st.data, "quit") == 0)
        {
            write(sd, &buf, sizeof(Boat));
            pthread_cancel(ptid);
            break;
        }
        write(sd, &buf, sizeof(buf));
    }
}
//私聊——客户机收信息
void *Pri_work(void *p)
{
    int sd = *(int *)p;
    while (1)
    {
        Boat chuan;
        int ret = read(sd, &chuan, sizeof(Boat));
        if (ret > 0)
        {

            if (chuan.type == OK)
            {
                Pri_chat_st *c = (Pri_chat_st *)chuan.data;
                printf("%s：%s\n", c->Myname, c->data);
                printf("%s\n", Gettime());
            }
            if (chuan.type == EXIT_PRI)
            {
                Pri_chat_st *c = (Pri_chat_st *)chuan.data;
                printf("%s 离开了\n", c->Myname);
            }
            if (chuan.type == FRE_ERROR)
            {
                printf("该用户不是你的好友\n输入任意键继续\n");
                Pri_statu = 1;
            }
        }
        if (ret <= 0)
        {
            printf("服务器异常，程序退出\n");
            exit(0);
        }
    }
}
//私聊 服务器
int Pri_chat_SER(int sd, int maxFD, Pri_chat_st *Ppr_st, fd_set *set)
{
    Boat buf;
    strcpy(Ppr_st->Myname, clientname[sd]);
    buf.type = OK;
    if (strcmp(Ppr_st->data, "quit") == 0)
    {
        strcpy(Ppr_st->data, "quit");
        FD_CLR(sd, set);
        buf.type = EXIT_PRI;
    }
    memcpy(buf.data, Ppr_st, sizeof(Pri_chat_st));
    //遍历好友列表 看是不是有这个好友
    char tempFr[128];
    sprintf(tempFr, "./friend_info/%s.txt", Ppr_st->Myname);
    int fd1 = open(tempFr, O_CREAT | O_RDWR, 0666);
    if (fd1 < 0)
    {
        PRINT_ERR("");
    }
    lseek(fd1, 0, SEEK_SET);
    int statu = 0;
    //遍历一次，看好友列表里面有没有该用户
    char file_temp[128];
    while (0 < read(fd1, file_temp, sizeof(file_temp)))
    {
        if (strcmp(file_temp, Ppr_st->Destname) == 0)
        {
            statu++;
            int tempFD = Name_Find_Id(Ppr_st->Destname, maxFD);
            int val = FD_ISSET(tempFD, set);
            if (val == 1)
            {
                write(tempFD, &buf, sizeof(Boat));
            }
            return 0;
        }
    }
    if (statu == 0)
    {
        buf.type = FRE_ERROR;
        write(sd, &buf, sizeof(Boat));
    }
}
//查看好友列表
void Friend_List_show(int sd)
{
    Boat buf;
    buf.type = LIST_SHOW;
    write(sd, &buf, sizeof(buf));

    Aircraft air;
    if (0 >= read(sd, &air, sizeof(Aircraft)))
    {
        printf("服务器异常\n");
        exit(0);
    }
    if (air.type == LIST_SHOW_OK)
    {
        for (int j = 0; j < air.count; j++)
        {
            printf("%s\n", air.data + j * 128);
        }
    }
}

//查看好友列表 服务器
void Friend_List_show_SER(int sd)
{
    Aircraft air;
    air.type = LIST_SHOW_OK;

    char temp_finame[128];
    sprintf(temp_finame, "./friend_info/%s.txt", clientname[sd]);
    char Friend_name[128 * 80];
    int fd = open(temp_finame, O_RDWR, 0666);

    int i = 0;
    while (read(fd, temp_finame, sizeof(temp_finame)) > 0)
    {
        memcpy(Friend_name + i * 128, temp_finame, sizeof(temp_finame));
        i++;
    }
    memcpy(air.data, Friend_name, sizeof(Friend_name));
    air.count = i;
    write(sd, &air, sizeof(air));
}

//申请添加好友 客户机
int Friend_Add(struct sockaddr_in addr, int sd)
{
    add_fri fri_st;
    printf("请输入你要加的好友的用户名：\n");
    scanf("%s", fri_st.Destname);
    if (strcmp(fri_st.Destname, "quit") == 0)
    {
        return 0;
    }
    Boat buf;
    buf.type = ADD_FRIENDS;
    memcpy(buf.data, &fri_st, sizeof(frd));
    write(sd, &buf, sizeof(Boat));

    //收信息
    Boat rec_buf;
    if (0 >= read(sd, &rec_buf, sizeof(buf)))
    {
        printf("服务器异常\n");
        exit(0);
    }
    if (rec_buf.type == FRE_ERROR)
    {
        printf("该用户不存在,请重新输入\n");
        Friend_Add(addr, sd);
    }
    else if (rec_buf.type == NO)
    {
        printf("该用户已经是你的好友了\n");
        AnyQuit();
    }
    else if (rec_buf.type == OK)
    {
        printf("已发送好友申请\n");
        AnyQuit();
    }
}

//申请添加好友 服务器
int Friend_Add_SER(add_fri *Pfr, int sd)
{
    //遍历user.txt 看有没有这个用户
    int fd = open("./user.txt", O_CREAT | O_RDWR | O_APPEND, 0666);
    if (fd < 0)
    {
        PRINT_ERR(" ");
    }
    int count = 0;
    lseek(fd, 0, SEEK_SET);
    zc st_fr;
    while (0 < read(fd, &st_fr, sizeof(zc)))
    {
        if (strcmp(st_fr.name, Pfr->Destname) == 0)
        {
            count++;
        }
    }
    if (count == 0)
    {
        Boat buf = {FRE_ERROR, "该用户不存在\n"};
        write(sd, &buf, sizeof(Boat));
        close(fd);
        return -1;
    }
    close(fd);

    char tempFr[128];
    sprintf(tempFr, "./friend_info/%s.txt", clientname[sd]);

    int fd1 = open(tempFr, O_CREAT | O_RDWR | O_APPEND, 0666);
    if (fd1 < 0)
    {
        PRINT_ERR("");
    }
    lseek(fd1, 0, SEEK_SET);
    int statu = 0;
    //遍历一次，看好友列表里面有没有该用户
    char file_temp[128];
    while (0 < read(fd1, file_temp, sizeof(file_temp)))
    {
        if (strcmp(file_temp, Pfr->Destname) == 0)
        {
            statu++;
            char temparr[512];
            sprintf(temparr, "\n用户%s已经是你好友了\n", Pfr->Destname);
            Boat buf;
            buf.type = NO;
            memcpy(buf.data, temparr, sizeof(temparr));
            write(sd, &buf, sizeof(Boat));
            return 0;
        }
    }
    if (statu == 0)
    {
        // fflush(fp);
        Boat buf = {OK, "\n申请成功\n"};
        int fd2 = open("./Friend_request", O_CREAT | O_RDWR | O_APPEND, 0666);
        if (fd2 < 0)
        {
            PRINT_ERR("");
        }
        memcpy(Pfr->Myname, clientname[sd], sizeof(Pfr->Destname));
        Pfr->type = UNHANDLE;
        if (write(fd2, Pfr, sizeof(add_fri)) <= 0)
        {
            perror("");
        }
        write(sd, &buf, sizeof(buf));
        close(fd2);
    }
    close(fd);
    close(fd1);
}
//询问服务器有没有好友申请
void Query_Server(int sd)
{
    Boat buf;
    buf.type = ASKSERVER_FRI;
    write(sd, &buf, sizeof(Boat));

    while (1)
    {
        int ret = read(sd, &buf, sizeof(Boat));
        printf("%d %d\n", ret,buf.type);
        if (buf.type == OVER)
        {
            break;
        }
        add_fri *pfri = (add_fri *)buf.data;
        printf("%s\n", pfri->Myname);
        strcpy(Requestname[RequesIndex], pfri->Myname);
        printf("%s\n", Requestname[RequesIndex]);
        RequesIndex++;
    }
}

//查看有没有好友申请———服务器
int Query_Server_SER(int sd, Boat *buf)
{
    Boat temp_buf;
    temp_buf.type = ASKSERVER_FRI;
    add_fri *pfri;
    int fd = open("./Friend_request", O_CREAT | O_RDWR | O_APPEND, 0666);
    if (fd < 0)
    {
        PRINT_ERR("");
    }
    strcpy(pfri->Destname, clientname[sd]);
    add_fri temp_st;
    while (read(fd, &temp_st, sizeof(add_fri)) > 0)
    {
        if (strcmp(pfri->Destname, temp_st.Destname) == 0 && temp_st.type == UNHANDLE)
        {
            printf("自己的用户名%s 加好友对象的用户名%s %d\n", temp_st.Myname, temp_st.Destname, temp_st.type);
            memcpy(temp_buf.data, &temp_st, sizeof(add_fri));
           int ret = write(sd, &temp_buf, sizeof(Boat));
           printf("%d,%d\n",ret,temp_buf.type);
        }
    }
    temp_buf.type = OVER;
    write(sd, &temp_buf, sizeof(Boat));
    close(fd);
}

//处理新朋友信息
int Handle_Addfriends(int sd)
{
    for (int i = 0; i < RequesIndex; i++)
    {
        printf("%s请求添加你为好友，是否同意？(y/n)\ny.同意  n.拒绝\n", Requestname[i]);
        char choose[10];
        scanf("%s", choose);

        add_fri handlie_st;
        strcpy(handlie_st.Myname, Requestname[i]);
        if (*choose == 'y')
        {
            handlie_st.type = AGREE;
        }
        else if (*choose == 'n')
        {
            handlie_st.type = OBJECT;
        }

        Boat buf;
        buf.type = HANDLE;
        memcpy(buf.data, &handlie_st, sizeof(add_fri));
        int ret = write(sd, &buf, sizeof(Boat));
        if (ret <= 0)
        {
            perror("");
        }
    }
    RequesIndex = 0;
    AnyQuit();
}

//处理新朋友信息————服务器
int Handle_Addfriends_SER(int sd, add_fri *phandle)
{
    strcpy(phandle->Destname, clientname[sd]);
    printf("%d %s %s\n", phandle->type, phandle->Myname, phandle->Destname);
    int fd2 = open("./Friend_request", O_RDWR, 0666);
    if (fd2 < 0)
    {
        PRINT_ERR("");
    }
    add_fri tempadd_st;
    while (read(fd2,&tempadd_st,sizeof(add_fri)) > 0)
    {
        if(strcmp(phandle->Myname,tempadd_st.Myname) == 0 && strcmp(phandle->Destname,tempadd_st.Destname) == 0)
        {
            lseek(fd2,-sizeof(add_fri),SEEK_CUR);
            write(fd2,phandle,sizeof(add_fri));
        }
    }
    
    char temp_path[128];
    char temp_path1[128];
    sprintf(temp_path, "./friend_info/%s.txt", phandle->Myname);
    sprintf(temp_path1, "./friend_info/%s.txt", phandle->Destname);

    int fd = open(temp_path, O_RDWR | O_APPEND, 0666);
    if (fd < 0)
    {
        PRINT_ERR("");
    }
    int fd1 = open(temp_path1,O_CREAT | O_RDWR | O_APPEND, 0666);
    if (fd1 < 0)
    {
        PRINT_ERR("");
    }
    if(phandle->type == AGREE)
    {
        write(fd,phandle->Destname,sizeof(phandle->Destname));
        write(fd1,phandle->Myname,sizeof(phandle->Myname));
    }
    close(fd);
    close(fd1);
    close(fd2);
}
//注册 客户机
void Myregister(int sd, struct sockaddr_in addr)
{

    zc info;
    printf("请输入用户名: ");
    scanf("%s", info.name);
    getchar();

    printf("请输入密码: ");
    scanf("%s", info.passwd);
    getchar();

    printf("请输入电话号码: ");
    scanf("%s", info.tel);
    getchar();

    Boat buf;
    memcpy(buf.data, &info, sizeof(info));
    buf.type = REG;
    write(sd, &buf, sizeof(buf));
    RecloginAndReg(addr, sd);
    if (temp_status == 1)
    {
        AnyQuit();
        temp_status = 0;
        strcpy(clientname[sd], info.name);
        ChooseFunction(sd, addr);
    }
}

//登录  客户机
void Mylogin(int sd, struct sockaddr_in addr)
{

    dl info;
    printf("请输入用户名:");
    scanf("%s", info.name);
    getchar();

    printf("请输入密码:");
    scanf("%s", info.passwd);
    getchar();

    Boat buf;
    memcpy(buf.data, &info, sizeof(info));
    buf.type = LOGIN;
    int ret = write(sd, &buf, sizeof(buf));
    RecloginAndReg(addr, sd);
    if (temp_status == 1)
    {
        AnyQuit();
        temp_status = 0;
        strcpy(clientname[sd], info.name);
        ChooseFunction(sd, addr);
    }
}

//服务器注册验证
int Myregister_SER(zc *Pzc, int sd, struct sockaddr_in addr, fd_set *set)
{
    int fd = open("./user.txt", O_CREAT | O_RDWR | O_APPEND, 0666);
    if (fd < 0)
    {
        PRINT_ERR(" ");
    }
    int count = 0;
    lseek(fd, 0, SEEK_SET);
    zc st_zc;
    while (0 < read(fd, &st_zc, sizeof(zc)))
    {
        if (strcmp(st_zc.name, Pzc->name) == 0)
        {
            Boat buf = {NO, "\n用户已存在，请重新注册\n注册失败,请重新注册\n"};
            write(sd, &buf, sizeof(buf));
            count++;
        }
    }

    if (count == 0)
    {
        write(fd, Pzc, sizeof(zc));
        // fflush(fp);
        Boat buf = {OK, "\n注册成功，并登录"};
        write(sd, &buf, sizeof(buf));
        FD_SET(sd, set);
        char arr[128];

        //将用户名字保存为数组下标为对应sd的元素
        strcpy(clientname[sd], Pzc->name);

        //创建一个对应的好友列表
        char command[128];
        sprintf(command, "touch ./friend_info/%s.txt", Pzc->name);
        system(command);

        //创建对应的聊天记录目录
        char command2[128];
        sprintf(command2, "mkdir ./Chat_History/%s_His", Pzc->name);
        system(command2);

        //转换地址并打印
        inet_ntop(AF_INET, &addr.sin_addr.s_addr, arr, sizeof(arr));
        printf("网络地址为%s:%d的用户登录了服务器\n", arr, ntohs(addr.sin_port));
    }

    close(fd);
}

//服务器登录验证
int Mylogin_SER(dl *pdl, int sd, int maxFD, struct sockaddr_in addr, fd_set *set)
{
    // char tempstr[64];
    // sprintf(tempstr, "%s:%s", pdl->name, pdl->passwd);
    // char Filestr[64];

    dl st_sl;
    int fd = open("./user.txt", O_RDONLY, 0666);
    if (fd < 0)
    {
        PRINT_ERR(" ");
    }
    lseek(fd, 0, SEEK_SET);
    int count = 0;
    // if (sd < maxFD + 1)
    // {
    //     printf("%d\n", Name_Find_Id(pdl->name,maxFD));
    //     if (FD_ISSET(Name_Find_Id(pdl->name,maxFD), set) == 1)
    //     {
    //         Boat buf = {ON_LINE, "该用户已经在其他地方登录了\n"};
    //         write(sd, &buf, sizeof(buf));
    //     }
    //     printf("我在这\n");
    //     return 0;
    // }

    while (0 < read(fd, &st_sl, sizeof(dl)))
    {
        if ((strcmp(st_sl.name, pdl->name) == 0) && (strcmp(st_sl.passwd, pdl->passwd) == 0))
        {
            Boat buf = {OK, "登陆成功"};
            int ret = write(sd, &buf, sizeof(buf));
            FD_SET(sd, set);
            char arr[128];
            inet_ntop(AF_INET, &addr.sin_addr.s_addr, arr, sizeof(arr));
            strcpy(clientname[sd], pdl->name);
            printf("网络地址为%s:%d的用户%s登录了服务器\n", arr, ntohs(addr.sin_port), clientname[sd]);
            count++;
        }
    }
    if (count == 0)
    {
        Boat buf = {NO, "登录失败,请重新登陆\n"};
        write(sd, &buf, sizeof(buf));
    }
    close(fd);
}

//接收来自服务器的回应信息 客户机
int RecloginAndReg(struct sockaddr_in addr, int sd)
{
    Boat buf;
    // int len = sizeof(addr);
    if (0 >= read(sd, &buf, sizeof(buf)))
    {
        printf("服务器异常\n");
        exit(0);
    }
    if (buf.type == OK)
    {
        printf("%s\n", buf.data);
        temp_status = 1;
    }
    if (buf.type == ON_LINE)
    {
        printf("该用户已经在其他地方登录了\n");
        printf("%s\n", buf.data);
    }
    else if (buf.type == NO)
    {
        printf("%s\n", buf.data);
    }
}

//找回密码
int Find_pawd(int sd, struct sockaddr_in addr)
{
    Boat buf;
    zc fpw;

    printf("请输入用户名\n");
    scanf("%s", fpw.name);
    getchar();

    printf("请输入电话号码\n");
    scanf("%s", fpw.tel);
    getchar();

    buf.type = PAWD_FIND;
    memcpy(buf.data, &fpw, sizeof(fpw));
    write(sd, &buf, sizeof(buf));
    RecloginAndReg(addr, sd);
    if (temp_status == 1)
    {
        temp_status = 0;
        if (1 == Change_pw(&fpw, sd, addr))
        {
            return 1;
        }
        //修改密码
    }
}
//修改密码
int Change_pw(zc *fpw, int sd, struct sockaddr_in addr)
{
    printf("是否要修改密码(用户信息) (y/n)\n");
    char val;
    scanf("%c", &val);
    if (val == 'y')
    {
        Boat buf;
        buf.type = CHANGE_PSD;
        printf("请输入新密码:\n");
        scanf("%s", fpw->passwd);
        memcpy(buf.data, fpw, sizeof(zc));
        write(sd, &buf, sizeof(Boat));
        Boat rec_buf;
        read(sd, &rec_buf, sizeof(Boat));
        if (rec_buf.type == CHANGE_PSD_OK)
        {
            printf("修改密码成功\n");
            AnyQuit();
            return 1;
        }
    }
    else if (val == 'n')
    {
        printf("不改算了\n");
        AnyQuit();
    }
    ChooseLogin(sd, addr);
}
//修改密码 服务器
int Change_pw_SER(zc *fpw, int sd)
{
    zc fw_sl;
    int fd = open("./user.txt", O_RDWR, 0666);
    if (fd < 0)
    {
        PRINT_ERR(" ");
    }
    while (0 < read(fd, &fw_sl, sizeof(zc)))
    {
        if ((strcmp(fw_sl.name, fpw->name) == 0) && (strcmp(fw_sl.tel, fpw->tel) == 0))
        {
            lseek(fd, -sizeof(zc), SEEK_CUR);
            write(fd, fpw, sizeof(zc));
            printf("密码修改成功\n");
            Boat buf;
            buf.type = CHANGE_PSD_OK;
            write(sd, &buf, sizeof(buf));
            break;
        }
    }
}
//找回密码 服务器
int MyFindpw_SER(zc *Pdfw, int sd, struct sockaddr_in addr)
{
    zc fw_sl;
    int fd = open("./user.txt", O_CREAT | O_RDWR | O_APPEND, 0666);
    if (fd < 0)
    {
        PRINT_ERR(" ");
    }
    lseek(fd, 0, SEEK_SET);
    int count = 0;
    while (0 < read(fd, &fw_sl, sizeof(zc)))
    {
        if ((strcmp(fw_sl.name, Pdfw->name) == 0) && (strcmp(fw_sl.tel, Pdfw->tel) == 0))
        {
            char temp[128];
            sprintf(temp, "\n找回密码了\n 密码是%s\n", fw_sl.passwd);
            Boat buf;
            buf.type = OK;
            strcpy(buf.data, temp);
            write(sd, &buf, sizeof(buf));
            count++;
        }
    }
    if (count == 0)
    {
        Boat buf = {NO, "\n用户名或者电话号码出错了，请重新操作\n"};
        write(sd, &buf, sizeof(buf));
    }
    close(fd);
}

//通过名字找用户的套接字
int Name_Find_Id(char *tname, int maxFD)
{
    int sid = 0;
    while (sid < maxFD + 1)
    {
        if (strcmp(tname, clientname[sid]) == 0)
        {
            return sid;
        }
        sid++;
    }
    return 0;
}
//获取当前时间
char *Gettime()
{
    time_t tm;
    struct tm *tim;
    tm = time(NULL);
    tim = localtime(&tm);
    char *time = (char *)malloc(sizeof(time));
    sprintf(time, "%d-%02d-%02d %02d:%02d:%02d\n", tim->tm_year + 1900, tim->tm_mon + 1, tim->tm_mday, tim->tm_hour, tim->tm_min, tim->tm_sec);
    return time;
}
//按任意键退出
void AnyQuit()
{
    printf("按任意键继续\n");
    char buf[10];
    scanf("%s", buf);
}
