#include"CommenUtil.h"
#include<winsock2.h>
#include<string>
#include"tree.h"
#include<deque>
#include<cassert>  //异常处理
#include"login.h"

#pragma comment(lib,"ws2_32.lib")	//编译指示



//select模式下的多线程
//DWORD WINAPI WorkerThread(LPVOID lpParam);

int main()
{
	//========================================================================
	//下面这部分的功能：
	//	在服务器开启的时候，扫描库函数
	//注：
	//	在服务器停止运行之前有销毁二叉排序树的操作,修改代码时要注意（见最后）
	//========================================================================
	
	//二叉排序树的根结点
	pTree root = NULL;

	//读取库文件并放入sBuffer缓冲区中
	char * file = "FuncSlibs.LIB";
	
	//存储Lib库信息的Vec
	vector<sFuncBlock> vFuncVec;
	
	//将Lib库中的函数信息存储到结构体容器中
	string sLibPath = "FuncSlibs.LIB";
	bool res = LibInfotoVec(sLibPath, vFuncVec);
	assert(res);

	//将容器中的结构体插入到二叉排序树中
	if(VectoTree(vFuncVec, root) == false)
	{
		cout<<"readLib false"<<endl;
		return 1;
	}

	//对二叉排序树进行先序遍历，打印输出函数名
	InOrderTraverse(root);


	//=========================================================================
	//
	//下面这一部分是网络通信部分
	//
	//=========================================================================
	SOCKET ListeningSocket;	//套接字描述符类型，socket()函数成功，则返回一个套接字描述符
	SOCKET NewConnection;
	SOCKADDR_IN serveraddr;
	SOCKADDR_IN clientaddr;
//	char buf[BUFLEN];
	int len;

	WSADATA wsa;
	WSAStartup(MAKEWORD(1,1),&wsa);	//WSA(Windows Sockets Asynchronous，Windows异步套接字)的启动命令
									//MAKEWORD是指定新变量的高低字节
	/*
	socket()用于根据指定的地址族、数据类型和协议来分配一个套接口的描述字及其所用的资源
	第一个参数：指定通信发生的区域（AF_INCF指国际网区域）
	第二个参数：描述要建立的套接字的类型（SOCKET_STREAM是流式套接字）
	第三个参数：该套接字使用的特定协议
	返回值：套接字描述符
	*/
	if((ListeningSocket = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP)) <= 0)
	{
		printf("Create socket fail!\n");
		return -1;
	}

	serveraddr.sin_family = AF_INET;
	serveraddr.sin_port = htons(789);
	serveraddr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);	//INADDR_ANY是指监听所有的地址

	/*
	bind()将一本地地址与一套接口捆绑
	第一个参数：套接字描述符（由socket()函数创建）
	第二个参数：赋给套接字（第一个参数）的本地地址
	第三个参数：第二个参数的长度
	返回值：成功返回0，失败返回SOCKET_ERROR错误号
	*/
	if(bind(ListeningSocket,(SOCKADDR *)&serveraddr, sizeof(serveraddr)) != 0)
	{
		printf("Bind fail!\n");
		return -1;
	}

	printf("Start listen...\n");
	/*
	listen()创建一个套接口并监听申请的连接
	第一个参数：标识一个本地已建立、尚未连接的套接字号，服务器愿意从它上面接收请求
	第二个参数：所允许的请求连接队列的最大长度
	返回值：成功返回0，失败返回SOCKET_ERROR
	*/
	if(listen(ListeningSocket,2) != 0)
	{
		printf("Listen fail!\n");
		return -1;
	}

	len = sizeof(SOCKADDR_IN);

	while(1)
	{
		cout<<"try to accept"<<endl;
		/*
		accept()
		第一个参数：本地套接字描述符
		第二个参数：客户端套接字地址结构的指针
		第三个参数：客户方套接字地址的长度
		*/
		if((NewConnection = accept(ListeningSocket,(SOCKADDR *)&clientaddr, &len)) <= 0)
		{
			printf("Accept fail!\n");
			return -1;
		}

		printf("Connected\n");

		/*
		recv()
		第一个参数：套接字描述符
		第二个参数：接收输入数据缓冲区的指针
		第三个参数：第二个参数的长度
		第四个参数：指定传输控制方式，一般是0
		返回值：成功，返回总共接收的字节数。如果连接被关闭，返回0。其他情况返回SOCKET_ERROR。
		*/
		//接收hello server
		char buf[BUFLEN]={0};
		int buflen = recv(NewConnection, buf, 12, 0);
		if(buflen < 0)
		{
			//接收失败
			return -2;
		}

		string strRecv(buf);

		if (strRecv != "hello server")
		{
			cout<<"strRecv fail"<<endl;
			return -3;
		}
		else
		{
			cout<<strRecv<<endl;
		}

		//发送server ok
		if(send(NewConnection, "server ok", 9, 0) <= 0)
		{
			cout<<"send Error!"<<endl;
		}
		else
		{
			cout<<"send hello world!"<<endl;
		}

		//接收鉴权申请，并进行鉴权
		memset(buf,BUFLEN,0);
		buflen = recv(NewConnection, buf, BUFLEN, 0);
		if(buflen < 0)
		{
			//接收失败
			return -2;
		}

		strRecv = buf;
		cout<<strRecv<<endl;

		loginManager mylogin;
		mylogin.Init();

		if(mylogin.IsUserOK(strRecv) == false)
		{
			if(send(NewConnection, "name or password error!", 23, 0) <= 0)
			{
				cout<<"send Error!"<<endl;
			}
			else
			{
				cout<<"send hello world!"<<endl;
			}
			continue;
		}
		
		//发送server ok
		if(send(NewConnection, "server ok", 9, 0) <= 0)
		{
			cout<<"send Error!"<<endl;
		}
		else
		{
			cout<<"send hello world!"<<endl;
		}

		//接收消息，不断读取协议接收缓冲区
		memset(buf, 0, sizeof(buf));
		strRecv = "";
		int rs = 1;
		while(rs)
		{
			buflen = recv(NewConnection, buf, sizeof(buf), 0);
			strRecv += buf;
			if(buflen < 0)
			{
				// 由于是非阻塞的模式,所以当buflen为EAGAIN时,表示当前缓冲区已无数据可读
				// 在这里就当作是该次事件已处理
				if(buflen == EAGAIN)
					break;
				else
				{
					//接收失败
					return -2;
				}
			}
			else if(buflen == 0)
			{
				// 这里表示对端的socket已正常关闭.
				break;
			}
			if(buflen != sizeof(buf))
				rs = 0;
			else
			{
				memset(buf, 0, sizeof(buf));
				rs = 1;// 需要再次读取
			}
		}


		//消息入队，并等待消息处理





		//发送server ok
		if(send(NewConnection, "server ok", 9, 0) <= 0)
		{
			cout<<"send Error!"<<endl;
		}
		else
		{
			cout<<"send hello world!"<<endl;
		}

		//发送返回结果



		

		


	}

	//释放套接字描述符
	closesocket(ListeningSocket);
	closesocket(NewConnection);

	//关闭异步套接字
	WSACleanup();




	//下面这部分内容不属于网络通信部分

	//销毁扫描库函数后形成的二叉排序树
	DeleteBST(root);

	return 0;
}