#ifndef _COMMENUTIL___
#define _COMMENUTIL___

#define BUFLEN 1024

typedef struct Para{
	char type;
	long lenth;
	char value[50];

	//结构体构造函数
	Para()
	{
		type = 0;
		lenth = 0;
		memset(value,0,50);
	}
}sPara;


#endif