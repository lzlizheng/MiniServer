#include"login.h"

//��ʼ��
int loginManager::Init()
{
	userlist["lz"] = "123";
	return 0;
}

//�ж��û���Ȩ�Ƿ�ͨ��
bool loginManager::IsUserOK(string recv)
{
	vector<string> arr;
    istringstream ss(recv);
    string word;
    while(ss>>word) {
        arr.push_back(word);
    }
	
	string name = arr[0];
	string password = arr[1];

	if( userlist.count(name) == 0 )
	{
		cout<<"�û�������"<<endl;
		return false;
	}

	if( userlist[name] != password )
	{
		cout<<"�������"<<endl;
		return false;
	}

	return true;
}
