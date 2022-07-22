#pragma once
#include "Account.h"
class CommonUser :
	public Account
{
public:
	// 单例
	static CommonUser* getInstance()
	{
		static CommonUser instance;
		return &instance;
	}
public:
	// 对应的数据表结构
	PsmDataStorage* schedule_table;
	PsmDataStorage* addrbook_table;
	PsmDataStorage* finance_table;
	PsmDataStorage* fit_table;
	PsmDataStorage* pswd_table;
	std::unordered_map<std::wstring, bool> valid_accounts;
	virtual bool login();
	virtual bool signup();
	virtual bool update(PsmPswdData& upd);
	virtual bool set(PsmPswdData& upd);
	PsmPswdData  get();
private:
	std::wstring db_name;
	CommonUser();
	~CommonUser();
};

