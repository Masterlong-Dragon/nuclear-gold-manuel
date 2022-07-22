#include "AdminUser.h"
#include "code_helper.h"
#include <unordered_map>
#include <filesystem>
namespace fs = std::filesystem;

bool AdminUser::login()
{
	/*if (info._account[0] == '\0')
		return false;
	if (!valid_accounts[A_To_W(std::string(info._account))])
		return false;*/
	if (psmInitConnect((char*)(ANSIToUTF8(W_To_A(db_name)).c_str()), (void*)info._pswd) == PSM_ERROR)
	{
		psmCloseConnect();
		return false;
	}

	// ��ʼ����ʼ�������ݱ�
	return psmPswdDataInit(&pswd_table) == PSM_OK;
}

bool AdminUser::signup()
{
	valid_accounts = Account::getValidAccounts();
	if(!fs::exists("./db/admin/admin.db"))
		if (psmInitConnect((char*)(ANSIToUTF8(W_To_A(db_name)).c_str()), (void*)info._pswd) == PSM_ERROR)
		{
			psmCloseConnect();
			return false;
		}
	psmCloseConnect();
	return true;
}

bool AdminUser::update(PsmPswdData& upd)
{
	// ���¹���ֻ���ڲ��������˻�
	/*if (!valid_accounts[A_To_W(std::string(upd._account))])
	{
		MessageBoxA(NULL, "�Ѵ�������û���", "ע��", NULL);
		return false;
	}*/
	// ���±�������
	strcpy(upd._note, upd._account);
	pswd_table->updData(pswd_table, &current, &upd);
	// ���µ�ǰ����
	memcpy(&current, &upd, sizeof(PsmPswdData));
	return true;
}

bool AdminUser::set(PsmPswdData& upd)
{
	PsmPswdData* res;
	int cnt;
	pswd_table->srchData(pswd_table, {}, &upd, (void**) & res, &cnt, PsmDataSrch::FULL_MATCH);
	// ���õ�ǰ�����û�
	memcpy(&current, res, sizeof(PsmPswdData));
	memcpy(&upd, res, sizeof(PsmPswdData));
	free(res);
	return true;
}

bool AdminUser::del(PsmPswdData& upd)
{
	return true;
}

bool AdminUser::add(PsmPswdData& info)
{
	// ������������
	pswd_table->insData(pswd_table, NULL, &info);
	return true;
}

AdminUser::AdminUser()
{
	db_name = L"./db/admin/admin.db";
	strcpy(info._account, "admin");
	strcpy(info._pswd, "admin");
}

AdminUser::~AdminUser()
{
}
