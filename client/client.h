#include<stdio.h>
#include<winsock2.h>
#include<iostream>
#include<string>
using namespace std;

#pragma comment(lib,"ws2_32.lib")

//---------------------------------------------------------
//功能：从文件中读取函数名和json格式的参数数据
//参数：[out] sendbuf -- 读取到的待发送数据
//返回值：0 -- 成功；其他 -- 失败，详见错误码
//---------------------------------------------------------
int ReadSendingData(string& sendbuf);

//----------------------------------------------------------
//功能：将sendbuf中的内容发送到服务器
//参数：sendbuf -- 待发送数据
//		resbuf -- 返回数据
//返回值：0 -- 成功；其他 -- 失败，具体看错误码
//----------------------------------------------------------
int SendRequest(const string& sendbuf, string& resbuf);

//第一个参数：指向字符串的指针
//第二个参数：远程调用的参数的数目
//第三个参数：远程调用函数的参数的类型
//第四个参数：远程调用函数的参数的长度
//第五个参数：远程调用函数的参数的具体内容
//其中，三~五为不定参数
void * FuncSCallProxy1(char * pFuncName,int ParaNum,...);
