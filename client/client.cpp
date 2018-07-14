#include "client.h"
#include<fstream>
#include<cassert>  //异常处理

int ReadSendingData(string& sendbuf)
{
	ifstream infile;
	string file("callfunc.txt");

    infile.open(file.data());   //将文件流对象与文件连接起来 
	bool is_open = infile.is_open();
    assert(is_open);   //若失败,则输出错误消息,并终止程序运行

	infile>>sendbuf;
//	cout<<sendbuf<<endl;

	infile.close();

	return 0;
}


int SendRequest(const string& sendbuf, string& resbuf)
{
	SOCKET soc;
	SOCKADDR_IN serveraddr;

	int conerror;

	WSADATA wsa;
	WSAStartup(MAKEWORD(1,1),&wsa);	//同server

	if((soc = socket(AF_INET,SOCK_STREAM, IPPROTO_TCP)) <= 0)	//同server
	{
		printf("Create socket fail!\n");
	//	return -1;
	}

	//设置服务器地址、端口号等
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_port = htons(789);
	serveraddr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");	//指本机地址


	//连接
	cout<<"Try to connect..."<<endl;
	
	/*
	connect()用于建立与指定socket的连接
	第一个参数：标识一个未连接socket
	第二个参数：指向要连接套接字的sockaddr结构体的指针
	第三个参数：sockaddr结构体的字节长度
	返回值：若无错误发生，则connect()返回0。否则的话，返回SOCKET_ERROR错误
	*/

	if((conerror=connect(soc,(SOCKADDR *)&serveraddr,sizeof(serveraddr))) != 0)
	{			
		cout<<"Connect fail:"<<endl;
		cout<<WSAGetLastError()<<endl;
		//连接失败
		return -1;
	}

	cout<<"Connected!"<<endl;



	//发送消息
	//发送hello server!,测试服务器是否正常工作
	if(send(soc, "hello server", 12, 0) <= 0)
	{
		cout<<"send Error!"<<endl;
	}
	else
	{
		cout<<"send hello world!"<<endl;
	}

	//确认服务器回复的消息
	char buf[64] = {0};
	int buflen = recv(soc, buf, 9, 0);
	string strRecvServerOK(buf);

	if(buflen < 0)
	{
		//接收失败
		return -2;
	}

	if (strRecvServerOK != "server ok")
	{
		cout<<"strRecvServerOK fail"<<endl;
		return -3;
	}
	else
	{
		cout<<strRecvServerOK<<endl;
	}

	//客户端发送用户名和密码，向服务器申请鉴权
	string name_password("lz 123");
	if(send(soc,(const char *)name_password.c_str(), name_password.size(), 0) <= 0)
	{
		cout<<"send Error!"<<endl;
	}
	else
	{
		cout<<"send  "<<name_password<<endl;
	}

	//查看服务器是否返回server ok
	buflen = recv(soc, buf, 9, 0);
	strRecvServerOK = buf;
	
	if(buflen < 0)
	{
		//接收失败
		return -2;
	}

	if (strRecvServerOK != "server ok")
	{
		cout<<"strRecvServerOK fail"<<endl;
		return -3;
	}
	else
	{
		cout<<strRecvServerOK<<endl;
	}

	//发送请求数据
	if(send(soc,(const char *)sendbuf.c_str(), sendbuf.size(), 0) <= 0)
	{
		cout<<"send Error!"<<endl;
	}
	else
	{
		cout<<"send  "<<name_password<<endl;
	}

	//接收消息，不断读取协议接收缓冲区
	memset(buf, 0, sizeof(buf));
	int rs = 1;
	while(rs)
	{
		buflen = recv(soc, buf, sizeof(buf), 0);
		resbuf += buf;
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

	//释放套接字描述符
	closesocket(soc);

	//关闭异步套接字
	WSACleanup();

	return 0;
}


//void * FuncSCallProxy1(char * pFuncName,int ParaNum,...)
//{
//	SOCKET soc;
//	SOCKADDR_IN serveraddr;
//
//	//获取不定参数
//	va_list arg_ptr; //定义一个指向参数列表的变量
//	va_start(arg_ptr,ParaNum); //把指向参数列表的变量初始化
//
//	int conerror;
//
//	WSADATA wsa;
//	WSAStartup(MAKEWORD(1,1),&wsa);	//同server
//
//	if((soc = socket(AF_INET,SOCK_STREAM, IPPROTO_TCP)) <= 0)	//同server
//	{
//		printf("Create socket fail!\n");
//	//	return -1;
//	}
//
//	//设置服务器地址、端口号等
//	serveraddr.sin_family = AF_INET;
//	serveraddr.sin_port = htons(789);
//	serveraddr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");	//指本机地址
//
//
//	//连接
//	cout<<"Try to connect..."<<endl;
//	
//	/*
//	connect()用于建立与指定socket的连接
//	第一个参数：标识一个未连接socket
//	第二个参数：指向要连接套接字的sockaddr结构体的指针
//	第三个参数：sockaddr结构体的字节长度
//	返回值：若无错误发生，则connect()返回0。否则的话，返回SOCKET_ERROR错误
//	*/
//
//	if((conerror=connect(soc,(SOCKADDR *)&serveraddr,sizeof(serveraddr))) != 0)
//	{			
//		cout<<"Connect fail:"<<endl;
//		cout<<WSAGetLastError()<<endl;
////		return -1;
//	}
//
//	cout<<"Connected!"<<endl;
//
//
//	char buf[256] = {0};
//	//发送消息
//	sprintf(buf,"%s %d",pFuncName,ParaNum);
//	for(int i=0;i<ParaNum;i++)
//	{
//		char buf2[1024]={0};
//		char Type=va_arg(arg_ptr,char);		//获取下一个char型的参数
//		long Lenth=va_arg(arg_ptr,long);	//获取下一个long型的参数
//		char * temp=va_arg(arg_ptr,char *);	//获取下一个char *型的参数
//
//		strcpy(buf2,buf);
//		sprintf(buf,"%s %c %ld %s",buf2,Type,Lenth,temp);
//	}
//
//	if(send(soc,(const char *)buf,strlen((const char *)buf)+1, 0) <= 0)
//	{
//		cout<<"send Error!"<<endl;
//	}
//
//
//	//接收消息
//	if(recv(soc, buf, 20, 0) <= 0)
//	{
//		cout<<"recive error"<<endl;
//	}
//	cout<<buf<<endl;
//
//
//
//	//释放套接字描述符
//	closesocket(soc);
//
//	//关闭异步套接字
//	WSACleanup();
//
//	//return buf;
//	return NULL;
//}
