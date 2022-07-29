#pragma once
#include "ui_components/windows_manager/window_ex.h"
#include "ui_components/ui_components.h"
#include "ui_components/ui_cef_control.h"

#define INPUT_LEN_MIN 1
#define ACCOUNT_LEN_MAX 10
#define PSWD_LEN_MIN 8
#define PSWD_LEN_MAX 16
#define S_QUESTION_LEN_MAX 25

// 检查字串长度合法
inline bool  isLengthFine(ui::RichEdit* edit, size_t l1, size_t l2)
{
	size_t s_len = edit->GetText().length();
	return s_len >= l1 && s_len <= l2;
}

// 处理内容提示
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
	 * 一下三个接口是必须要覆写的接口，父类会调用这三个接口来构建窗口
	 * GetSkinFolder		接口设置你要绘制的窗口皮肤资源路径
	 * GetSkinFile			接口设置你要绘制的窗口的 xml 描述文件
	 * GetWindowClassName	接口
	设置窗口唯一的类名称
	 */
	virtual std::wstring GetSkinFolder() override;
	virtual std::wstring GetSkinFile() override;
	virtual std::wstring GetWindowClassName() const override;
	virtual std::wstring GetWindowId() const override;
	virtual ui::Control* CreateControl(const std::wstring& pstrClass) override;
	/**
	 * 收到 WM_CREATE 消息时该函数会被调用，通常做一些控件初始化的操作
	 */
	virtual void InitWindow() override;

	/**
	 * 收到 WM_CLOSE 消息时该函数会被调用
	 */
	virtual LRESULT OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	static const std::wstring kClassName;
private:
	// 账户
	ui::Box* account_view;
	// 主界面
	ui::Box* main_view;
	// 日程
	ui::Box* schedule_view;
	// 通讯录
	ui::Box* addrbook_view;
	// 记账
	ui::Box* finance_view;
	// 健身
	ui::Box* fit_view;
	// 密码
	ui::Box* pswd_view;
	// CEF
	nim_comp::CefControlBase* cef_control;
};

