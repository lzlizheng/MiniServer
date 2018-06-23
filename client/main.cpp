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
��һ��������ָ���ַ�����ָ��
�ڶ���������Զ�̵��õĲ�������Ŀ
������������Զ�̵��ú����Ĳ���������
���ĸ�������Զ�̵��ú����Ĳ����ĳ���
�����������Զ�̵��ú����Ĳ����ľ�������
���У���~��Ϊ��������
*/
void * FuncSCallProxy1(char * pFuncName,int ParaNum,...)
{
	SOCKET soc;
	SOCKADDR_IN serveraddr;

	//��ȡ��������
	va_list arg_ptr; //����һ��ָ������б�ı���
	va_start(arg_ptr,ParaNum); //��ָ������б�ı�����ʼ��

	int conerror;

	WSADATA wsa;
	WSAStartup(MAKEWORD(1,1),&wsa);	//ͬserver

	if((soc = socket(AF_INET,SOCK_STREAM, IPPROTO_TCP)) <= 0)	//ͬserver
	{
		printf("Create socket fail!\n");
	//	return -1;
	}

	//���÷�������ַ���˿ںŵ�
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_port = htons(789);
	serveraddr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");	//ָ������ַ


	//����
	cout<<"Try to connect..."<<endl;
	
	/*
	connect()���ڽ�����ָ��socket������
	��һ����������ʶһ��δ����socket
	�ڶ���������ָ��Ҫ�����׽��ֵ�sockaddr�ṹ���ָ��
	������������sockaddr�ṹ����ֽڳ���
	����ֵ�����޴���������connect()����0������Ļ�������SOCKET_ERROR����
	*/

	if((conerror=connect(soc,(SOCKADDR *)&serveraddr,sizeof(serveraddr))) != 0)
	{			
		cout<<"Connect fail:"<<endl;
		cout<<WSAGetLastError()<<endl;
//		return -1;
	}

	cout<<"Connected!"<<endl;



	//������Ϣ
	sprintf(buf,"%s %d",pFuncName,ParaNum);
	for(int i=0;i<ParaNum;i++)
	{
		char buf2[1024]={0};
		char Type=va_arg(arg_ptr,char);		//��ȡ��һ��char�͵Ĳ���
		long Lenth=va_arg(arg_ptr,long);	//��ȡ��һ��long�͵Ĳ���
		char * temp=va_arg(arg_ptr,char *);	//��ȡ��һ��char *�͵Ĳ���

		strcpy(buf2,buf);
		sprintf(buf,"%s %c %ld %s",buf2,Type,Lenth,temp);
	}

	if(send(soc,(const char *)buf,strlen((const char *)buf)+1, 0) <= 0)
	{
		cout<<"send Error!"<<endl;
	}


	//������Ϣ
	if(recv(soc, buf, 20, 0) <= 0)
	{
		cout<<"recive error"<<endl;
	}
	cout<<buf<<endl;



	//�ͷ��׽���������
	closesocket(soc);

	//�ر��첽�׽���
	WSACleanup();

	return buf;
}