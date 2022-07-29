#include "Account.h"
#include "code_helper.h"
#include <filesystem>
#include <iostream>
using namespace std;
namespace fs = std::filesystem;

// 直接遍历当前db目录 数据库的名称为所求
std::unordered_map<std::wstring, bool> Account::getValidAccounts()
{
	std::unordered_map<std::wstring, bool> dbs;

	auto testdir = fs::path("./db/");

	if (!fs::exists(testdir))
	{
		fs::create_directory("./db/");
	}

	// none	（默认）跳过符号链接，权限拒绝是错误。
	fs::directory_options opt(fs::directory_options::none);

	fs::directory_entry dir(testdir);
	// 遍历当前目录
	for (fs::directory_entry const& entry : fs::directory_iterator(testdir, opt))
	{
		if (entry.is_regular_file())
		{
			string ss = entry.path().filename().string();
			size_t spos = ss.length() - 3;
			if (ss.substr(spos, 3) == ".db")
				dbs.emplace(A_To_W(ss.replace(spos, 3, "")), true);
		}
	}

	return dbs;
}

Account::Account()
{
}

Account::~Account()
{
}
