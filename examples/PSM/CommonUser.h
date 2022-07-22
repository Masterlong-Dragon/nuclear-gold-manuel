#pragma once
#include "Account.h"
class CommonUser :
	public Account
{
public:
	// ����
	static CommonUser* getInstance()
	{
		static CommonUser instance;
		return &instance;
	}
public:
	// ��Ӧ�����ݱ�ṹ
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

