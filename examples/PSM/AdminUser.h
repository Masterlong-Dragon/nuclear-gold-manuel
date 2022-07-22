#pragma once
#include "Account.h"
#include "psm_core/psm_pswd_data.h"
class AdminUser : public Account
{
public:
	// ����
	static AdminUser* getInstance()
	{
		static AdminUser instance;
		return &instance;
	}
public:
	// ��Ӧ�����ݱ�ṹ ��ֻ�����˺����룩
	PsmDataStorage* pswd_table;
	std::unordered_map<std::wstring, bool> valid_accounts;
	virtual bool login();
	virtual bool signup();
	virtual bool update(PsmPswdData& upd);
	virtual bool set(PsmPswdData& upd);
	// ɾ���˻�
	bool del(PsmPswdData& upd);
	// ����ע���˻�
	bool add(PsmPswdData& info);
private:
	std::wstring db_name;
	PsmPswdData current;
	AdminUser();
	~AdminUser();
};

