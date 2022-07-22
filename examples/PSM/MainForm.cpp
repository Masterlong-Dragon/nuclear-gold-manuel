// 谨记 2022/7/17晚22:30至23:16
// 开发者因为误删一段include且误操作致使IDE自动保存&关闭
// 导致这坨乱麻一般的包含关系成为了真正的乱麻
// 经过排列组合&乱改其它文件的包含关系如此一段时间才偶然发现了另一种可以正常编译的模式
#include "EditView.h"
#include "MainForm.h"
#include "code_helper.h"
#include "psm_core/psm_schedule_data.h"
#include "psm_core/psm_addrbook_data.h"
#include "psm_core/psm_finance_data.h"
#include "psm_core/psm_fitrecord_data.h"
#include "psm_core/psm_pswd_data.h"
#include <regex>
#include "AdminUser.h"
#include "CommonUser.h"
const std::wstring MainForm::kClassName = L"Main";
bool upd_con[4] = { 0 };
EditView<PsmScheduleData>* s_view;
EditView<PsmAddrbookData>* a_view;
EditView<PsmFinanceData>* fnc_view;
EditView<PsmFitrecordData>* fitr_view;
EditView<PsmPswdData>* p_view;
// 账户
PsmPswdData current_data;
ui::RichEdit* acc_e;
ui::RichEdit* pw_e;
ui::RichEdit* scq_e;
ui::RichEdit* sca_e;
// 默认设置输入框
inline void initInfoEdit(ui::RichEdit* e)
{
	initPsmEditAction(e, [=]() {return true; });
}
// 设置当前账号信息
inline void SetAccountInfo()
{
	if (current_data._account[0] == '\0')
	{
		// 退出账户
		psmCloseConnect();
		// 当前账户
		CommonUser* user = CommonUser::getInstance();
		// 登录管理员
		AdminUser* admin = AdminUser::getInstance();
		admin->signup();
		if (admin->login())
		{
			current_data = user->get();
			admin->set(current_data);
		}
		// 退出账户
		psmCloseConnect();
		user->login();
	}
		acc_e->SetText(A_To_W(UTF8ToANSI(std::string(current_data._account))));
		pw_e->SetText(A_To_W(UTF8ToANSI(std::string(current_data._pswd))));
		scq_e->SetText(A_To_W(UTF8ToANSI(std::string(current_data._secure_question))));
		sca_e->SetText(A_To_W(UTF8ToANSI(std::string(current_data._secure_answer))));
}
MainForm::MainForm()
{

}


MainForm::~MainForm()
{
}

std::wstring MainForm::GetSkinFolder()
{
	return L"main";
}

std::wstring MainForm::GetSkinFile()
{
	return L"main.xml";
}

std::wstring MainForm::GetWindowClassName() const
{
	return kClassName;
}

std::wstring MainForm::GetWindowId() const
{
	return L"main";
}

ui::Control* MainForm::CreateControl(const std::wstring& pstrClass)
{
	if (pstrClass == L"VirtualTileBox")
	{
		return new VirtualTileBox();
	}
	// 扫描 XML 发现有名称为 CefControl 的节点，则创建一个 ui::CefControl 控件
	if (pstrClass == L"CefControl")
	{
		if (nim_comp::CefManager::GetInstance()->IsEnableOffsetRender())
			return new nim_comp::CefControl;
		else
			return new nim_comp::CefNativeControl;
	}
	return nullptr;
}

void MainForm::InitWindow()
{
	// 做一些初始化工作
	// 不同界面的容器
	ui::Box* main_container = static_cast<ui::Box*>(FindControl(L"main_container"));
	ui::Box* account_container = static_cast<ui::Box*>(FindControl(L"account_container"));
	ui::Box* schedule_container = static_cast<ui::Box*>(FindControl(L"schedule_container"));
	ui::Box* addrbook_container = static_cast<ui::Box*>(FindControl(L"addrbook_container"));
	ui::Box* finance_container = static_cast<ui::Box*>(FindControl(L"finance_container"));
	ui::Box* fit_container = static_cast<ui::Box*>(FindControl(L"fit_container"));
	ui::Box* pswd_container = static_cast<ui::Box*>(FindControl(L"pswd_container"));

	// 主界面
	main_view = new ui::Box;
	// 账户
	account_view = new ui::Box;
	// 日程
	schedule_view = new ui::Box;
	// 通讯录
	addrbook_view = new ui::Box;
	// 记账
	finance_view = new ui::Box;
	// 健身
	fit_view = new ui::Box;
	// 密码
	pswd_view = new ui::Box;
	// 加载页面
	// ui::GlobalManager::FillBox(main_view, L"main/views/main_view.xml");
	ui::GlobalManager::FillBox(account_view, L"main/views/account_view.xml");
	ui::GlobalManager::FillBox(schedule_view, L"main/views/schedule_view.xml");
	ui::GlobalManager::FillBox(addrbook_view, L"main/views/addrbook_view.xml");
	ui::GlobalManager::FillBox(finance_view, L"main/views/finance_view.xml");
	ui::GlobalManager::FillBox(fit_view, L"main/views/fit_view.xml");
	ui::GlobalManager::FillBox(pswd_view, L"main/views/pswd_view.xml");
	// 装载页面
	// main_container->Add(main_view);
	account_container->Add(account_view);
	schedule_container->Add(schedule_view);
	addrbook_container->Add(addrbook_view);
	finance_container->Add(finance_view);
	fit_container->Add(fit_view);
	pswd_container->Add(pswd_view);

	// 加载作为网页的主页
	cef_control = dynamic_cast<nim_comp::CefControlBase*>(FindControl(L"cef_control"));
	cef_control ->LoadURL(nbase::win32::GetCurrentModuleDirectory() + L"homepage.html");
	
	// 个人账户的页面
	acc_e = static_cast<ui::RichEdit*>(account_view->FindSubControl(L"account_signup"));
	pw_e = static_cast<ui::RichEdit*>(account_view->FindSubControl(L"pswd_signup"));
	scq_e = static_cast<ui::RichEdit*>(account_view->FindSubControl(L"secure_question"));
	sca_e = static_cast<ui::RichEdit*>(account_view->FindSubControl(L"secure_answer"));
	ui::Button* upac_b = static_cast<ui::Button*>(account_view->FindSubControl(L"upd_account"));
	// 判断输入内容是否合法的效果
	initPsmEditAction(acc_e, [=]() {return upd_con[0] = isLengthFine(acc_e, INPUT_LEN_MIN, ACCOUNT_LEN_MAX); });
	initPsmEditAction(pw_e, [=]() {return  upd_con[1] = isLengthFine(pw_e, PSWD_LEN_MIN, PSWD_LEN_MAX); });
	initPsmEditAction(scq_e, [=]() {return upd_con[2] = isLengthFine(scq_e, INPUT_LEN_MIN, S_QUESTION_LEN_MAX); });
	initPsmEditAction(sca_e, [=]() {return upd_con[3] = isLengthFine(sca_e, INPUT_LEN_MIN, S_QUESTION_LEN_MAX); });
	// acc主页
	ui::OptionBox* acc_op = static_cast<ui::OptionBox*>(FindControl(L"account"));
	// 取消选中 则清除原有内容
	acc_op->AttachUnSelect([&](ui::EventArgs* arg) {
		SetAccountInfo();
		return true;
		});
	// 更新账号信息的操作
	upac_b->AttachClick([&](ui::EventArgs* arg) {
		// 检查内容合法
		for (auto& b : upd_con)
			if (!b)
				return true;
		// 当前账户
		CommonUser* user = CommonUser::getInstance();
		PsmPswdData acc_new;
		// 获得输入内容
		strcpy(acc_new._account, (char*)ANSIToUTF8(W_To_A(acc_e->GetText())).c_str());
		strcpy(acc_new._pswd, (char*)ANSIToUTF8(W_To_A(pw_e->GetText())).c_str());
		strcpy(acc_new._secure_question, (char*)ANSIToUTF8(W_To_A(scq_e->GetText())).c_str());
		strcpy(acc_new._secure_answer, (char*)ANSIToUTF8(W_To_A(sca_e->GetText())).c_str());
		user->update(acc_new);
		// 更新current
		memset(&current_data, NULL, sizeof(PsmPswdData));
		// 重新连接数据库
		user->login();
		return true;
		});
	SetAccountInfo();// 初始化内容

	// 窗体管理每个具体管理页面的元素和功能实现
	s_view = new EditView<PsmScheduleData>(schedule_container, (char*)"Id",
		{ L"schedule_name", L"schedule_time", L"schedule_addr" });
	// 初始化对应数据表
	psmScheduleDataInit(&s_view->table);
	// 绑定对应事件
	// 创建视觉元素的内容
	s_view->SetProviderAddAction([&](Item<PsmScheduleData>* temp, PsmScheduleData& data) {
		temp->label_tittle_->SetText(A_To_W(UTF8ToANSI(std::string(data._name))));
		temp->label_info_->SetText(A_To_W(UTF8ToANSI(std::string(data._time))));
		temp->control_img_->SetBkImage(L"icons/schedule.png");
		})
		// 创建新增项目的同步响应
			->SetDataAddAction([&]() {
			std::wstring name = s_view->info_edits[L"schedule_name"]->GetText();
			std::wstring time = s_view->info_edits[L"schedule_time"]->GetText();
			std::wstring addr = s_view->info_edits[L"schedule_addr"]->GetText();
			PsmScheduleData data = {0};
			strcpy(data._name, (char*)ANSIToUTF8(W_To_A(name)).c_str());
			strcpy(data._time, (char*)ANSIToUTF8(W_To_A(time)).c_str());
			strcpy(data._address, (char*)ANSIToUTF8(W_To_A(addr)).c_str());
			return data;
				})
			// 创建显示项目的对应操作
					->SetDataShowAction([&](PsmScheduleData& data) {
					s_view->info_edits[L"schedule_name"]->SetText(A_To_W(UTF8ToANSI(std::string(data._name))));
					s_view->info_edits[L"schedule_time"]->SetText(A_To_W(UTF8ToANSI(std::string(data._time))));
					s_view->info_edits[L"schedule_addr"]->SetText(A_To_W(UTF8ToANSI(std::string(data._address))));
						})
					//设置搜索使用索引
							->SetSrchTextAction([&](PsmScheduleData& data, std::wstring text) {
							strcpy(data._name, (char*)ANSIToUTF8(W_To_A(text)).c_str());
								})
							->Init([&]() {
									for (auto& e : s_view->info_edits)
										initPsmEditAction(e.second, [=]() {return upd_con[0] = isLengthFine(e.second, 0, PSM_MAX_SCHEDULE_DATA_COUNT); });
								});

	// 窗体管理每个具体管理页面的元素和功能实现
	a_view = new EditView<PsmAddrbookData>(addrbook_container, (char*)"name",
		{ L"addr_name", L"addr_phone", L"addr_age" , L"addr_email", L"addr_addr"});
	// 初始化对应数据表
	psmAddrbookDataInit(&a_view->table);
	// 绑定对应事件
	// 创建视觉元素的内容
	a_view->SetProviderAddAction([&](Item<PsmAddrbookData>* temp, PsmAddrbookData& data) {
		temp->label_tittle_->SetText(A_To_W(UTF8ToANSI(std::string(data._name))));
		temp->label_info_->SetText(A_To_W(UTF8ToANSI(std::string(data._phone))));
		temp->control_img_->SetBkImage(L"icons/addrbook.png");
		})
		// 创建新增项目的同步响应
			->SetDataAddAction([&]() {
			std::wstring name = a_view->info_edits[L"addr_name"]->GetText();
			std::wstring phone = a_view->info_edits[L"addr_phone"]->GetText();
			std::wstring age = a_view->info_edits[L"addr_age"]->GetText();
			std::wstring email = a_view->info_edits[L"addr_email"]->GetText();
			std::wstring addr = a_view->info_edits[L"addr_addr"]->GetText();
			PsmAddrbookData data = {0};
			strcpy(data._name, (char*)ANSIToUTF8(W_To_A(name)).c_str());
			strcpy(data._phone, (char*)ANSIToUTF8(W_To_A(phone)).c_str());
			if (age.empty())
				data._age = -1;
			else 
				data._age = std::stoi((W_To_A(age)));
			strcpy(data._email, (char*)ANSIToUTF8(W_To_A(email)).c_str());
			strcpy(data._address, (char*)ANSIToUTF8(W_To_A(addr)).c_str());
			return data;
				})
			// 创建显示项目的对应操作
					->SetDataShowAction([&](PsmAddrbookData& data) {
					a_view->info_edits[L"addr_name"]->SetText(A_To_W(UTF8ToANSI(std::string(data._name))));
					a_view->info_edits[L"addr_phone"]->SetText(A_To_W(UTF8ToANSI(std::string(data._phone))));
					a_view->info_edits[L"addr_age"]->SetText(A_To_W(UTF8ToANSI(std::to_string(data._age))));
					a_view->info_edits[L"addr_email"]->SetText(A_To_W(UTF8ToANSI(std::string(data._email))));
					a_view->info_edits[L"addr_addr"]->SetText(A_To_W(UTF8ToANSI(std::string(data._address))));
						})
					//设置搜索使用索引
							->SetSrchTextAction([&](PsmAddrbookData& data, std::wstring text) {
							strcpy(data._name, (char*)ANSIToUTF8(W_To_A(text)).c_str());
							data._age = -1;
								})
							->Init([&]() {
									initPsmEditAction(a_view->info_edits[L"addr_name"], [=]() { return isLengthFine(a_view->info_edits[L"addr_name"], 0, PSM_MAX_ADDRBOOK_DATA_COUNT);});
									initPsmEditAction(a_view->info_edits[L"addr_addr"], [=]() { return isLengthFine(a_view->info_edits[L"addr_addr"], 0, PSM_MAX_ADDRBOOK_DATA_COUNT); });
									// 年龄
									initPsmEditAction(a_view->info_edits[L"addr_age"], [&]() {
										std::wregex n_regex(L"^[0-9]*[1-9][0-9]*$");
										std::wstring str = a_view->info_edits[L"addr_age"]->GetText();
										return (str.empty() || std::regex_match(str, n_regex)) && isLengthFine(a_view->info_edits[L"addr_age"], 0, PSM_MAX_ADDRBOOK_DATA_COUNT);
										});
									// 电话号码正则匹配
									initPsmEditAction(a_view->info_edits[L"addr_phone"], [&]() {
										std::wregex p_regex(L"(^1[3|4|5|7|8][0-9]\\d{8}$)");
										std::wstring str = a_view->info_edits[L"addr_phone"]->GetText();
										return (str.empty() || std::regex_match(str, p_regex)) && isLengthFine(a_view->info_edits[L"addr_phone"], 0, PSM_MAX_ADDRBOOK_DATA_COUNT);
										});
									// 对邮箱号码正则匹配
									initPsmEditAction(a_view->info_edits[L"addr_email"], [&]() {
										std::wregex e_regex(L"([a-zA-Z0-9_-])+@([a-zA-Z0-9_-])+((\\.[a-zA-Z0-9_-]{2,3}){1,2})");
										std::wstring str = a_view->info_edits[L"addr_email"]->GetText();
										return (str.empty() || std::regex_match(str, e_regex)) && isLengthFine(a_view->info_edits[L"addr_email"], 0, PSM_MAX_ADDRBOOK_DATA_COUNT);
										});
								});



// 窗体管理每个具体管理页面的元素和功能实现
	fnc_view = new EditView<PsmFinanceData>(finance_container, (char*)"Id",
		{ L"finance_type", L"finance_amount", L"finance_name" , L"finance_date", L"finance_note"});
	// 初始化对应数据表
	psmFinanceDataInit(&fnc_view->table);
	// 绑定对应事件
	// 创建视觉元素的内容
	fnc_view->SetProviderAddAction([&](Item<PsmFinanceData>* temp, PsmFinanceData& data) {
		temp->label_tittle_->SetText(A_To_W(nbase::StringPrintf("%.2lf", data._amount)));
		temp->label_info_->SetText((data._type == PsmFinanceDataType::PSM_FINANCE_DATA_TYPE_INCOME)? L"收入" : L"支出");
		temp->control_img_->SetBkImage(L"icons/finance.png");
		})
		// 创建新增项目的同步响应
			->SetDataAddAction([&]() {
			std::wstring type = fnc_view->info_edits[L"finance_type"]->GetText();
			std::wstring amount = fnc_view->info_edits[L"finance_amount"]->GetText();
			std::wstring name = fnc_view->info_edits[L"finance_name"]->GetText();
			std::wstring date = fnc_view->info_edits[L"finance_date"]->GetText();
			std::wstring note = fnc_view->info_edits[L"finance_note"]->GetText();
			PsmFinanceData data = {};
			if (type == L"支出")
				data._type = PsmFinanceDataType::PSM_FINANCE_DATA_TYPE_EXPENSE;
			else
				data._type = PsmFinanceDataType::PSM_FINANCE_DATA_TYPE_INCOME;
			if (amount.empty())
				data._amount = 0;
			else
				data._amount = std::stod(ANSIToUTF8(W_To_A(amount)));
			strcpy(data._name, (char*)ANSIToUTF8(W_To_A(name)).c_str());
			strcpy(data._date, (char*)ANSIToUTF8(W_To_A(date)).c_str());
			strcpy(data._note, (char*)ANSIToUTF8(W_To_A(note)).c_str());
			return data;
				})
			// 创建显示项目的对应操作
					->SetDataShowAction([&](PsmFinanceData& data) {
					fnc_view->info_edits[L"finance_type"]->SetText((data._type == PsmFinanceDataType::PSM_FINANCE_DATA_TYPE_INCOME) 
						? L"收入" : L"支出");
					// 补丁 只显示小数点后两位
					fnc_view->info_edits[L"finance_amount"]->SetText(A_To_W(nbase::StringPrintf("%.2lf", data._amount)));
					fnc_view->info_edits[L"finance_name"]->SetText(A_To_W(UTF8ToANSI(std::string(data._name))));
					fnc_view->info_edits[L"finance_date"]->SetText(A_To_W(UTF8ToANSI(std::string(data._date))));
					fnc_view->info_edits[L"finance_note"]->SetText(A_To_W(UTF8ToANSI(std::string(data._note))));
						})
					//设置搜索使用索引
							->SetSrchTextAction([&](PsmFinanceData& data, std::wstring text) {
							strcpy(data._name, (char*)ANSIToUTF8(W_To_A(text)).c_str());
								})
							->Init([&]() {
									// 收入和支出
									initPsmEditAction(fnc_view->info_edits[L"finance_type"], [&]() {
										std::wstring st = fnc_view->info_edits[L"finance_type"]->GetText();
										return st == L"收入" || st == L"支出";
										});
									// 金额 
									initPsmEditAction(fnc_view->info_edits[L"finance_amount"], [&]() {
										std::wregex a_regex(L"^\\d+(\\.\\d+)?$");
										std::wstring sa = fnc_view->info_edits[L"finance_amount"]->GetText();
										return (sa.empty() || std::regex_match(sa, a_regex)) && isLengthFine(fnc_view->info_edits[L"finance_amount"], 0, PSM_MAX_FINANCE_LENGTH);
										});
									initPsmEditAction(fnc_view->info_edits[L"finance_name"], [&]() {return isLengthFine(fnc_view->info_edits[L"finance_name"], 0, PSM_MAX_FINANCE_LENGTH); });
									initPsmEditAction(fnc_view->info_edits[L"finance_date"], [&]() {return isLengthFine(fnc_view->info_edits[L"finance_date"], 0, PSM_MAX_FINANCE_LENGTH); });
									initPsmEditAction(fnc_view->info_edits[L"finance_note"], [&]() {return isLengthFine(fnc_view->info_edits[L"finance_note"], 0, PSM_MAX_FINANCE_LENGTH); });
								});

// 窗体管理每个具体管理页面的元素和功能实现
	fitr_view = new EditView<PsmFitrecordData>(fit_container, (char*)"Id",
		{ L"fit_note", L"fit_date", L"fit_duration", L"fit_intensity", L"fit_type"});
	// 初始化对应数据表
	psmFitrecordDataInit(&fitr_view->table);
	// 绑定对应事件
	// 创建视觉元素的内容
	fitr_view->SetProviderAddAction([&](Item<PsmFitrecordData>* temp, PsmFitrecordData& data) {
		temp->label_tittle_->SetText(A_To_W(UTF8ToANSI(std::string(data._type))));
		temp->label_info_->SetText(A_To_W(UTF8ToANSI(std::string(data._date))));
		temp->control_img_->SetBkImage(L"icons/fit.png");
		})
		// 创建新增项目的同步响应
			->SetDataAddAction([&]() {
			std::wstring note = fitr_view->info_edits[L"fit_note"]->GetText();
			std::wstring date = fitr_view->info_edits[L"fit_date"]->GetText();
			std::wstring duration = fitr_view->info_edits[L"fit_duration"]->GetText();
			std::wstring intensity = fitr_view->info_edits[L"fit_intensity"]->GetText();
			std::wstring type = fitr_view->info_edits[L"fit_type"]->GetText();
			PsmFitrecordData data = {0};
			strcpy(data._note, (char*)ANSIToUTF8(W_To_A(note)).c_str());
			strcpy(data._date, (char*)ANSIToUTF8(W_To_A(date)).c_str());
			strcpy(data._duration, (char*)ANSIToUTF8(W_To_A(duration)).c_str());
			strcpy(data._intensity, (char*)ANSIToUTF8(W_To_A(intensity)).c_str());
			strcpy(data._type, (char*)ANSIToUTF8(W_To_A(type)).c_str());
			return data;
				})
			// 创建显示项目的对应操作
					->SetDataShowAction([&](PsmFitrecordData& data) {
					fitr_view->info_edits[L"fit_note"]->SetText(A_To_W(UTF8ToANSI(std::string(data._note))));
					fitr_view->info_edits[L"fit_date"]->SetText(A_To_W(UTF8ToANSI(std::string(data._date))));
					fitr_view->info_edits[L"fit_duration"]->SetText(A_To_W(UTF8ToANSI(std::string(data._duration))));
					fitr_view->info_edits[L"fit_intensity"]->SetText(A_To_W(UTF8ToANSI(std::string(data._intensity))));
					fitr_view->info_edits[L"fit_type"]->SetText(A_To_W(UTF8ToANSI(std::string(data._type))));
						})
					//设置搜索使用索引
							->SetSrchTextAction([&](PsmFitrecordData& data, std::wstring text) {
							strcpy(data._note, (char*)ANSIToUTF8(W_To_A(text)).c_str());
								})
							->Init([&]() {
									for (auto& e : fitr_view->info_edits)
										initPsmEditAction(e.second, [&]() {return isLengthFine(e.second, 0, PSM_MAX_FITRECORD_DATA_COUNT); });
								});
								
// 窗体管理每个具体管理页面的元素和功能实现
	p_view = new EditView<PsmPswdData>(pswd_container, (char*)"Id",
		{ L"pswd_note", L"pswd_name", L"pswd_pswd", L"pswd_question", L"pswd_answer"});
	// 初始化对应数据表
	psmPswdDataInit(&p_view->table);
	// 绑定对应事件
	// 创建视觉元素的内容
	p_view->SetProviderAddAction([&](Item<PsmPswdData>* temp, PsmPswdData& data) {
		temp->label_tittle_->SetText(A_To_W(UTF8ToANSI(std::string(data._account))));
		temp->label_info_->SetText(A_To_W(UTF8ToANSI(std::string(data._note))));
		temp->control_img_->SetBkImage(L"icons/pswd.png");
		});
		// 创建新增项目的同步响应
			p_view->SetDataAddAction([&]() {
			std::wstring note = p_view->info_edits[L"pswd_note"]->GetText();
			std::wstring name = p_view->info_edits[L"pswd_name"]->GetText();
			std::wstring pswd = p_view->info_edits[L"pswd_pswd"]->GetText();
			std::wstring question = p_view->info_edits[L"pswd_question"]->GetText();
			std::wstring answer = p_view->info_edits[L"pswd_answer"]->GetText();
			PsmPswdData data = {0};
			strcpy(data._note, (char*)ANSIToUTF8(W_To_A(note)).c_str());
			strcpy(data._account, (char*)ANSIToUTF8(W_To_A(name)).c_str());
			strcpy(data._pswd, (char*)ANSIToUTF8(W_To_A(pswd)).c_str());
			strcpy(data._secure_question, (char*)ANSIToUTF8(W_To_A(question)).c_str());
			strcpy(data._secure_answer, (char*)ANSIToUTF8(W_To_A(answer)).c_str());
			return data;
				})
			// 创建显示项目的对应操作
					->SetDataShowAction([&](PsmPswdData& data) {
					p_view->info_edits[L"pswd_note"]->SetText(A_To_W(UTF8ToANSI(std::string(data._note))));
					p_view->info_edits[L"pswd_name"]->SetText(A_To_W(UTF8ToANSI(std::string(data._account))));
					p_view->info_edits[L"pswd_pswd"]->SetText(A_To_W(UTF8ToANSI(std::string(data._pswd))));
					p_view->info_edits[L"pswd_question"]->SetText(A_To_W(UTF8ToANSI(std::string(data._secure_question))));
					p_view->info_edits[L"pswd_answer"]->SetText(A_To_W(UTF8ToANSI(std::string(data._secure_answer))));
						})
					//设置搜索使用索引
							->SetSrchTextAction([&](PsmPswdData& data, std::wstring text) {
							strcpy(data._note, (char*)ANSIToUTF8(W_To_A(text)).c_str());
								})
							->Init([&]() {
									for (auto& e : p_view->info_edits)
										initPsmEditAction(e.second, [&]() {return isLengthFine(e.second, 0, PSM_MAX_PSWD_DATA_COUNT); });
								});




 
}

LRESULT MainForm::OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	PostQuitMessage(0L);
	return __super::OnClose(uMsg, wParam, lParam, bHandled);
}