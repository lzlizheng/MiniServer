#include"client.h"

int main()
{
//	char *pFuncName="add";
//	char *pContent="1";
//	long added=strlen(pContent);
//	void * pRet;

//	pRet=FuncSCallProxy1(pFuncName,2,'d',added,pContent,'s',8,"hello s");


	string senddata;
	string resdata;
	//��ָ���ļ��ж�ȡ��������
	int iret = ReadSendingData(senddata);
	//�����ݷ��͸������������õ���������
	iret = SendRequest(senddata, resdata);

	cout<<resdata<<endl;

	return 0;
}
