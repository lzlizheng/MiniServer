#include"QRequest.h"

//------------------------------------------------------------------------------------------------
//���ܣ�
//	��Ĺ��캯������ʼ�����������������ڷ��������׽���
//------------------------------------------------------------------------------------------------
CQueue::CQueue()
{

}

//===================================================================================================
//������������ע���ĺ�����Χ�Ƶ�ClientSoc���У��Ѿ���Ϊserver��ȫ�ֱ�����������

//------------------------------------------------------------------------------------------------
//���ܣ���accept���ķ�������Ŀͻ����׽��ּ���ͻ��׽��ֶ���
//������[in] clientsoc -- Ҫ������Ŀͻ����׽���
//------------------------------------------------------------------------------------------------
//void CReqRes::PushClientSoc(SOCKET clientsoc)
//{
//	m_qClientSoc.push(clientsoc);
//}

//------------------------------------------------------------------------------------------------
//���ܣ��ӿͻ��׽��ֶ����л�ȡһ���׽���
//������[out] clientsoc -- Ҫ��ȡ���Ŀͻ����׽��ֵ�����
//����ֵ��void
//------------------------------------------------------------------------------------------------
//void CReqRes::PopClientSoc(SOCKET & clientsoc)
//{
//	clientsoc = m_qClientSoc.front();
//	m_qClientSoc.pop();
//}
//=====================================================================================================



//------------------------------------------------------------------------------------------------------
//���ܣ���strReqBuf�����ݽ��н������������������
//������[in] clientsoc - �ͻ����׽���
//-------------------------------------------------------------------------------------------------------
void CQueue::Push(SOCKET clientsoc)
{
	//���Կͻ��˵����������ַ���
	char strReqBuf[BUFLEN];

//===================================================================

//�����ⲿ�ֱ�ע�͵�����Ϊaccept�������������б�����

/*
	//�ͻ����׽��ֵ�ַ�ĳ���
	int iClientSocAddrLen = sizeof(SOCKADDR_IN);
	//�ͻ����׽���
	SOCKET clientsoc;
	//�ͻ����׽��ֵ�ַ�ṹ��ָ��(�������Լ����Լ��ϵģ����ܲ���ʵ�ʽ��ж��߳�ͨ��)
	SOCKADDR_IN clientaddr;

	//���Կͻ��˵����������ַ���
	char strReqBuf[BUFLEN];
*/
	/*
	accept()
	��һ�������������׽���������
	�ڶ����������ͻ����׽��ֵ�ַ�ṹ��ָ��
	�������������ͻ����׽��ֵ�ַ�ĳ���
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
	��һ���������׽���������
	�ڶ��������������������ݻ�������ָ��
	�������������ڶ��������ĳ���
	���ĸ�������ָ��������Ʒ�ʽ�����壿��
	����ֵ���ɹ��������ܹ����յ��ֽ�����������ӱ��رգ�����0�������������SOCKET_ERROR��
	*/

//===================================================================

	//iRecLen�ǽ��յ����ݵĳ��ȣ�strReqBuf���ݵĳ���
	int iRecLen = recv(clientsoc, strReqBuf, BUFLEN, 0);

	if(iRecLen <= 0)
	{
		printf("Close connection\n");
		//�ر��׽���
		closesocket(clientsoc);
		return;
	}
	printf("%s\n",strReqBuf);



//========================================================================
//
//�������Կͻ��˵������ַ���
//
//========================================================================

	//ָ���������ݻ������ļ�����
	int iBuf=0;
	//����ṹ��
	SRequstNode sRequst;


	//��ȡ��һ������(255�ֽڵĺ���ȫ����)
	for(iBuf; ( strReqBuf[iBuf] != ' ' ) && ( iBuf < 255 ); iBuf++)
	{
		sRequst.funcName[iBuf] = strReqBuf[iBuf];
	}


	//��ȡ�ڶ�������(��θ���)
	iBuf++;

	for(iBuf; strReqBuf[iBuf] != ' '; iBuf++)
	{
		sRequst.paraNum = sRequst.paraNum * 10 + strReqBuf[iBuf] - '0';
	}


	//��ȡ����Ĳ���
	int iParaNum = sRequst.paraNum;

	while(iParaNum--)
	{
		Para a;

		//��ȡ��������
		iBuf++;
		a.type=strReqBuf[iBuf];

		//��ȡ��������
		iBuf+=2;
		char tempLenth[25];
		for(int i=0;strReqBuf[iBuf]!=' ';i++,iBuf++)
		{
			tempLenth[i]=strReqBuf[iBuf];
		}
		a.lenth = atol(tempLenth);


		//��ȡ����ֵ
		iBuf++;
		for(int j=0;strReqBuf[iBuf]&&strReqBuf[iBuf]!=' ';j++,iBuf++)
		{
			a.value[j]=strReqBuf[iBuf];
		}
		
		//������a��ӽ���������
		sRequst.vecPara.push_back(a);
	}


	//��������ṹ������������
	m_qRequest.push(sRequst);

	return;
}

//------------------------------------------------------------------------------------------------
//���ܣ���������ж���ȡ������ִ����ز�����������������������
//------------------------------------------------------------------------------------------------
void CQueue::RunTask()
{
	SRequst sReqData = m_qRequest.front();
	m_qRequest.pop();

	char cReturnType = 0;	//����ֵ����
	long iReturnlenth = 0;	//����ֵ����
	char * cReturnValue = "";	//����ֵ�ַ���

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
//���ܣ���������ж��׵Ľ����Ϣ�����Ϣ��,���͸��ͻ���
//------------------------------------------------
void CQueue::Pop()
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
