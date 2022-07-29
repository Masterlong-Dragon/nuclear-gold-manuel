// PSM.cpp : 定义应用程序的入口点。
//
#include "framework.h"
#include "PSM.h"
#include "MainForm.h"
#include "LoginForm.h"
#include "Account.h"
#include "ui_components/windows_manager/windows_manager.h"
#include "psm_core/psm_data_sqlite.h"
// 补丁
template<>
void nbase::MessageLoopProxy::PostTaskAndReplyRelay<void(), void()>::Run()
{
	std_task_();
	origin_loop_->PostTask(
		nbase::Bind(&PostTaskAndReplyRelay::RunReplyAndSelfDestruct, this));
}

void MainThread::Init()
{
	nbase::ThreadManager::RegisterThread(0);
	psmInitSys();
#ifdef SQLITE_ENABLED
	psmInitSqliteBinding();
#endif // SQLITE_ENABLED
	// 获取资源路径，初始化全局参数
	// 默认皮肤使用 resources\\themes\\default
	// 默认语言使用 resources\\lang\\zh_CN
	// 如需修改请指定 Startup 最后两个参数
	std::wstring theme_dir = nbase::win32::GetCurrentModuleDirectory();
#ifdef _DEBUG
	// Debug 模式下使用本地文件夹作为资源
	// 默认皮肤使用 resources\\themes\\default
	// 默认语言使用 resources\\lang\\zh_CN
	// 如需修改请指定 Startup 最后两个参数
	ui::GlobalManager::Startup(theme_dir + L"resources\\", ui::CreateControlCallback(), false);
#else
	// Release 模式下使用资源中的压缩包作为资源
	// 资源被导入到资源列表分类为 THEME，资源名称为 IDR_THEME
	// 如果资源使用的是本地的 zip 文件而非资源中的 zip 压缩包
	// 可以使用 OpenResZip 另一个重载函数打开本地的资源压缩包
	ui::GlobalManager::OpenResZip(L"resources.zip", "");
	// ui::GlobalManager::OpenResZip(L"resources.zip", "");
	ui::GlobalManager::Startup(L"resources\\", ui::CreateControlCallback(), false);
#endif

	// 创建一个默认带有阴影的居中窗口
	/*MainForm* window = new MainForm();
	window->Create(NULL, MainForm::kClassName.c_str(), WS_OVERLAPPEDWINDOW & ~WS_MAXIMIZEBOX, 0);
	window->CenterWindow();
	window->ShowWindow();*/
	// MessageBoxA(NULL, (std::string("libver: ") +std::string(sqlite3_libversion())).c_str(), "debug", NULL);
	LoginForm* login = nim_comp::WindowsManager::SingletonShow<LoginForm>(LoginForm::kClassName);
	login->CenterWindow();
	login->ShowWindow();
	Account::getValidAccounts();
}

void MainThread::Cleanup()
{
	ui::GlobalManager::Shutdown();
	SetThreadWasQuitProperly(true);
	nbase::ThreadManager::UnregisterThread();
}

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// 将 bin\\cef 目录添加到环境变量，这样可以将所有 CEF 相关文件放到该目录下，方便管理
	// 在项目属性->连接器->输入，延迟加载 nim_libcef.dll
	nim_comp::CefManager::GetInstance()->AddCefDllToPath();

	HRESULT hr = ::OleInitialize(NULL);
	if (FAILED(hr))
		return 0;

	// 初始化 CEF
	CefSettings settings;
	if (!nim_comp::CefManager::GetInstance()->Initialize(nbase::win32::GetCurrentModuleDirectory() + L"cef_temp\\", settings, true))
	{
		return 0;
	}

	MainThread thread;
	// 执行主线程循环
	thread.RunOnCurrentThreadWithLoop(nbase::MessageLoop::kUIMessageLoop);

	// 清理 CEF
	nim_comp::CefManager::GetInstance()->UnInitialize();

	::OleUninitialize();

	return 0;
}
