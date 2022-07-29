#pragma once
#include "framework.h"
#include "layouts/Item.h"
#include <unordered_map>
#include <initializer_list>
#include "psm_core/psm_data_system.h"
// 操作界面的基本功能封装
template <class T>
class EditView
{
public:
	// 特化的信息输入框
	std::unordered_map<std::wstring, ui::RichEdit*> info_edits;
	// 核心数据表
	PsmDataStorage* table;

	// 必须提供主操作页面
	// 特化内容的初始化
	EditView(ui::Box* view, char* _col, std::initializer_list<std::wstring> edits)
	{
		if (view == nullptr)
			ASSERT(FALSE);
		edit_view = view;
		// 获得基本操作控件
		list_btn = static_cast<ui::Button*>(edit_view->FindSubControl(L"list_btn"));
		add_btn = static_cast<ui::Button*>(edit_view->FindSubControl(L"add_info"));
		srch_btn = static_cast<ui::Button*>(edit_view->FindSubControl(L"srch_info"));
		edit_btn = static_cast<ui::Button*>(edit_view->FindSubControl(L"edit_info"));
		srch_edit = static_cast<ui::RichEdit*>(edit_view->FindSubControl(L"srch_edit"));
		mode_label = static_cast<ui::Label*>(edit_view->FindSubControl(L"display_mode"));
		srch_cond_view = static_cast<ui::Box*>(edit_view->FindSubControl(L"srch_options"));
		partial_option = static_cast<ui::Option*>(edit_view->FindSubControl(L"partial_match"));
		desc_option = static_cast<ui::Option*>(edit_view->FindSubControl(L"desc_list"));
		v_list = (VirtualTileBox*)(edit_view->FindSubControl(L"v_list"));
		v_list->SelectNextWhenActiveRemoved(false);
		// 获取特化操作控件
		for (auto& es : edits)
			info_edits.emplace(es, static_cast<ui::RichEdit*>(edit_view->FindSubControl(es)));
		// 初始化provider完成关联
		pv = new Provider<T>;
		v_list->SetDataProvider((VirtualTileInterface*)pv);
		// RelateDataView();
		currentMode = EditMode::ADD;
		current_index = -1;
		order = { _col, PsmSrchOrder::PSM_SRCH_ORDER_DESC };
	};

	// 请仅在初始化时调用
	// 确认显示项目的基本内容和属性
	EditView* SetProviderAddAction(std::function<void(Item<T>* temp, T& data)> add)
	{
		pv->SetAddElement(add);
		return this;
	}
	// 请仅在初始化时调用
	EditView* SetProviderRemoveAction(std::function<void(T& info)> del)
	{
		if (del == nullptr)
			del = [=](T& info) {
			// 设置为默认删除选项
			table->delData(table, (void*)(&info));
		};
		pv->SetDelElement(del);
		return this;
	}
	// 请仅在初始化时调用
	// 确认新增时进行的操作
	// 返回操作完成的结构体
	EditView* SetDataAddAction(std::function<T()> add)
	{
		if (add != nullptr)
		{
			add_data = add;
			edit_btn->AttachClick([&](ui::EventArgs* arg) {
				if (currentMode == EditMode::ADD)
				{
					if (!EditsValid())
						return true;
					// 获取完整数据结构体
					T _data = add_data();
					// 复制到temp中
					memcpy(&temp_data, (void*)&_data, sizeof(T));
					// 显示部分添加
					pv->AddTask(temp_data);
					// 数据表同步操作
					table->insData(table, NULL, (void*)(&temp_data));
				}
				else if (currentMode == EditMode::UPD)
				{
					if (!EditsValid())
						return true;
					// 获取完整数据结构体
					T _data = add_data();
					// 数据表同步操作
					table->updData(table, (void*)&temp_data, (void*)&_data);
					// 复制到temp中
					memcpy(&temp_data, (void*)&_data, sizeof(T));
					// 显示部分更新
					pv->UpdateTask(current_index, temp_data);
				}
				else if (currentMode == EditMode::SRCH)
				{
					if (!EditsValid())
						return true;
					// 获取完整数据结构体
					T srch_temp = add_data();
					PsmDataSrch match = partial_option->IsSelected() ? PsmDataSrch::PARTIAL_MATCH : PsmDataSrch:: FULL_MATCH;
					PsmSrchOrder srch_order = desc_option->IsSelected() ? PsmSrchOrder::PSM_SRCH_ORDER_DESC : PsmSrchOrder::PSM_SRCH_ORDER_ASC;
					PsmDataSrchOrder _order = { order._column, srch_order };
					T* res;
					int count;
					table->srchData(table, _order, (void*)&srch_temp, (void**)&res, &count, match);
					pv->ClearTask();
					pv->InitTask(res, count);
					free(res);
				}
				return true;
				});
		}
		return this;
	}

	EditView* SetDataShowAction(std::function<void(T& data)> show)
	{
		if (show != nullptr)
			show_data = show;
		return this;
	}

	EditView* SetSrchTextAction(std::function<void(T& data, std::wstring text)> srch)
	{
		if (srch != nullptr)
			srch_text = srch;
		return this;
	}

	EditView* Init(std::function<void()> setEditsValid)
	{
		// 绑定基本事件函数
		pv->SetDelElement([=](T& info) {
			// 设置为默认删除选项
			table->delData(table, (void*)(&info));
			// 如果是编辑模式 需要退出当前状态
			if (currentMode == EditMode::UPD)
			{
				for (auto& edit : info_edits)
				{
					edit.second->SetText(L"");
					edit.second->SetBorderSize(0);
				}
				currentMode = EditMode::ADD;
				mode_label->SetText(L"新增内容");
			}
			});
		// 设置基本按键对应绑定内容
		// 新增模式
		add_btn->AttachClick([&](ui::EventArgs* arg) {
			currentMode = EditMode::ADD;
			mode_label->SetText(L"新增内容");
			for (auto& edit : info_edits)
			{
				edit.second->SetText(L"");
				edit.second->SetBorderSize(0);
			}
			// 隐藏搜索界面
			srch_cond_view->SetVisible(false);
			return true;
			});
		// 搜索模式
		srch_btn->AttachClick([&](ui::EventArgs* arg) {
			currentMode = EditMode::SRCH;
			mode_label->SetText(L"搜索模式");
			for (auto& edit : info_edits)
			{
				edit.second->SetText(L"");
				edit.second->SetBorderSize(0);
			}
			// 展示搜索界面
			srch_cond_view->SetVisible(true);
			return true;
			});
		// 编辑模式
		v_list->AttachSelect([&](ui::EventArgs* arg) {
			currentMode = EditMode::UPD;
			mode_label->SetText(L"编辑模式");
			// 隐藏搜索界面
			srch_cond_view->SetVisible(false);
			current_index = arg->wParam;
			// 复制到当前操作内容
			memcpy((void*)&temp_data, (void*)&pv->GetData(current_index), sizeof(T));
			// 展示内容
			show_data(temp_data);
			for (auto& edit : info_edits)
			{
				edit.second->SetBorderSize(0);
			}
			return true;
			});
		// 回车然后搜索
		srch_edit->AttachReturn([&](ui::EventArgs* arg) {
			std::wstring srch_content = srch_edit->GetText();
			if (srch_content != L"")
			{
				T srch_temp = {};
				srch_text(srch_temp, srch_content);
				T* res;
				int count;
				table->srchData(table, order, (void*)&srch_temp, (void**)&res, &count, PsmDataSrch::PARTIAL_MATCH);
				pv->ClearTask();
				pv->InitTask(res, count);
				free(res);
			}
			return true;
			});
		list_btn->AttachClick([&](ui::EventArgs* arg) {
			ListAll();
			return true;
			});
		ListAll();
		setEditsValid();
		return this;
	}
	~EditView()
	{
		delete pv;
		delete this;
	};
private:
	// 缓存操作的基本数据结构
	T temp_data;
	// 当前的选择项
	int current_index;
	// 顺序
	PsmDataSrchOrder order;
	// 新增项
	std::function<T()> add_data;
	// 编辑显示项
	std::function<void(T& data)> show_data;
	// 默认搜索操作项
	std::function<void(T& data, std::wstring text)> srch_text;
	// 主页
	ui::Box* edit_view;
	// 搜索条件栏
	ui::Box* srch_cond_view;
	// 匹配模式
	ui::Option* partial_option;
	// 排序方式
	ui::Option* desc_option;
	// 查看按钮
	ui::Button* list_btn;
	// 新增按钮
	ui::Button* add_btn;
	// 查找按钮
	ui::Button* srch_btn;
	// 编辑确认按钮
	ui::Button* edit_btn;
	// 查找输入框
	ui::RichEdit* srch_edit;
	// 信息栏模式
	ui::Label* mode_label;
	// 列表平铺显示框
	VirtualTileBox* v_list;
	// 列表元素操作器
	Provider<T>* pv;
	// 当前操作状态
	enum EditMode
	{
		ADD,
		UPD,
		SRCH
	};
	EditMode currentMode;

	void RelateDataView(std::function<void(Item<T>* temp, T& data)> add)
	{
		// 初始化provider完成关联
		pv.SetAddElement(add);
		v_list->SetDataProvider(pv);
	}

	void ListAll()
	{
		T* res;
		int count;
		table->lsData(table, order, NULL, (void**)&res, &count);
		pv->ClearTask();
		pv->InitTask(res, count);
		free(res);
	}

	bool EditsValid()
	{
		bool res = false;
		for (auto& e : info_edits)
		{
			if (e.second->GetBorderSize())
				return false;
			if (!e.second->GetText().empty())
				res = true;
		}
		return res;
	}
};

