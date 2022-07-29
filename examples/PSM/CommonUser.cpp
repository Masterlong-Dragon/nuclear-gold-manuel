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

	// 初始化开始连接数据表
	return psmScheduleDataInit(&schedule_table) == PSM_OK &&
		psmAddrbookDataInit(&addrbook_table) == PSM_OK &&
		psmFinanceDataInit(&finance_table) == PSM_OK &&
		psmFitrecordDataInit(&fit_table) == PSM_OK &&
		psmPswdDataInit(&pswd_table) == PSM_OK;
}

// 使用之后默认关闭数据库连接，请注意
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
	// 超管目录下操作
	AdminUser* admin = AdminUser::getInstance();
	admin->signup();
	if (admin->login())
	{
		// 增加项目
		admin->add(info);
	}
	psmCloseConnect();
	return true;
}

// 更新内容
// 使用之后默认关闭数据库连接，请注意
bool CommonUser::update(PsmPswdData& upd)
{
	if (upd._account[0] == '\0' || upd._pswd[0] == '\0')
		return false;
	// 更新当前数据库名称
	std::wstring  new_db_name = db_name;
	if (strcmp(info._account, upd._account))
	{
		// 关闭当前连接
		psmCloseConnect();
		new_db_name = A_To_W(UTF8ToANSI("./db/" + std::string(upd._account) + ".db"));
		// 重命名数据库文件
		// rename((char*)(ANSIToUTF8(W_To_A(db_name)).c_str()), (char*)(ANSIToUTF8(W_To_A(new_db_name)).c_str()));
		fs::rename(fs::u8path(ANSIToUTF8(W_To_A(db_name))), fs::u8path(ANSIToUTF8(W_To_A(new_db_name))));
	}
	// 更新当前数据库密码
	if (strcmp(info._pswd, upd._pswd))
	{
		// 关闭当前连接
		psmCloseConnect();
#ifdef SQLITE_ENABLED
		// 连接sqlite
		psmInitSqliteBinding();
		// 连接数据库
		if (psmInitConnect((char*)(ANSIToUTF8(W_To_A(db_name)).c_str()), (void*)info._pswd) == PSM_ERROR)
		{
			psmCloseConnect();
			return false;
		}
		sqlite3* db;
		psmGetDb(&db);
		// 重置密码
		sqlite3_rekey(db, (void*)upd._pswd, -1);
		// 关闭数据库
		psmCloseConnect();
		// 更改密码
#endif // SQLITE_ENABLED
	}
	// 更新超管数据库内容
	// 超管目录下操作
	AdminUser* admin = AdminUser::getInstance();
	admin->signup();
	if (admin->login())
	{
		// 更新项目
		admin->set(info);
		admin->update(upd);
	}
	psmCloseConnect();
	// 完全拷贝达成更新
	memcpy(&info, &upd, sizeof(PsmPswdData));
	return true;
}

// 设置内容
bool CommonUser::set(PsmPswdData& upd)
{
	if (upd._account[0] == '\0' || upd._pswd[0] == '\0')
		return false;
	memcpy(&info, &upd, sizeof(PsmPswdData));
	// 设置对应数据库名称
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
