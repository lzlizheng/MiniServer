#include<stdio.h>
#include<winsock2.h>
#include<iostream>
#include<string.h>
#include"tree.h"
#include"qRequest.h"

using namespace std;

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
	if(LibInfotoVec(sLibPath, vFuncVec) == false)
	{
		cout<<"readLib false"<<endl;
		return 1;
	}

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
	SOCKET serversoc;	//套接字描述符类型，socket()函数成功，则返回一个套接字描述符
	SOCKET clientsoc;
	SOCKADDR_IN serveraddr;
	SOCKADDR_IN clientaddr;
//	char buf[BUFLEN];
	int len;
	DWORD dwThreadId;	//线程ID

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
	if((serversoc = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP)) <= 0)
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
	if(bind(serversoc,(SOCKADDR *)&serveraddr, sizeof(serveraddr)) != 0)
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
	if(listen(serversoc,2) != 0)
	{
		printf("Listen fail!\n");
		return -1;
	}

	len = sizeof(SOCKADDR_IN);


	//多线程请求队列核心操作对象
	CReqRes myReqRes(serversoc);

	CreateThread(NULL, 0, myReqRes.WorkerThread, NULL, 0, &dwThreadId);

	while(1)
	{
		cout<<"try to accept"<<endl;
		/*
		accept()
		第一个参数：本地套接字描述符
		第二个参数：客户端套接字地址结构的指针
		第三个参数：客户方套接字地址的长度
		*/
		if((clientsoc = accept(serversoc,(SOCKADDR *)&clientaddr, &len)) <= 0)
		{
			printf("Accept fail!\n");
			return -1;
		}

		// Add socket to g_CliSocketArr
	    myReqRes.g_CliSocketArr[myReqRes.g_iTotalConn++] = clientsoc;

		printf("Connected\n");
	}

	//释放套接字描述符
	closesocket(serversoc);

	//关闭异步套接字
	WSACleanup();

 


	//下面这部分内容不属于网络通信部分

	//销毁扫描库函数后形成的二叉排序树
	DeleteBST(root);

	return 0;
}