#pragma once
#include "ui_components/windows_manager/window_ex.h"
#include "ui_components/ui_components.h"
#include "ui_components/ui_cef_control.h"

#define INPUT_LEN_MIN 1
#define ACCOUNT_LEN_MAX 10
#define PSWD_LEN_MIN 8
#define PSWD_LEN_MAX 16
#define S_QUESTION_LEN_MAX 25

// ����ִ����ȺϷ�
inline bool  isLengthFine(ui::RichEdit* edit, size_t l1, size_t l2)
{
	size_t s_len = edit->GetText().length();
	return s_len >= l1 && s_len <= l2;
}

// ����������ʾ
template  <typename EXP>
inline void initPsmEditAction(ui::RichEdit* edit, EXP f)
{
	edit->AttachTextChange([=](ui::EventArgs* arg) {
		edit->SetBorderSize(1);
		if (f())
			edit->SetBorderColor(L"gray");
		else
			edit->SetBorderColor(L"red");
		return true;
		});
	edit->AttachKillFocus([=](ui::EventArgs* arg) {
		if (f())
			edit->SetBorderSize(0);
		return true;
		});
}

class MainForm : public nim_comp::WindowEx
{
public:
	MainForm();
	~MainForm();

	/**
	 * һ�������ӿ��Ǳ���Ҫ��д�Ľӿڣ����������������ӿ�����������
	 * GetSkinFolder		�ӿ�������Ҫ���ƵĴ���Ƥ����Դ·��
	 * GetSkinFile			�ӿ�������Ҫ���ƵĴ��ڵ� xml �����ļ�
	 * GetWindowClassName	�ӿ�
	���ô���Ψһ��������
	 */
	virtual std::wstring GetSkinFolder() override;
	virtual std::wstring GetSkinFile() override;
	virtual std::wstring GetWindowClassName() const override;
	virtual std::wstring GetWindowId() const override;
	virtual ui::Control* CreateControl(const std::wstring& pstrClass) override;
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
	// �˻�
	ui::Box* account_view;
	// ������
	ui::Box* main_view;
	// �ճ�
	ui::Box* schedule_view;
	// ͨѶ¼
	ui::Box* addrbook_view;
	// ����
	ui::Box* finance_view;
	// ����
	ui::Box* fit_view;
	// ����
	ui::Box* pswd_view;
	// CEF
	nim_comp::CefControlBase* cef_control;
};

