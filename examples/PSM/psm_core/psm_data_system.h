/**
 * @file psm_data_system.h
 * @author 龙家可
 * @brief 数据交互的基本接口
 * @version 0.2
 * @date 2022-07-13
 *
 * @copyright Copyright (c) 2022
 *
 */
#ifndef PSM_DATA_SYSTEM
#define PSM_DATA_SYSTEM
#ifdef __cplusplus
#if      __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */ 
#include "psm_base.h"
#include <stdlib.h>  
#include <malloc.h>

#define NULL_LABLE(X, Y) ((X) ? (Y) : PSM_NULL)

	typedef enum psm_data_type
	{
		PSM_NULL,
		PSM_INTEGER,
		PSM_REAL,
		PSM_TEXT,
		PSM_BLOB
	} PsmDataType;

	typedef enum psm_srch_order
	{
		PSM_SRCH_ORDER_ASC,
		PSM_SRCH_ORDER_DESC
	} PsmSrchOrder;

	typedef enum psm_srch_mode
	{
		FULL_MATCH = 0,
		PARTIAL_MATCH
	} PsmDataSrch;

	/**
	 * @brief 表查询结构
	 *
	 */
	typedef struct psm_data_srched
	{
		char** _resultp;
		int _nrow;
		int _ncolumn;
	} PsmDataSrched;

	typedef struct psm_data_srch_order
	{
		char* _column;
		PsmSrchOrder _order;
	} PsmDataSrchOrder;

	/**
	 * @brief 对表的基本信息的封装
	 *
	 */
	struct psm_data_sys
	{
		char _table_name[MAX_FILENAME_LENGTH]; // 表名
		PsmDataSize _size;                     // 大小
		/**
		 * @brief 创建新表
		 *
		 */
		PsmCode(*createTable)(PsmDataStorage* data_storage, ...);

		/**
		 * @brief 删除表
		 *
		 */
		PsmCode(*dropTable)(PsmDataStorage* data_storage);

		/**
		 * @brief 插入内容
		 *
		 */
		PsmCode(*insData)(PsmDataStorage* data_storage, void* key, void* content);

		/**
		 * @brief 修改内容
		 *
		 */
		PsmCode(*updData)(PsmDataStorage* data_storage, void* key, void* content);

		/**
		 * @brief 列举内容
		 *
		 */
		PsmCode(*lsData)(PsmDataStorage* data_storage, PsmDataSrchOrder order, void* key, void** res, int* count);

		/**
		 * @brief 按键值搜索内容
		 *
		 */
		PsmCode(*srchData)(PsmDataStorage* data_storage, PsmDataSrchOrder order, void* key, void** res, int* count, PsmDataSrch mode);

		/**
		 * @brief 按键值删除
		 *
		 */
		PsmCode(*delData)(PsmDataStorage* data_storage, void* key);
	};

	/**
	 * @brief 建立数据库连接
	 *
	 */
	extern PsmCode(*psmInitConnect)(char* db_name, void* key);

	/**
	 * @brief 关闭数据库
	 *
	 * @param db_name 数据库名称
	 * @param key 密码
	 * @return PsmCode 执行码
	 */
	extern PsmCode(*psmCloseConnect)();

	/**
	 * @brief 创建表
	 * 可变参数描述列名称和属性类型，顺序为：列属性，列名称，列属性，列名称，...
	 * 需要先指定组数，例如：psmCreateTable("table_name", 2, "id", PSM_TEXT, "name", PSM_TEXT);
	 *
	 * @param name 表名称
	 * @param args_count 参数组数
	 * @param ... 参数列表
	 * @return PsmCode 执行码
	 */
	extern PsmCode(*psmCreateTable)(char* name, int args_count, ...);

	/**
	 * @brief 删除表
	 * 需要提供表的名称
	 *
	 * @param name 表名称
	 * @return PsmCode 执行码
	 */
	extern PsmCode(*psmDropTable)(char* name);

	/**
	 * @brief 插入数据
	 * 可变参数描述插入数据列，数据类型和名称，顺序为：先顺序输入数据列，再依次有数据类型，数据名称，数据类型，数据名称，...
	 * 例如：psmDbInsert("table_name", 2, "id", "name", PSM_TEXT, "1", PSM_TEXT, "张三");
	 *
	 * @param name 表名称
	 * @param args_count 参数组数
	 * @param ... 参数列表
	 * @return PsmCode 执行码
	 */
	extern PsmCode(*psmInsData)(char* name, int args_count, ...);

	/**
	 * @brief 更新数据
	 * 先传入更新部分的参数组数，再传入条件筛选部分的参数组数
	 * 按照顺序传入更新部分的参数，再按照顺序传入条件筛选部分的参数
	 * 例如：psmDbUpdate("table_name", 1, 1, "name", PSM_TEXT, "李四", "name", PSM_TEXT, "张三");
	 *
	 * @param name 表名称
	 * @param key 关键字（通常NULL缺省）
	 * @param content 内容
	 * @param args_count 参数组数
	 * @param ... 参数列表
	 * @return PsmCode 执行码
	 */
	extern PsmCode(*psmUpdData)(char* name, int args_count_set, int args_count_where, ...);

	/**
	 * @brief 直接通过sql命令查询获得表结果，返回的是一个结果集
	 * 如题，是一个偏底层的操作，不建议使用
	 *
	 * @param name 表名称
	 * @param sql sql命令
	 * @param result 结果集
	 * @return PsmCode 执行码
	 */
	extern PsmCode(*psmSrchDataRaw)(char* name, char* sql, PsmDataSrched* result);

	/**
	 * @brief 复杂属性匹配查询
	 * 可变参数描述查询条件，顺序为：列名称，列属性，匹配内容，列名称，列属性，匹配内容，...
	 * 例如：psmDbSrchWithCondition("table_name", 2, "name", PSM_TEXT, "张三", "age", PSM_INTEGER, 20);
	 *
	 * @param name 表名称
	 * @param order 顺序
	 * @param result 结果集
	 * @param args_count 可变参数个数
	 * @param ... 参数列表
	 * @return PsmCode 执行码
	 */
	extern PsmCode(*psmSrchWithMatches)(char* name, PsmDataSrchOrder order, PsmDataSrch mode, PsmDataSrched* result, int args_count, ...);

	/**
	 * @brief 列出表下全部内容，返回的是一个结果集
	 *
	 * @param name 表名称
	 * @param order 顺序
	 * @param result 结果集
	 * @return PsmCode 执行码
	 */
	extern PsmCode(*psmListData)(char* name, PsmDataSrchOrder order, PsmDataSrched* result);

	/**
	 * @brief 删除数据
	 * 可变参数描述删除条件，顺序为：列名称，列属性，匹配内容，列名称，列属性，匹配内容，...
	 * 例如：psmDbDelData("table_name", 2, "name", PSM_TEXT, "张三", "age", PSM_INTEGER, 20);
	 *
	 * @param name 表名称
	 * @param args_count 参数组数
	 * @param ... 参数列表
	 * @return PsmCode 执行码
	 */
	extern PsmCode(*psmDelData)(char* name, int args_count, ...);
#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */ 
#endif
