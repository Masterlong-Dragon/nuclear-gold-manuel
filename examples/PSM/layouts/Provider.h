#pragma once

// #include "VirtualTileBox.h"
#include <map>
#include <functional>
#include "../framework.h"
#include "Item.h"

template <class T>
class Item;

template <class T>
class Provider : public ui::VirtualTileInterface
{
public:
	Provider() :total(0) {};
	~Provider() = default;

	virtual ui::Control* CreateElement()
	{
		Item<T>* item = new Item<T>;
		ui::GlobalManager::FillBoxWithCache(item, L"main/views/items/item.xml");
		return item;
	}

	/**
	* @brief 填充指定子项
	* @param[in] control 子项控件指针
	* @param[in] index 索引
	* @return 返回创建后的子项指针
	*/
	virtual void FillElement(ui::Control* control, int index)
	{
		Item<T>* p_item = dynamic_cast<Item<T>*>(control);
		// 获取该元素应当显示的内容
		p_item->InitSubControls(index);
		addElement(p_item, data[index]);
	}

	/**
	* @brief 获取子项总数
	* @return 返回子项总数
	*/
	virtual int GetElementtCount()
	{
		// 加锁
		nbase::NAutoLock auto_lock(&lock_);
		return data.size();
	}

public:
	// 对不同类别操作添加时的特别处理
	void SetAddElement(std::function<void(Item<T>* temp, T& data)> add)
	{
		addElement = add;
	}
	void SetDelElement(std::function<void(T& info)> del)
	{
		delElement = del;
	}
	// 对元素的处理
	// 增加项目
	void AddTask(T& temp)
	{
		// 加锁
		lock_.Lock();
		data.emplace_back(temp);
		lock_.Unlock();
		// 通知TileBox数据总数变动
		EmitCountChanged();
	}
	// 移除项目
	void RemoveTask(int nIndex)
	{
		lock_.Lock();
		if (delElement != nullptr)
			delElement(data[nIndex]);
		auto iter = data.begin() + nIndex;
		data.erase(iter);
		lock_.Unlock();
		// 通知TileBox数据总数变动
		EmitCountChanged();
	}
	// 更新项目
	void UpdateTask(int nIndex, T& temp)
	{
		lock_.Lock();
		// 复制内容
		if (nIndex >= 0 && nIndex < (int)data.size())
			memcpy(&data[nIndex], &temp, sizeof(T));
		lock_.Unlock();
		// 发送数据变动通知
		EmitDataChanged(nIndex, nIndex);
	}
	// 清除项目
	void ClearTask()
	{
		lock_.Lock();
		data.clear();
		lock_.Unlock();

		// 通知TileBox数据总数变动
		EmitCountChanged();
	}
	// 初始化设置
	void InitTask(T* head, int count)
	{
		lock_.Lock();
		data = std::vector<T>(head, head + count);
		lock_.Unlock();
		// 通知TileBox数据总数变动
		EmitCountChanged();
	}

	T& GetData(int index)
	{
		return data[index];
	}

private:
	// 新增项目时的操作
	std::function<void(Item<T>* item, T& data)> addElement;
	// 删除项目时的操作
	std::function<void(T& info)> delElement;
	std::vector<T> data;
	int total;
	nbase::NLock  lock_;
};

