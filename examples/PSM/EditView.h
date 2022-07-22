#pragma once
#include "framework.h"
#include "layouts/Item.h"
#include <unordered_map>
#include <initializer_list>
#include "psm_core/psm_data_system.h"
// ��������Ļ������ܷ�װ
template <class T>
class EditView
{
public:
	// �ػ�����Ϣ�����
	std::unordered_map<std::wstring, ui::RichEdit*> info_edits;
	// �������ݱ�
	PsmDataStorage* table;

	// �����ṩ������ҳ��
	// �ػ����ݵĳ�ʼ��
	EditView(ui::Box* view, char* _col, std::initializer_list<std::wstring> edits)
	{
		if (view == nullptr)
			ASSERT(FALSE);
		edit_view = view;
		// ��û��������ؼ�
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
		// ��ȡ�ػ������ؼ�
		for (auto& es : edits)
			info_edits.emplace(es, static_cast<ui::RichEdit*>(edit_view->FindSubControl(es)));
		// ��ʼ��provider��ɹ���
		pv = new Provider<T>;
		v_list->SetDataProvider((VirtualTileInterface*)pv);
		// RelateDataView();
		currentMode = EditMode::ADD;
		current_index = -1;
		order = { _col, PsmSrchOrder::PSM_SRCH_ORDER_DESC };
	};

	// ����ڳ�ʼ��ʱ����
	// ȷ����ʾ��Ŀ�Ļ������ݺ�����
	EditView* SetProviderAddAction(std::function<void(Item<T>* temp, T& data)> add)
	{
		pv->SetAddElement(add);
		return this;
	}
	// ����ڳ�ʼ��ʱ����
	EditView* SetProviderRemoveAction(std::function<void(T& info)> del)
	{
		if (del == nullptr)
			del = [=](T& info) {
			// ����ΪĬ��ɾ��ѡ��
			table->delData(table, (void*)(&info));
		};
		pv->SetDelElement(del);
		return this;
	}
	// ����ڳ�ʼ��ʱ����
	// ȷ������ʱ���еĲ���
	// ���ز�����ɵĽṹ��
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
					// ��ȡ�������ݽṹ��
					T _data = add_data();
					// ���Ƶ�temp��
					memcpy(&temp_data, (void*)&_data, sizeof(T));
					// ��ʾ�������
					pv->AddTask(temp_data);
					// ���ݱ�ͬ������
					table->insData(table, NULL, (void*)(&temp_data));
				}
				else if (currentMode == EditMode::UPD)
				{
					if (!EditsValid())
						return true;
					// ��ȡ�������ݽṹ��
					T _data = add_data();
					// ���ݱ�ͬ������
					table->updData(table, (void*)&temp_data, (void*)&_data);
					// ���Ƶ�temp��
					memcpy(&temp_data, (void*)&_data, sizeof(T));
					// ��ʾ���ָ���
					pv->UpdateTask(current_index, temp_data);
				}
				else if (currentMode == EditMode::SRCH)
				{
					if (!EditsValid())
						return true;
					// ��ȡ�������ݽṹ��
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
		// �󶨻����¼�����
		pv->SetDelElement([=](T& info) {
			// ����ΪĬ��ɾ��ѡ��
			table->delData(table, (void*)(&info));
			// ����Ǳ༭ģʽ ��Ҫ�˳���ǰ״̬
			if (currentMode == EditMode::UPD)
			{
				for (auto& edit : info_edits)
				{
					edit.second->SetText(L"");
					edit.second->SetBorderSize(0);
				}
				currentMode = EditMode::ADD;
				mode_label->SetText(L"��������");
			}
			});
		// ���û���������Ӧ������
		// ����ģʽ
		add_btn->AttachClick([&](ui::EventArgs* arg) {
			currentMode = EditMode::ADD;
			mode_label->SetText(L"��������");
			for (auto& edit : info_edits)
			{
				edit.second->SetText(L"");
				edit.second->SetBorderSize(0);
			}
			// ������������
			srch_cond_view->SetVisible(false);
			return true;
			});
		// ����ģʽ
		srch_btn->AttachClick([&](ui::EventArgs* arg) {
			currentMode = EditMode::SRCH;
			mode_label->SetText(L"����ģʽ");
			for (auto& edit : info_edits)
			{
				edit.second->SetText(L"");
				edit.second->SetBorderSize(0);
			}
			// չʾ��������
			srch_cond_view->SetVisible(true);
			return true;
			});
		// �༭ģʽ
		v_list->AttachSelect([&](ui::EventArgs* arg) {
			currentMode = EditMode::UPD;
			mode_label->SetText(L"�༭ģʽ");
			// ������������
			srch_cond_view->SetVisible(false);
			current_index = arg->wParam;
			// ���Ƶ���ǰ��������
			memcpy((void*)&temp_data, (void*)&pv->GetData(current_index), sizeof(T));
			// չʾ����
			show_data(temp_data);
			for (auto& edit : info_edits)
			{
				edit.second->SetBorderSize(0);
			}
			return true;
			});
		// �س�Ȼ������
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
	// ��������Ļ������ݽṹ
	T temp_data;
	// ��ǰ��ѡ����
	int current_index;
	// ˳��
	PsmDataSrchOrder order;
	// ������
	std::function<T()> add_data;
	// �༭��ʾ��
	std::function<void(T& data)> show_data;
	// Ĭ������������
	std::function<void(T& data, std::wstring text)> srch_text;
	// ��ҳ
	ui::Box* edit_view;
	// ����������
	ui::Box* srch_cond_view;
	// ƥ��ģʽ
	ui::Option* partial_option;
	// ����ʽ
	ui::Option* desc_option;
	// �鿴��ť
	ui::Button* list_btn;
	// ������ť
	ui::Button* add_btn;
	// ���Ұ�ť
	ui::Button* srch_btn;
	// �༭ȷ�ϰ�ť
	ui::Button* edit_btn;
	// ���������
	ui::RichEdit* srch_edit;
	// ��Ϣ��ģʽ
	ui::Label* mode_label;
	// �б�ƽ����ʾ��
	VirtualTileBox* v_list;
	// �б�Ԫ�ز�����
	Provider<T>* pv;
	// ��ǰ����״̬
	enum EditMode
	{
		ADD,
		UPD,
		SRCH
	};
	EditMode currentMode;

	void RelateDataView(std::function<void(Item<T>* temp, T& data)> add)
	{
		// ��ʼ��provider��ɹ���
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

