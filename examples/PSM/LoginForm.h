#pragma once
#include "ui_components/windows_manager/window_ex.h"
#include "CommonUser.h"
class LoginForm : public nim_comp::WindowEx
{
public:
	LoginForm();
	~LoginForm();

	/**
	 * һ�������ӿ��Ǳ���Ҫ��д�Ľӿڣ����������������ӿ�����������
	 * GetSkinFolder		�ӿ�������Ҫ���ƵĴ���Ƥ����Դ·��
	 * GetSkinFile			�ӿ�������Ҫ���ƵĴ��ڵ� xml �����ļ�
	 * GetWindowClassName	�ӿ����ô���Ψһ��������
	 */
	virtual std::wstring GetSkinFolder() override;
	virtual std::wstring GetSkinFile() override;
	virtual std::wstring GetWindowClassName() const override;
	virtual std::wstring GetWindowId() const override;

	/**
	 * �յ� WM_CREATE ��Ϣʱ�ú����ᱻ���ã�ͨ����һЩ�ؼ���ʼ���Ĳ���
	 */
	virtual void InitWindow() override;

	/**
	 * �յ� WM_CLOSE ��Ϣʱ�ú����ᱻ����
	 */
	virtual LRESULT OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	static const std::wstring kClassName;
private:
	CommonUser* common_user;// ��ͨ�û�
	ui::Box* login_panel;// ��¼ҳ��
	ui::Box* signup_panel;// ע��ҳ��
	ui::Box* forget_panel;// ��������
	// ������
	ui::Button* login_btn;// ��¼��ť
	ui::RichEdit* account_edit;// �˺������
	ui::RichEdit* pswd_edit;// ���������
	ui::Label* forget_pswd_edit;// �������밴ť
	// ע�����
	ui::RichEdit* account_signup_edit;// �˻�ע�������
	ui::RichEdit* pswd_signup_edit;// ����ע�������
	ui::RichEdit* pswd_signup_confirm_edit;// ȷ�����������
	ui::RichEdit* secure_ques_edit;// �ܱ����������
	ui::RichEdit* secure_ans_edit;// �ܱ��������
	ui::Button* account_signup_btn;// �˺�ע�ᰴť
	// �����һ�ҳ��
	ui::Combo* account_combo;
	ui::RichEdit* secure_ans_verify_edit;
	ui::Label* secure_question_info;
	ui::Label* pswd_found_info;
	ui::Button* check_verify_btn;
};

