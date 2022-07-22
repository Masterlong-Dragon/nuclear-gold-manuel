#pragma once
#include "ui_components/windows_manager/window_ex.h"
#include "CommonUser.h"
class LoginForm : public nim_comp::WindowEx
{
public:
	LoginForm();
	~LoginForm();

	/**
	 * 一下三个接口是必须要覆写的接口，父类会调用这三个接口来构建窗口
	 * GetSkinFolder		接口设置你要绘制的窗口皮肤资源路径
	 * GetSkinFile			接口设置你要绘制的窗口的 xml 描述文件
	 * GetWindowClassName	接口设置窗口唯一的类名称
	 */
	virtual std::wstring GetSkinFolder() override;
	virtual std::wstring GetSkinFile() override;
	virtual std::wstring GetWindowClassName() const override;
	virtual std::wstring GetWindowId() const override;

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
	CommonUser* common_user;// 普通用户
	ui::Box* login_panel;// 登录页面
	ui::Box* signup_panel;// 注册页面
	ui::Box* forget_panel;// 忘记密码
	// 主界面
	ui::Button* login_btn;// 登录按钮
	ui::RichEdit* account_edit;// 账号输入框
	ui::RichEdit* pswd_edit;// 密码输入框
	ui::Label* forget_pswd_edit;// 忘记密码按钮
	// 注册界面
	ui::RichEdit* account_signup_edit;// 账户注册输入框
	ui::RichEdit* pswd_signup_edit;// 密码注册输入框
	ui::RichEdit* pswd_signup_confirm_edit;// 确认密码输入框
	ui::RichEdit* secure_ques_edit;// 密保问题输入框
	ui::RichEdit* secure_ans_edit;// 密保答案输入框
	ui::Button* account_signup_btn;// 账号注册按钮
	// 密码找回页面
	ui::Combo* account_combo;
	ui::RichEdit* secure_ans_verify_edit;
	ui::Label* secure_question_info;
	ui::Label* pswd_found_info;
	ui::Button* check_verify_btn;
};

