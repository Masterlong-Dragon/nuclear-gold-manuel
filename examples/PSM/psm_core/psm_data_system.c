#include "psm_data_system.h"
/**
 * @brief 建立数据库连接
 *
 */
PsmCode(*psmInitConnect)(char* db_name, void* key);

/**
 * @brief 关闭数据库
 *
 * @param db_name 数据库名称
 * @param key 密码
 * @return PsmCode 执行码
 */
PsmCode(*psmCloseConnect)();

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
PsmCode(*psmCreateTable)(char* name, int args_count, ...);

/**
 * @brief 删除表
 * 需要提供表的名称
 *
 * @param name 表名称
 * @return PsmCode 执行码
 */
PsmCode(*psmDropTable)(char* name);

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
PsmCode(*psmInsData)(char* name, int args_count, ...);

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
PsmCode(*psmUpdData)(char* name, int args_count_set, int args_count_where, ...);

/**
 * @brief 直接通过sql命令查询获得表结果，返回的是一个结果集
 * 如题，是一个偏底层的操作，不建议使用
 *
 * @param name 表名称
 * @param sql sql命令
 * @param result 结果集
 * @return PsmCode 执行码
 */
PsmCode(*psmSrchDataRaw)(char* name, char* sql, PsmDataSrched* result);

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
PsmCode(*psmSrchWithMatches)(char* name, PsmDataSrchOrder order, PsmDataSrch mode, PsmDataSrched* result, int args_count, ...);

/**
 * @brief 列出表下全部内容，返回的是一个结果集
 *
 * @param name 表名称
 * @param order 顺序
 * @param result 结果集
 * @return PsmCode 执行码
 */
PsmCode(*psmListData)(char* name, PsmDataSrchOrder order, PsmDataSrched* result);

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
PsmCode(*psmDelData)(char* name, int args_count, ...);