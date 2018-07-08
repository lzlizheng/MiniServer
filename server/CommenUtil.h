#ifndef _COMMENUTIL___
#define _COMMENUTIL___

#define BUFLEN 1024

#include<stdio.h>
#include<iostream>
#include<string>
using namespace std;

//ºöÂÔÒ»Ğ©VC±àÒëµÄ´íÎó
//begin
#pragma   warning   (disable:   4786)
#pragma   warning(push) 
#include   <yvals.h>  
#pragma   warning(disable:   4018)
#pragma   warning(disable:   4100)
#pragma   warning(disable:   4245)
#pragma   warning(disable:   4512)
#pragma   warning(disable:   4663)
#pragma   warning(disable:   4710)
#pragma   warning(disable:   4786 4514)
#pragma   warning(disable:   4146)
#pragma   warning(pop)
//end

typedef struct Para{
	char type;
	long lenth;
	char value[50];

	Para():type(0),lenth(0)
	{
		memset(value,0,50);
	}
}sPara;


#endif