#include <fstream>
#include <Windows.h>
#include "tree.h"

typedef int(*Dllfun)(int *,int);

//-----------------------------------------------------------
// Summary:
//	将指定的Lib文件中的信息存储到存放函数信息结构体的容器中
//------------------------------------------------------------
bool LibInfotoVec(string sLibPath, vector<sFuncBlock> & vFuncVec)
{
	ifstream inf((const char *)sLibPath.c_str(), ios::in|ios::binary);

	//总迭代器
	string::size_type nIter = 28;

	//每一个函数块相关信息的偏移迭代器
	int iIter = 0;
	//记录函数块内容大小
	int iFuncSize = 0;

	inf.seekg(nIter,ios::beg);

	while(inf.peek() != EOF)
	{
		sFuncBlock * pFuncBlock = new sFuncBlock();

		//跳过函数块开始的“Func Block”
		if(iIter < 10)
		{
			iIter++;
			inf.seekg(nIter+iIter,ios::beg);
			continue;
		}
		
		//读取函数块的大小
		inf.read((char *)&iFuncSize,sizeof(int));
		iIter += 4;

		//读取函数名
		inf.read(pFuncBlock->sFuncName,255);
		iIter += 255;

		pFuncBlock->iBeginPos = inf.tellg();
		pFuncBlock->iEndPos = pFuncBlock->iBeginPos + iFuncSize;
		iIter += iFuncSize;

		//将该结构体追加到容器中
		vFuncVec.push_back(*pFuncBlock);

		//将本次循环的偏移地址添加到总的迭带器里
		nIter += iIter;
		inf.seekg(nIter,ios::beg);

		//将读取的iFuncSize和偏移迭带器iIter值清空，以免对下一次存值造成干扰
		iFuncSize = 0;
		iIter = 0;
	}

	return true;
}



//-------------------------------------------------------------
// Summary:
//	将存储Lib文件中函数信息的结构体存入二叉排序树中
//-------------------------------------------------------------
bool VectoTree(vector<sFuncBlock> & vFuncVec, pTree & root)
{
	for(vector<sFuncBlock>::iterator iter = vFuncVec.begin(); iter != vFuncVec.end(); iter++)
	{
		InsertBST(root,iter);
	}

	return true;
}

//--------------------------------------------------------------
// Summary:	
//	根据函数名，在二叉排序树中寻找目标函数，获取其在库文件中的偏移量
//并将函数内容写到一个临时文件中，进行调用。
// parameter:
//	[in] root - 二叉排序树根结点
//	[in] sLibPath - 库文件路径
//	[in] funcname - 被调用函数名
//	[in] paralist - 参数数组对象
//	[out] returntype - 返回值类型
//	[out] returnlenth - 返回值长度
//	[out] returnvalue - 返回值字符串内容
// return:
//	成功返回true，失败返回false
//--------------------------------------------------------------
bool callfunc(pTree root, string sLibPath, char funcname[255], vector<sPara> paralist, char & returntype, long & returnlenth, char * returnvalue)
{
	//根据函数名，在二叉排序树中寻找目标函数
	pTree pFuncNode;	//指向目标函数所在节点的指针

	SearchBST(root,funcname,NULL,pFuncNode);

	//获取目标函数在库文件中的偏移量,并将函数内容写到一个临时文件"temp.dll"中
	ifstream inf((const char *)sLibPath.c_str(), ios::in|ios::binary);	//输入文件流
	inf.seekg(pFuncNode->funcBlock->iBeginPos,ios::beg);

	long iFuncLen = pFuncNode->funcBlock->iEndPos - pFuncNode->funcBlock->iBeginPos;	//函数块长度
	char * sFuncBuf;	//临时存储函数内容
	
	inf.read(sFuncBuf, iFuncLen);
	inf.close();

	char * tempDllName = "temp.dll";	//临时dll文件名
	ofstream outf(tempDllName, ios::out|ios::binary);	//输出文件流
	outf.write(sFuncBuf, iFuncLen);
	outf.close();
	

	//进行调用：动态调用dll文件
	Dllfun dllFunc;
	HINSTANCE hDll;
	hDll=LoadLibrary(tempDllName);

	if(hDll==NULL)
	{
		FreeLibrary(hDll);
	}

	dllFunc=(Dllfun)GetProcAddress(hDll,funcname);
	if(dllFunc==NULL)
	{
		FreeLibrary(hDll);
	}

	//参数还原
	//检验vecPara中的内容
	vector<sPara>::iterator ivecPara;




	//貌似会涉及到参数栈。。。




	for(ivecPara = paralist.begin(); ivecPara != paralist.end(); ivecPara++)
	{
		switch(ivecPara->type)
		{
		case 'd':
		case 'D':
//			int * pInt = new int;
//			* pInt = atoi(ivecPara->value);


			break;
		case 'c':
		case 'C':

			break;

		case 'f':
		case 'F':

			break;

		case 's':
		case 'S':

			break;
			
		default:
			break;
		}

		cout<<"type:"<<ivecPara->type<<"    lenth:"<<ivecPara->lenth<<"    value:"<<ivecPara->value<<endl;
	}

	
	
	return true;
}