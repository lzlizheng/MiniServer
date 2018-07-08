#include"CommenUtil.h"
#include<map>
#include<vector>
#include<sstream>

class loginManager{
private:
	//已注册用户列表
	map<string, string> userlist;

public:
	//初始化
	int Init();
	//判断用户鉴权是否通过
	bool IsUserOK(string recv);
};
