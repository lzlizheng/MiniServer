#include"CommenUtil.h"
#include<winsock2.h>
#include<string>
#include"tree.h"
#include<deque>
#include<cassert>  //�쳣����
#include"login.h"

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
	bool res = LibInfotoVec(sLibPath, vFuncVec);
	assert(res);

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
	SOCKET ListeningSocket;	//�׽������������ͣ�socket()�����ɹ����򷵻�һ���׽���������
	SOCKET NewConnection;
	SOCKADDR_IN serveraddr;
	SOCKADDR_IN clientaddr;
//	char buf[BUFLEN];
	int len;

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
	if((ListeningSocket = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP)) <= 0)
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
	if(bind(ListeningSocket,(SOCKADDR *)&serveraddr, sizeof(serveraddr)) != 0)
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
		��һ�������������׽���������
		�ڶ����������ͻ����׽��ֵ�ַ�ṹ��ָ��
		�������������ͻ����׽��ֵ�ַ�ĳ���
		*/
		if((NewConnection = accept(ListeningSocket,(SOCKADDR *)&clientaddr, &len)) <= 0)
		{
			printf("Accept fail!\n");
			return -1;
		}

		printf("Connected\n");

		/*
		recv()
		��һ���������׽���������
		�ڶ��������������������ݻ�������ָ��
		�������������ڶ��������ĳ���
		���ĸ�������ָ��������Ʒ�ʽ��һ����0
		����ֵ���ɹ��������ܹ����յ��ֽ�����������ӱ��رգ�����0�������������SOCKET_ERROR��
		*/
		//����hello server
		char buf[BUFLEN]={0};
		int buflen = recv(NewConnection, buf, 12, 0);
		if(buflen < 0)
		{
			//����ʧ��
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

		//����server ok
		if(send(NewConnection, "server ok", 9, 0) <= 0)
		{
			cout<<"send Error!"<<endl;
		}
		else
		{
			cout<<"send hello world!"<<endl;
		}

		//���ռ�Ȩ���룬�����м�Ȩ
		memset(buf,BUFLEN,0);
		buflen = recv(NewConnection, buf, BUFLEN, 0);
		if(buflen < 0)
		{
			//����ʧ��
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
		
		//����server ok
		if(send(NewConnection, "server ok", 9, 0) <= 0)
		{
			cout<<"send Error!"<<endl;
		}
		else
		{
			cout<<"send hello world!"<<endl;
		}

		//������Ϣ�����϶�ȡЭ����ջ�����
		memset(buf, 0, sizeof(buf));
		strRecv = "";
		int rs = 1;
		while(rs)
		{
			buflen = recv(NewConnection, buf, sizeof(buf), 0);
			strRecv += buf;
			if(buflen < 0)
			{
				// �����Ƿ�������ģʽ,���Ե�buflenΪEAGAINʱ,��ʾ��ǰ�������������ݿɶ�
				// ������͵����Ǹô��¼��Ѵ���
				if(buflen == EAGAIN)
					break;
				else
				{
					//����ʧ��
					return -2;
				}
			}
			else if(buflen == 0)
			{
				// �����ʾ�Զ˵�socket�������ر�.
				break;
			}
			if(buflen != sizeof(buf))
				rs = 0;
			else
			{
				memset(buf, 0, sizeof(buf));
				rs = 1;// ��Ҫ�ٴζ�ȡ
			}
		}


		//��Ϣ��ӣ����ȴ���Ϣ����





		//����server ok
		if(send(NewConnection, "server ok", 9, 0) <= 0)
		{
			cout<<"send Error!"<<endl;
		}
		else
		{
			cout<<"send hello world!"<<endl;
		}

		//���ͷ��ؽ��



		

		


	}

	//�ͷ��׽���������
	closesocket(ListeningSocket);
	closesocket(NewConnection);

	//�ر��첽�׽���
	WSACleanup();




	//�����ⲿ�����ݲ���������ͨ�Ų���

	//����ɨ��⺯�����γɵĶ���������
	DeleteBST(root);

	return 0;
}