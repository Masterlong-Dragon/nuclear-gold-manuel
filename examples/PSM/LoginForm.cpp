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
// ��������
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
	// ��ȡ���뽻������Ҫ�ؼ�
	login_panel = static_cast<ui::Box*>(FindControl(L"login_panel"));
	signup_panel = static_cast<ui::Box*>(FindControl(L"signup_panel"));
	// ������
	login_btn = static_cast<ui::Button*>(FindControl(L"login"));// ��¼
	account_edit = static_cast<ui::RichEdit*>(FindControl(L"account"));// �˺�
	pswd_edit = static_cast<ui::RichEdit*>(FindControl(L"pswd"));// ����
	forget_pswd_edit = static_cast<ui::Label*>(FindControl(L"forget"));// ��������
	// ע�����
	account_signup_edit = static_cast<ui::RichEdit*>(FindControl(L"account_signup"));// �˻�ע�������
	pswd_signup_edit = static_cast<ui::RichEdit*>(FindControl(L"pswd_signup"));// ����ע�������
	pswd_signup_confirm_edit = static_cast<ui::RichEdit*>(FindControl(L"pswd_confirm"));// ȷ�����������
	secure_ques_edit = static_cast<ui::RichEdit*>(FindControl(L"secure_question"));// �ܱ����������
	secure_ans_edit = static_cast<ui::RichEdit*>(FindControl(L"secure_answer"));// �ܱ��������
	account_signup_btn = static_cast<ui::Button*>(FindControl(L"signup_account"));// �˺�ע�ᰴť
	// �һ��������
	account_combo = static_cast<ui::Combo*>(FindControl(L"accounts"));
	secure_ans_verify_edit = static_cast<ui::RichEdit*>(FindControl(L"secure_answer_verify"));
	secure_question_info = static_cast<ui::Label*>(FindControl(L"secure_question_verify"));
	pswd_found_info = static_cast<ui::Label*>(FindControl(L"pswd_found"));
	check_verify_btn = static_cast<ui::Button*>(FindControl(L"check_verify"));

	// ������������
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
		// ��ò����˻���
		std::wstring forget_name = account_combo->GetText();
		psmCloseConnect();
		AdminUser* admin = AdminUser::getInstance();
		// �������Ա�˻�
		admin->signup();
		admin->login();
		memset(&acc_info, NULL, sizeof(PsmPswdData));
		// ʹ���˺�����ΪΨһ����
		strcpy(acc_info._account, (char*)ANSIToUTF8(W_To_A(forget_name)).c_str());
		admin->set(acc_info);
		// ��ʾ�ܱ�����
		secure_question_info->SetText(A_To_W(UTF8ToANSI(std::string(acc_info._secure_question))));
		return true;
		});
	// ���������߼�
	// ��������
	forget_pswd_edit->AttachButtonUp([&](ui::EventArgs* arg) {
		// �������
		secure_question_info->SetText(L"");
		pswd_found_info->SetText(L"");
		memset(&acc_info, NULL, sizeof(PsmPswdData));
		secure_ans_verify_edit->SetText(L"");
		auto accounts = Account::getValidAccounts();
		account_combo->RemoveAll();
		// ������Ч����
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
	// ���õ�¼�¼��߼�
	login_btn->AttachClick([&](ui::EventArgs* arg) {
		// ��¼��Ӧ
		// һ��temp��������
		rstPswdTemp(temp_data);
		strcpy(temp_data._account, ANSIToUTF8(W_To_A(account_edit->GetText())).c_str());
		strcpy(temp_data._pswd, ANSIToUTF8(W_To_A(pswd_edit->GetText())).c_str());
		common_user->set(temp_data);
		// ���Ե�¼
		if (common_user->login())
		{
			// �򴰿ڹ�������ȡ������
			// չʾ������
			MainForm* main_window = nim_comp::WindowsManager::SingletonShow<MainForm>(MainForm::kClassName);
			main_window->CenterWindow();
			main_window->ShowWindow();
			// �����˳�����
			is_Exit = false;
			// ����Close
			Close();
		}
		else
		{
			MessageBoxA(NULL, "�˺��������", "����", NULL);
		}
		return true;
		});
	// ����ע���¼��߼�
	account_signup_btn->AttachClick([&](ui::EventArgs* arg) {
		// �����Ƿ�Ϸ�
		for (int i = 0; i < 5; i++)
			if (!signup_con[i])
				return true;
		// ����temp����
		rstPswdTemp(temp_data);
		strcpy(temp_data._account, ANSIToUTF8(W_To_A(account_signup_edit->GetText())).c_str());
		strcpy(temp_data._pswd, ANSIToUTF8(W_To_A(pswd_signup_edit->GetText())).c_str());
		strcpy(temp_data._secure_question, ANSIToUTF8(W_To_A(secure_ques_edit->GetText())).c_str());
		strcpy(temp_data._secure_answer, ANSIToUTF8(W_To_A(secure_ans_edit->GetText())).c_str());
		strcpy(temp_data._note, temp_data._account);
		common_user->set(temp_data);
		// ע��
		if (!common_user->signup())
		{
			MessageBoxA(NULL, "���˺ſ����ѱ�ע��", "ע��", NULL);
			return true;
		}
		MessageBoxA(NULL, "�ɹ�ע��", "��ϲ", NULL);
		// �����˺�����
		return true;
		});
	// �ж����������Ƿ�Ϸ���Ч��
	initPsmEditAction(account_signup_edit, [=]() {return signup_con[0] = isLengthFine(account_signup_edit, INPUT_LEN_MIN, ACCOUNT_LEN_MAX); });
	initPsmEditAction(pswd_signup_edit, [=]() {return  signup_con[1] = isLengthFine(pswd_signup_edit, PSWD_LEN_MIN, PSWD_LEN_MAX); });
	// initPsmEditAction(pswd_signup_confirm_edit, PSWD_LEN_MIN, PSWD_LEN_MAX);
	// ȷ��ǰ�������Ƿ���ͬ
	initPsmEditAction(pswd_signup_confirm_edit, [&]() {
		return signup_con[2] = (isLengthFine(pswd_signup_confirm_edit, PSWD_LEN_MIN, PSWD_LEN_MAX)
			&& pswd_signup_confirm_edit->GetText() == pswd_signup_edit->GetText());
		});
	initPsmEditAction(secure_ques_edit, [=]() {return signup_con[3] = isLengthFine(secure_ques_edit, INPUT_LEN_MIN, S_QUESTION_LEN_MAX); });
	initPsmEditAction(secure_ans_edit, [=]() {return signup_con[4] = isLengthFine(secure_ans_edit, INPUT_LEN_MIN, S_QUESTION_LEN_MAX); });
}

LRESULT LoginForm::OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	// �ж��ǵ�½ҳ���˳������л�ҳ��
	if (is_Exit)
		PostQuitMessage(0L);// �����˳���Ϣ
	return __super::OnClose(uMsg, wParam, lParam, bHandled);
}
