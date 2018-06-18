#include <fstream>
#include <Windows.h>
#include "tree.h"

typedef int(*Dllfun)(int *,int);

//-----------------------------------------------------------
// Summary:
//	��ָ����Lib�ļ��е���Ϣ�洢����ź�����Ϣ�ṹ���������
//------------------------------------------------------------
bool LibInfotoVec(string sLibPath, vector<sFuncBlock> & vFuncVec)
{
	ifstream inf((const char *)sLibPath.c_str(), ios::in|ios::binary);

	//�ܵ�����
	string::size_type nIter = 28;

	//ÿһ�������������Ϣ��ƫ�Ƶ�����
	int iIter = 0;
	//��¼���������ݴ�С
	int iFuncSize = 0;

	inf.seekg(nIter,ios::beg);

	while(inf.peek() != EOF)
	{
		sFuncBlock * pFuncBlock = new sFuncBlock();

		//���������鿪ʼ�ġ�Func Block��
		if(iIter < 10)
		{
			iIter++;
			inf.seekg(nIter+iIter,ios::beg);
			continue;
		}
		
		//��ȡ������Ĵ�С
		inf.read((char *)&iFuncSize,sizeof(int));
		iIter += 4;

		//��ȡ������
		inf.read(pFuncBlock->sFuncName,255);
		iIter += 255;

		pFuncBlock->iBeginPos = inf.tellg();
		pFuncBlock->iEndPos = pFuncBlock->iBeginPos + iFuncSize;
		iIter += iFuncSize;

		//���ýṹ��׷�ӵ�������
		vFuncVec.push_back(*pFuncBlock);

		//������ѭ����ƫ�Ƶ�ַ��ӵ��ܵĵ�������
		nIter += iIter;
		inf.seekg(nIter,ios::beg);

		//����ȡ��iFuncSize��ƫ�Ƶ�����iIterֵ��գ��������һ�δ�ֵ��ɸ���
		iFuncSize = 0;
		iIter = 0;
	}

	return true;
}



//-------------------------------------------------------------
// Summary:
//	���洢Lib�ļ��к�����Ϣ�Ľṹ����������������
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
//	���ݺ��������ڶ�����������Ѱ��Ŀ�꺯������ȡ���ڿ��ļ��е�ƫ����
//������������д��һ����ʱ�ļ��У����е��á�
// parameter:
//	[in] root - ���������������
//	[in] sLibPath - ���ļ�·��
//	[in] funcname - �����ú�����
//	[in] paralist - �����������
//	[out] returntype - ����ֵ����
//	[out] returnlenth - ����ֵ����
//	[out] returnvalue - ����ֵ�ַ�������
// return:
//	�ɹ�����true��ʧ�ܷ���false
//--------------------------------------------------------------
bool callfunc(pTree root, string sLibPath, char funcname[255], vector<sPara> paralist, char & returntype, long & returnlenth, char * returnvalue)
{
	//���ݺ��������ڶ�����������Ѱ��Ŀ�꺯��
	pTree pFuncNode;	//ָ��Ŀ�꺯�����ڽڵ��ָ��

	SearchBST(root,funcname,NULL,pFuncNode);

	//��ȡĿ�꺯���ڿ��ļ��е�ƫ����,������������д��һ����ʱ�ļ�"temp.dll"��
	ifstream inf((const char *)sLibPath.c_str(), ios::in|ios::binary);	//�����ļ���
	inf.seekg(pFuncNode->funcBlock->iBeginPos,ios::beg);

	long iFuncLen = pFuncNode->funcBlock->iEndPos - pFuncNode->funcBlock->iBeginPos;	//�����鳤��
	char * sFuncBuf;	//��ʱ�洢��������
	
	inf.read(sFuncBuf, iFuncLen);
	inf.close();

	char * tempDllName = "temp.dll";	//��ʱdll�ļ���
	ofstream outf(tempDllName, ios::out|ios::binary);	//����ļ���
	outf.write(sFuncBuf, iFuncLen);
	outf.close();
	

	//���е��ã���̬����dll�ļ�
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

	//������ԭ
	//����vecPara�е�����
	vector<sPara>::iterator ivecPara;




	//ò�ƻ��漰������ջ������




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