#include"client.h"

int main()
{
//======================================
//====FuncSCallProxy1方式===============
//	char *pFuncName="add";
//	char *pContent="1";
//	long added=strlen(pContent);
//	void * pRet;

//	pRet=FuncSCallProxy1(pFuncName,2,'d',added,pContent,'s',8,"hello s");


//======================================
//====FuncSCallProxy3方式===============
	int iret = 0;

	string senddata;
	string resdata;

	//发送“hello server”
	iret = SendRequest("hello server", resdata);
	cout<<resdata<<endl;
	resdata = "";

	//从指定文件中读取发送数据
	iret = ReadSendingData(senddata);
	//将数据发送给服务器，并得到返回数据
	iret = SendRequest(senddata, resdata);

	cout<<resdata<<endl;

	return 0;
}
