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
	//从指定文件中读取发送数据
	int iret = ReadSendingData(senddata);
	//将数据发送给服务器，并得到返回数据
	iret = SendRequest(senddata, resdata);

	cout<<resdata<<endl;

	return 0;
}
