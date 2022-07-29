#include "framework.h"
#include "LoginForm.h"
#include "MainForm.h"
#include "ui_components/comboex/CheckCombo.h"
#include "ui_components/comboex/FilterCombo.h"
#include "ui_components/windows_manager/windows_manager.h"
#include "CommonUser.h"
#include "AdminUser.h"
#include "code_helper.h"
#include <string>

const std::wstring LoginForm::kClassName = L"Login";
bool is_Exit = true;
PsmPswdData temp_data;
bool signup_con[5];
// 忘记密码
PsmPswdData acc_info;
inline void rstPswdTemp(PsmPswdData& temp)
{
	memset(&temp_data, 0, sizeof(PsmPswdData));
}


LoginForm::LoginForm()
{
}

LoginForm::~LoginForm()
{
}

std::wstring LoginForm::GetSkinFolder()
{
	return L"main";
}

std::wstring LoginForm::GetSkinFile()
{
	return L"login.xml";
}

std::wstring LoginForm::GetWindowClassName() const
{
	return kClassName;
}

std::wstring LoginForm::GetWindowId() const
{
	return L"login";
}

void LoginForm::InitWindow()
{
	common_user = CommonUser::getInstance();
	common_user->valid_accounts = Account::getValidAccounts();
	// 获取参与交互的重要控件
	login_panel = static_cast<ui::Box*>(FindControl(L"login_panel"));
	signup_panel = static_cast<ui::Box*>(FindControl(L"signup_panel"));
	// 主界面
	login_btn = static_cast<ui::Button*>(FindControl(L"login"));// 登录
	account_edit = static_cast<ui::RichEdit*>(FindControl(L"account"));// 账号
	pswd_edit = static_cast<ui::RichEdit*>(FindControl(L"pswd"));// 密码
	forget_pswd_edit = static_cast<ui::Label*>(FindControl(L"forget"));// 忘记密码
	// 注册界面
	account_signup_edit = static_cast<ui::RichEdit*>(FindControl(L"account_signup"));// 账户注册输入框
	pswd_signup_edit = static_cast<ui::RichEdit*>(FindControl(L"pswd_signup"));// 密码注册输入框
	pswd_signup_confirm_edit = static_cast<ui::RichEdit*>(FindControl(L"pswd_confirm"));// 确认密码输入框
	secure_ques_edit = static_cast<ui::RichEdit*>(FindControl(L"secure_question"));// 密保问题输入框
	secure_ans_edit = static_cast<ui::RichEdit*>(FindControl(L"secure_answer"));// 密保答案输入框
	account_signup_btn = static_cast<ui::Button*>(FindControl(L"signup_account"));// 账号注册按钮
	// 找回密码界面
	account_combo = static_cast<ui::Combo*>(FindControl(L"accounts"));
	secure_ans_verify_edit = static_cast<ui::RichEdit*>(FindControl(L"secure_answer_verify"));
	secure_question_info = static_cast<ui::Label*>(FindControl(L"secure_question_verify"));
	pswd_found_info = static_cast<ui::Label*>(FindControl(L"pswd_found"));
	check_verify_btn = static_cast<ui::Button*>(FindControl(L"check_verify"));

	// 查找忘记密码
	check_verify_btn->AttachClick([&](ui::EventArgs* arg) {
		if (account_combo->GetText().empty())
			return true;
		if (acc_info._account[0] == '\0')
			return true;
		std::wstring ans = secure_ans_verify_edit->GetText();
		if (ans.empty())
			return true;
		std::wstring rans = A_To_W(UTF8ToANSI(std::string(acc_info._secure_answer)));
		if (!strcmp((char*)ans.c_str(), (char*)rans.c_str()))
			pswd_found_info->SetText(A_To_W(UTF8ToANSI(std::string(acc_info._pswd))));
		return true;
		});

	account_combo->AttachSelect([&](ui::EventArgs* arg){
		// 获得操作账户名
		std::wstring forget_name = account_combo->GetText();
		psmCloseConnect();
		AdminUser* admin = AdminUser::getInstance();
		// 登入管理员账户
		admin->signup();
		admin->login();
		memset(&acc_info, NULL, sizeof(PsmPswdData));
		// 使用账号名作为唯一索引
		strcpy(acc_info._account, (char*)ANSIToUTF8(W_To_A(forget_name)).c_str());
		admin->set(acc_info);
		// 显示密保问题
		secure_question_info->SetText(A_To_W(UTF8ToANSI(std::string(acc_info._secure_question))));
		return true;
		});
	// 忘记密码逻辑
	// 更新内容
	forget_pswd_edit->AttachButtonUp([&](ui::EventArgs* arg) {
		// 清空内容
		secure_question_info->SetText(L"");
		pswd_found_info->SetText(L"");
		memset(&acc_info, NULL, sizeof(PsmPswdData));
		secure_ans_verify_edit->SetText(L"");
		auto accounts = Account::getValidAccounts();
		account_combo->RemoveAll();
		// 插入有效内容
		for (auto& wstr : accounts)
		{
			ui::ListContainerElement* element = new ui::ListContainerElement;
			element->SetClass(L"listitem");
			element->SetFixedHeight(30);
			element->SetBkColor(L"white");
			element->SetTextPadding({ 6,0,6,0 });
			element->SetText(wstr.first);
			account_combo->Add(element);
		}
		return true;
		});
	// 设置登录事件逻辑
	login_btn->AttachClick([&](ui::EventArgs* arg) {
		// 登录响应
		// 一个temp管理数据
		rstPswdTemp(temp_data);
		strcpy(temp_data._account, ANSIToUTF8(W_To_A(account_edit->GetText())).c_str());
		strcpy(temp_data._pswd, ANSIToUTF8(W_To_A(pswd_edit->GetText())).c_str());
		common_user->set(temp_data);
		// 尝试登录
		if (common_user->login())
		{
			// 向窗口管理器获取主窗体
			// 展示主窗体
			MainForm* main_window = nim_comp::WindowsManager::SingletonShow<MainForm>(MainForm::kClassName);
			main_window->CenterWindow();
			main_window->ShowWindow();
			// 并非退出程序
			is_Exit = false;
			// 调用Close
			Close();
		}
		else
		{
			MessageBoxA(NULL, "账号密码错误！", "警告", NULL);
		}
		return true;
		});
	// 设置注册事件逻辑
	account_signup_btn->AttachClick([&](ui::EventArgs* arg) {
		// 输入是否合法
		for (int i = 0; i < 5; i++)
			if (!signup_con[i])
				return true;
		// 重置temp数据
		rstPswdTemp(temp_data);
		strcpy(temp_data._account, ANSIToUTF8(W_To_A(account_signup_edit->GetText())).c_str());
		strcpy(temp_data._pswd, ANSIToUTF8(W_To_A(pswd_signup_edit->GetText())).c_str());
		strcpy(temp_data._secure_question, ANSIToUTF8(W_To_A(secure_ques_edit->GetText())).c_str());
		strcpy(temp_data._secure_answer, ANSIToUTF8(W_To_A(secure_ans_edit->GetText())).c_str());
		strcpy(temp_data._note, temp_data._account);
		common_user->set(temp_data);
		// 注册
		if (!common_user->signup())
		{
			MessageBoxA(NULL, "该账号可能已被注册", "注意", NULL);
			return true;
		}
		MessageBoxA(NULL, "成功注册", "恭喜", NULL);
		// 更新账号数据
		return true;
		});
	// 判断输入内容是否合法的效果
	initPsmEditAction(account_signup_edit, [=]() {return signup_con[0] = isLengthFine(account_signup_edit, INPUT_LEN_MIN, ACCOUNT_LEN_MAX); });
	initPsmEditAction(pswd_signup_edit, [=]() {return  signup_con[1] = isLengthFine(pswd_signup_edit, PSWD_LEN_MIN, PSWD_LEN_MAX); });
	// initPsmEditAction(pswd_signup_confirm_edit, PSWD_LEN_MIN, PSWD_LEN_MAX);
	// 确认前后密码是否相同
	initPsmEditAction(pswd_signup_confirm_edit, [&]() {
		return signup_con[2] = (isLengthFine(pswd_signup_confirm_edit, PSWD_LEN_MIN, PSWD_LEN_MAX)
			&& pswd_signup_confirm_edit->GetText() == pswd_signup_edit->GetText());
		});
	initPsmEditAction(secure_ques_edit, [=]() {return signup_con[3] = isLengthFine(secure_ques_edit, INPUT_LEN_MIN, S_QUESTION_LEN_MAX); });
	initPsmEditAction(secure_ans_edit, [=]() {return signup_con[4] = isLengthFine(secure_ans_edit, INPUT_LEN_MIN, S_QUESTION_LEN_MAX); });
}

LRESULT LoginForm::OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	// 判断是登陆页面退出还是切换页面
	if (is_Exit)
		PostQuitMessage(0L);// 发送退出消息
	return __super::OnClose(uMsg, wParam, lParam, bHandled);
}
