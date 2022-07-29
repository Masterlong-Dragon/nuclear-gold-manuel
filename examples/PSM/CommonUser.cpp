#include "CommonUser.h"
#include "AdminUser.h"
#include "psm_core/psm_addrbook_data.h"
#include "psm_core/psm_finance_data.h"
#include "psm_core/psm_fitrecord_data.h"
#include "psm_core/psm_pswd_data.h"
#include "psm_core/psm_schedule_data.h"
#include "code_helper.h"
#include <unordered_map>
#include <filesystem>
namespace fs = std::filesystem;

bool CommonUser::login()
{
	if (info._account[0] == '\0' || info._pswd[0] == '\0')
		return false;
	if (!valid_accounts[A_To_W(UTF8ToANSI(std::string(info._account)))])
		return false;
	if (psmInitConnect((char*)(ANSIToUTF8(W_To_A(db_name)).c_str()), (void*)info._pswd) == PSM_ERROR)
	{
		psmCloseConnect();
		return false;
	}

	// ��ʼ����ʼ�������ݱ�
	return psmScheduleDataInit(&schedule_table) == PSM_OK &&
		psmAddrbookDataInit(&addrbook_table) == PSM_OK &&
		psmFinanceDataInit(&finance_table) == PSM_OK &&
		psmFitrecordDataInit(&fit_table) == PSM_OK &&
		psmPswdDataInit(&pswd_table) == PSM_OK;
}

// ʹ��֮��Ĭ�Ϲر����ݿ����ӣ���ע��
bool CommonUser::signup()
{
	if (info._account[0] == '\0' || info._pswd[0] == '\0')
		return false;
	if (valid_accounts[A_To_W(UTF8ToANSI(std::string(info._account)))])
		return false;
	if (psmInitConnect((char*)(ANSIToUTF8(W_To_A(db_name)).c_str()), (void*)info._pswd) == PSM_ERROR)
		return false;
	psmCloseConnect();
	valid_accounts = Account::getValidAccounts();
	// ����Ŀ¼�²���
	AdminUser* admin = AdminUser::getInstance();
	admin->signup();
	if (admin->login())
	{
		// ������Ŀ
		admin->add(info);
	}
	psmCloseConnect();
	return true;
}

// ��������
// ʹ��֮��Ĭ�Ϲر����ݿ����ӣ���ע��
bool CommonUser::update(PsmPswdData& upd)
{
	if (upd._account[0] == '\0' || upd._pswd[0] == '\0')
		return false;
	// ���µ�ǰ���ݿ�����
	std::wstring  new_db_name = db_name;
	if (strcmp(info._account, upd._account))
	{
		// �رյ�ǰ����
		psmCloseConnect();
		new_db_name = A_To_W(UTF8ToANSI("./db/" + std::string(upd._account) + ".db"));
		// ���������ݿ��ļ�
		// rename((char*)(ANSIToUTF8(W_To_A(db_name)).c_str()), (char*)(ANSIToUTF8(W_To_A(new_db_name)).c_str()));
		fs::rename(fs::u8path(ANSIToUTF8(W_To_A(db_name))), fs::u8path(ANSIToUTF8(W_To_A(new_db_name))));
	}
	// ���µ�ǰ���ݿ�����
	if (strcmp(info._pswd, upd._pswd))
	{
		// �رյ�ǰ����
		psmCloseConnect();
#ifdef SQLITE_ENABLED
		// ����sqlite
		psmInitSqliteBinding();
		// �������ݿ�
		if (psmInitConnect((char*)(ANSIToUTF8(W_To_A(db_name)).c_str()), (void*)info._pswd) == PSM_ERROR)
		{
			psmCloseConnect();
			return false;
		}
		sqlite3* db;
		psmGetDb(&db);
		// ��������
		sqlite3_rekey(db, (void*)upd._pswd, -1);
		// �ر����ݿ�
		psmCloseConnect();
		// ��������
#endif // SQLITE_ENABLED
	}
	// ���³������ݿ�����
	// ����Ŀ¼�²���
	AdminUser* admin = AdminUser::getInstance();
	admin->signup();
	if (admin->login())
	{
		// ������Ŀ
		admin->set(info);
		admin->update(upd);
	}
	psmCloseConnect();
	// ��ȫ������ɸ���
	memcpy(&info, &upd, sizeof(PsmPswdData));
	return true;
}

// ��������
bool CommonUser::set(PsmPswdData& upd)
{
	if (upd._account[0] == '\0' || upd._pswd[0] == '\0')
		return false;
	memcpy(&info, &upd, sizeof(PsmPswdData));
	// ���ö�Ӧ���ݿ�����
	db_name = A_To_W(UTF8ToANSI("./db/" + std::string(upd._account) + ".db"));
	return true;
}

PsmPswdData CommonUser::get()
{
	return info;
}


CommonUser::CommonUser()
{

}

CommonUser::~CommonUser()
{
}
