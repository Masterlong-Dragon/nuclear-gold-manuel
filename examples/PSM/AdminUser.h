#pragma once
#include "Account.h"
#include "psm_core/psm_pswd_data.h"
class AdminUser : public Account
{
public:
	// 单例
	static AdminUser* getInstance()
	{
		static AdminUser instance;
		return &instance;
	}
public:
	// 对应的数据表结构 （只保存账号密码）
	PsmDataStorage* pswd_table;
	std::unordered_map<std::wstring, bool> valid_accounts;
	virtual bool login();
	virtual bool signup();
	virtual bool update(PsmPswdData& upd);
	virtual bool set(PsmPswdData& upd);
	// 删除账户
	bool del(PsmPswdData& upd);
	// 增加注册账户
	bool add(PsmPswdData& info);
private:
	std::wstring db_name;
	PsmPswdData current;
	AdminUser();
	~AdminUser();
};

