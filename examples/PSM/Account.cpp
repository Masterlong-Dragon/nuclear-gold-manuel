#include "Account.h"
#include "code_helper.h"
#include <filesystem>
#include <iostream>
using namespace std;
namespace fs = std::filesystem;

// ֱ�ӱ�����ǰdbĿ¼ ���ݿ������Ϊ����
std::unordered_map<std::wstring, bool> Account::getValidAccounts()
{
	std::unordered_map<std::wstring, bool> dbs;

	auto testdir = fs::path("./db/");

	if (!fs::exists(testdir))
	{
		fs::create_directory("./db/");
	}

	// none	��Ĭ�ϣ������������ӣ�Ȩ�޾ܾ��Ǵ���
	fs::directory_options opt(fs::directory_options::none);

	fs::directory_entry dir(testdir);
	// ������ǰĿ¼
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
