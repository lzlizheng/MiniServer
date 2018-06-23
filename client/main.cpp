#include<stdio.h>
#include<winsock2.h>
#include<iostream>
using namespace std;

#pragma comment(lib,"ws2_32.lib")

void * FuncSCallProxy1(char * pFuncName,int ParaNum,...);

char buf[1024]={0};

int main()
{
	char *pFuncName="add";
	char *pContent="1";
	long added=strlen(pContent);
	void * pRet;

	pRet=FuncSCallProxy1(pFuncName,2,'d',added,pContent,'s',8,"hello s");

//	free(pRet);

	return 0;
}

/*
第一个参数：指向字符串的指针
第二个参数：远程调用的参数的数目
第三个参数：远程调用函数的参数的类型
第四个参数：远程调用函数的参数的长度
第五个参数：远程调用函数的参数的具体内容
其中，三~五为不定参数
*/
void * FuncSCallProxy1(char * pFuncName,int ParaNum,...)
{
	SOCKET soc;
	SOCKADDR_IN serveraddr;

	//获取不定参数
	va_list arg_ptr; //定义一个指向参数列表的变量
	va_start(arg_ptr,ParaNum); //把指向参数列表的变量初始化

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
//		return -1;
	}

	cout<<"Connected!"<<endl;



	//发送消息
	sprintf(buf,"%s %d",pFuncName,ParaNum);
	for(int i=0;i<ParaNum;i++)
	{
		char buf2[1024]={0};
		char Type=va_arg(arg_ptr,char);		//获取下一个char型的参数
		long Lenth=va_arg(arg_ptr,long);	//获取下一个long型的参数
		char * temp=va_arg(arg_ptr,char *);	//获取下一个char *型的参数

		strcpy(buf2,buf);
		sprintf(buf,"%s %c %ld %s",buf2,Type,Lenth,temp);
	}

	if(send(soc,(const char *)buf,strlen((const char *)buf)+1, 0) <= 0)
	{
		cout<<"send Error!"<<endl;
	}


	//接收消息
	if(recv(soc, buf, 20, 0) <= 0)
	{
		cout<<"recive error"<<endl;
	}
	cout<<buf<<endl;



	//释放套接字描述符
	closesocket(soc);

	//关闭异步套接字
	WSACleanup();

	return buf;
}