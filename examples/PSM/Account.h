#pragma once
#include <string>
#include <unordered_map>
#include "psm_core/psm_pswd_data.h"
typedef struct psm_pswd_data PsmPswdData;
class Account
{
public:
	static std::unordered_map<std::wstring, bool> getValidAccounts();
	virtual bool login() = 0;
	virtual bool signup() = 0;
	virtual bool update(PsmPswdData& upd) = 0;
	virtual bool set(PsmPswdData& upd) = 0;
protected:
	Account();
	~Account();
	PsmPswdData info;
};

