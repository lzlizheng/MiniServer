#include<stdio.h>
#include<winsock2.h>
#include<iostream>
#include<string.h>
#include"tree.h"
#include"qRequest.h"

using namespace std;

#pragma comment(lib,"ws2_32.lib")	//����ָʾ




//selectģʽ�µĶ��߳�
//DWORD WINAPI WorkerThread(LPVOID lpParam);

int main()
{
	//========================================================================
	//�����ⲿ�ֵĹ��ܣ�
	//	�ڷ�����������ʱ��ɨ��⺯��
	//ע��
	//	�ڷ�����ֹͣ����֮ǰ�����ٶ����������Ĳ���,�޸Ĵ���ʱҪע�⣨�����
	//========================================================================
	
	//�����������ĸ����
	pTree root = NULL;

	//��ȡ���ļ�������sBuffer��������
	char * file = "FuncSlibs.LIB";
	
	//�洢Lib����Ϣ��Vec
	vector<sFuncBlock> vFuncVec;
	
	//��Lib���еĺ�����Ϣ�洢���ṹ��������
	string sLibPath = "FuncSlibs.LIB";
	if(LibInfotoVec(sLibPath, vFuncVec) == false)
	{
		cout<<"readLib false"<<endl;
		return 1;
	}

	//�������еĽṹ����뵽������������
	if(VectoTree(vFuncVec, root) == false)
	{
		cout<<"readLib false"<<endl;
		return 1;
	}

	//�Զ������������������������ӡ���������
	InOrderTraverse(root);


	

	//=========================================================================
	//
	//������һ����������ͨ�Ų���
	//
	//=========================================================================
	SOCKET serversoc;	//�׽������������ͣ�socket()�����ɹ����򷵻�һ���׽���������
	SOCKET clientsoc;
	SOCKADDR_IN serveraddr;
	SOCKADDR_IN clientaddr;
//	char buf[BUFLEN];
	int len;
	DWORD dwThreadId;	//�߳�ID

	WSADATA wsa;
	WSAStartup(MAKEWORD(1,1),&wsa);	//WSA(Windows Sockets Asynchronous��Windows�첽�׽���)����������
									//MAKEWORD��ָ���±����ĸߵ��ֽ�
	/*
	socket()���ڸ���ָ���ĵ�ַ�塢�������ͺ�Э��������һ���׽ӿڵ������ּ������õ���Դ
	��һ��������ָ��ͨ�ŷ���������AF_INCFָ����������
	�ڶ�������������Ҫ�������׽��ֵ����ͣ�SOCKET_STREAM����ʽ�׽��֣�
	���������������׽���ʹ�õ��ض�Э��
	����ֵ���׽���������
	*/
	if((serversoc = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP)) <= 0)
	{
		printf("Create socket fail!\n");
		return -1;
	}

	serveraddr.sin_family = AF_INET;
	serveraddr.sin_port = htons(789);
	serveraddr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);	//INADDR_ANY��ָ�������еĵ�ַ

	/*
	bind()��һ���ص�ַ��һ�׽ӿ�����
	��һ���������׽�������������socket()����������
	�ڶ��������������׽��֣���һ���������ı��ص�ַ
	�������������ڶ��������ĳ���
	����ֵ���ɹ�����0��ʧ�ܷ���SOCKET_ERROR�����
	*/
	if(bind(serversoc,(SOCKADDR *)&serveraddr, sizeof(serveraddr)) != 0)
	{
		printf("Bind fail!\n");
		return -1;
	}

	printf("Start listen...\n");
	/*
	listen()����һ���׽ӿڲ��������������
	��һ����������ʶһ�������ѽ�������δ���ӵ��׽��ֺţ�������Ը����������������
	�ڶ�����������������������Ӷ��е���󳤶�
	����ֵ���ɹ�����0��ʧ�ܷ���SOCKET_ERROR
	*/
	if(listen(serversoc,2) != 0)
	{
		printf("Listen fail!\n");
		return -1;
	}

	len = sizeof(SOCKADDR_IN);


	//���߳�������к��Ĳ�������
	CReqRes myReqRes(serversoc);

	CreateThread(NULL, 0, myReqRes.WorkerThread, NULL, 0, &dwThreadId);

	while(1)
	{
		cout<<"try to accept"<<endl;
		/*
		accept()
		��һ�������������׽���������
		�ڶ����������ͻ����׽��ֵ�ַ�ṹ��ָ��
		�������������ͻ����׽��ֵ�ַ�ĳ���
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

	//�ͷ��׽���������
	closesocket(serversoc);

	//�ر��첽�׽���
	WSACleanup();

 


	//�����ⲿ�����ݲ���������ͨ�Ų���

	//����ɨ��⺯�����γɵĶ���������
	DeleteBST(root);

	return 0;
}