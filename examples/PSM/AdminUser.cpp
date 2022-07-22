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

	// 初始化开始连接数据表
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
	// 更新功能只用于操作本地账户
	/*if (!valid_accounts[A_To_W(std::string(upd._account))])
	{
		MessageBoxA(NULL, "已存在相关用户名", "注意", NULL);
		return false;
	}*/
	// 更新表中数据
	strcpy(upd._note, upd._account);
	pswd_table->updData(pswd_table, &current, &upd);
	// 更新当前数据
	memcpy(&current, &upd, sizeof(PsmPswdData));
	return true;
}

bool AdminUser::set(PsmPswdData& upd)
{
	PsmPswdData* res;
	int cnt;
	pswd_table->srchData(pswd_table, {}, &upd, (void**) & res, &cnt, PsmDataSrch::FULL_MATCH);
	// 设置当前操作用户
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
	// 插入新增内容
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
