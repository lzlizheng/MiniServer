#include"login.h"

//初始化
int loginManager::Init()
{
	userlist["lz"] = "123";
	return 0;
}

//判断用户鉴权是否通过
bool loginManager::IsUserOK(string recv)
{
	vector<string> arr;
    istringstream ss(recv);
    string word;
    while(ss>>word) {
        arr.push_back(word);
    }

	//cout<<"client name and passworld:"<<recv<<endl;
	
	string name = arr[0];
	string password = arr[1];

	//cout<<"name:"<<name<<"  password:"<<password<<endl;

	if( userlist.count(name) == 0 )
	{
		cout<<"用户不存在"<<endl;
		return false;
	}

	if( userlist[name] != password )
	{
		cout<<"密码错误"<<endl;
		return false;
	}

	return true;
}
