#include"QRequest.h"

//------------------------------------------------------------------------------------------------
//功能：
//	类的构造函数，初始化该请求结果队列所在服务器的套接字
//------------------------------------------------------------------------------------------------
CReqRes::CReqRes(SOCKET serversoc):m_serversoc(serversoc)
{

}

//静态成员需要外部声明！！！
int CReqRes::g_iTotalConn = 0;
SOCKET CReqRes::g_CliSocketArr[256];

//===================================================================================================
//下面这两个被注掉的函数所围绕的ClientSoc队列，已经作为server的全局变量被操作了

//------------------------------------------------------------------------------------------------
//功能：将accept到的发来请求的客户端套接字加入客户套接字队列
//参数：[in] clientsoc -- 要被加入的客户端套接字
//------------------------------------------------------------------------------------------------
//void CReqRes::PushClientSoc(SOCKET clientsoc)
//{
//	m_qClientSoc.push(clientsoc);
//}

//------------------------------------------------------------------------------------------------
//功能：从客户套接字队列中获取一个套接字
//参数：[out] clientsoc -- 要被取出的客户端套接字的引用
//返回值：void
//------------------------------------------------------------------------------------------------
//void CReqRes::PopClientSoc(SOCKET & clientsoc)
//{
//	clientsoc = m_qClientSoc.front();
//	m_qClientSoc.pop();
//}
//=====================================================================================================



//------------------------------------------------------------------------------------------------------
//功能：对strReqBuf的内容进行解析，并加入任务队列
//参数：[in] clientsoc - 客户端套接字
//-------------------------------------------------------------------------------------------------------
void CReqRes::PushTask(SOCKET clientsoc)
{
	//来自客户端的请求数据字符串
	char strReqBuf[BUFLEN];

//===================================================================

//下面这部分被注释掉是因为accept方法在主程序中被调用

/*
	//客户方套接字地址的长度
	int iClientSocAddrLen = sizeof(SOCKADDR_IN);
	//客户端套接字
	SOCKET clientsoc;
	//客户端套接字地址结构的指针(这是我自己尝试加上的，看能不能实际进行多线程通信)
	SOCKADDR_IN clientaddr;

	//来自客户端的请求数据字符串
	char strReqBuf[BUFLEN];
*/
	/*
	accept()
	第一个参数：本地套接字描述符
	第二个参数：客户端套接字地址结构的指针
	第三个参数：客户方套接字地址的长度
	*/
/*
	if((clientsoc = accept(m_serversoc,(SOCKADDR *)&clientaddr, &iClientSocAddrLen)) <= 0)
	{
		printf("Accept fail!\n");
		return;
	}

	printf("Connected\n");
*/

	/*
	recv()
	第一个参数：套接字描述符
	第二个参数：接收输入数据缓冲区的指针
	第三个参数：第二个参数的长度
	第四个参数：指定传输控制方式（具体？）
	返回值：成功，返回总共接收的字节数。如果连接被关闭，返回0。其他情况返回SOCKET_ERROR。
	*/

//===================================================================

	//iRecLen是接收的数据的长度，strReqBuf数据的长度
	int iRecLen = recv(clientsoc, strReqBuf, BUFLEN, 0);

	if(iRecLen <= 0)
	{
		printf("Close connection\n");
		//关闭套接字
		closesocket(clientsoc);
		return;
	}
	printf("%s\n",strReqBuf);



//========================================================================
//
//解析来自客户端的请求字符串
//
//========================================================================

	//指向请求数据缓冲区的计数器
	int iBuf=0;
	//请求结构体
	SRequst sRequst;


	//提取第一个参数(255字节的函数全名称)
	for(iBuf; ( strReqBuf[iBuf] != ' ' ) && ( iBuf < 255 ); iBuf++)
	{
		sRequst.funcName[iBuf] = strReqBuf[iBuf];
	}


	//提取第二个参数(入参个数)
	iBuf++;

	for(iBuf; strReqBuf[iBuf] != ' '; iBuf++)
	{
		sRequst.paraNum = sRequst.paraNum * 10 + strReqBuf[iBuf] - '0';
	}


	//提取后面的参数
	int iParaNum = sRequst.paraNum;

	while(iParaNum--)
	{
		Para a;

		//提取参数类型
		iBuf++;
		a.type=strReqBuf[iBuf];

		//提取参数长度
		iBuf+=2;
		char tempLenth[25];
		for(int i=0;strReqBuf[iBuf]!=' ';i++,iBuf++)
		{
			tempLenth[i]=strReqBuf[iBuf];
		}
		a.lenth = atol(tempLenth);


		//提取参数值
		iBuf++;
		for(int j=0;strReqBuf[iBuf]&&strReqBuf[iBuf]!=' ';j++,iBuf++)
		{
			a.value[j]=strReqBuf[iBuf];
		}
		
		//将参数a添加进参数数组
		sRequst.vecPara.push_back(a);
	}


	//请求任务结构体加入请求队列
	m_qRequest.push(sRequst);

	return;
}

//------------------------------------------------------------------------------------------------
//功能：从请求队列队首取出任务，执行相关操作，将处理结果放入结果队列
//------------------------------------------------------------------------------------------------
void CReqRes::RunTask()
{
	SRequst sReqData = m_qRequest.front();
	m_qRequest.pop();

	char cReturnType = 0;	//返回值类型
	long iReturnlenth = 0;	//返回值长度
	char * cReturnValue = "";	//返回值字符串

//		callfunc(root, "", sReqData.funcName, sReqData.vecPara, cReturnType, iReturnlenth, cReturnValue);


	SResult resValue;
	resValue.sReturnData.lenth = iReturnlenth;
	resValue.sReturnData.type = cReturnType;
	memcpy(resValue.sReturnData.value, cReturnValue, resValue.sReturnData.lenth);
	resValue.socClient = sReqData.socClient;

	m_qResult.push(resValue);
	return;
}

//------------------------------------------------
//功能：将结果队列队首的结果信息打成信息包,发送给客户端
//------------------------------------------------
void CReqRes::PopTask()
{
	SResult resValue = m_qResult.front();
	m_qResult.pop();

	char buf[1024];
	memset(buf, 0, 1024);
//		sprintf(buf,"%c %ld %s", resValue.sReturnData.type, resValue.sReturnData.lenth, resValue.sReturnData.value);
	sprintf(buf, "%d",33333333);
	if(send(resValue.socClient,(const char *)buf,strlen((const char *)buf)+1, 0) <= 0)
	{
		printf("Error!\n");
	}

	return;
}


DWORD WINAPI CReqRes::WorkerThread(LPVOID lpParam)
{
	int            i;
	fd_set         fdread;
	int            ret;
	struct timeval tv = {1, 0};
	char           szMessage[MSGSIZE];

	while(true)
	{
		FD_ZERO(&fdread);

		//参考http://bbs.csdn.net/topics/50400895

		for (i = 0; i < g_iTotalConn; i++)
		{
		  FD_SET(g_CliSocketArr[i], &fdread);
		}

		// We only care read event
		ret = select(0, &fdread, NULL, NULL, &tv);
		if (ret == 0 || (ret == SOCKET_ERROR && WSAGetLastError() == WSAECONNRESET))
		{
			// Time expired
			continue;
		}

		for (i = 0; i < g_iTotalConn; i++)
		{
			if (FD_ISSET(g_CliSocketArr[i], &fdread))
			{
				// A read event happened on g_CliSocketArr[i]
				ret = recv(g_CliSocketArr[i], szMessage, MSGSIZE, 0);
        
				if (ret == 0 || (ret == SOCKET_ERROR && WSAGetLastError() == WSAECONNRESET))
				{
					// Client socket closed
					printf("Client socket %d closed.\n", g_CliSocketArr[i]);
					closesocket(g_CliSocketArr[i]);
					
					if (i < g_iTotalConn - 1)
					{
					  g_CliSocketArr[i--] = g_CliSocketArr[--g_iTotalConn];
					}
				}
				else
				{
		    		// We received a message from client
					szMessage[ret] = '\0';
					send(g_CliSocketArr[i], szMessage, strlen(szMessage), 0);
				}
			}
		}
	}

	return 0;
}