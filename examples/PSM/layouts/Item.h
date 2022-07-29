#pragma once
#include "VirtualTileBox.h"
#include "../framework.h"
#include "Provider.h"

template <class T>
class Provider;

// 从 ui::ListContainerElement 中继承所有可用功能
template <class T>
class Item : public ui::ListContainerElement
{
public:
	ui::Button* btn_del_;
	ui::Control* control_img_;
	ui::Label* label_tittle_;
	ui::Label* label_info_;
	Item() :btn_del_(nullptr) {};
	~Item() = default;

	// 提供外部调用来初始化 item 数据
	void InitSubControls(int nDataIndex)
	{
		if (btn_del_ == nullptr)
		{
			control_img_  = dynamic_cast<ui::Control*>(FindSubControl(L"info_img"));
			btn_del_ = dynamic_cast<ui::Button*>(FindSubControl(L"btn_del"));
			label_tittle_ = dynamic_cast<ui::Label*>(FindSubControl(L"label_tittle"));
			label_info_ = dynamic_cast<ui::Label*>(FindSubControl(L"label_info"));
			btn_del_->AttachClick(nbase::Bind(&Item<T>::OnRemove, this, std::placeholders::_1));
		}
		
		m_nDataIndex = nDataIndex;
	}
private:
	bool OnRemove(ui::EventArgs* args)
	{
		// 删除时，只需删除数据就可以了，不要删除界面上的元素
		VirtualTileBox* pTileBox = static_cast<VirtualTileBox*>(m_pOwner);
		// 不知道为什么只能用C风格的不安全转换，否则会出错
		Provider<T>* pProvider = (Provider<T>*)(pTileBox->GetDataProvider());
		pProvider->RemoveTask(m_nDataIndex);
		return true;
	}
private:
	int m_nDataIndex;
};

