#include"CommenUtil.h"
#include<map>
#include<vector>
#include<sstream>

class loginManager{
private:
	//��ע���û��б�
	map<string, string> userlist;

public:
	//��ʼ��
	int Init();
	//�ж��û���Ȩ�Ƿ�ͨ��
	bool IsUserOK(string recv);
};
