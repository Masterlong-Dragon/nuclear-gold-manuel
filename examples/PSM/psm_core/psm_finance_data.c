#include "psm_finance_data.h"
#include "psm_data_system.h"
#include "string.h"
#include "math.h"
// 列数
#define PSM_FINANCE_COL 5

// 浮点数判0
#define PSM_FINANCE_FLOAT_IS_ZERO(x) (fabs(x) > 0.000001)

// 创建表
PsmCode psmFinanceCreateTable(PsmDataStorage *data_storage, ...)
{
    // 检查参数非法
    if (data_storage == NULL)
        return PSM_ERROR;
    // 创建表
    psmCreateTable(data_storage->_table_name, PSM_FINANCE_COL,
                   PSM_INTEGER, "type",
                   PSM_TEXT, "date",
                   PSM_TEXT, "name",
                   PSM_REAL, "amount",
                   PSM_TEXT, "note");
    return PSM_OK;
}

// 删除表
PsmCode psmFinanceDropTable(PsmDataStorage *data_storage)
{
    // 检查参数非法
    if (data_storage == NULL)
        return PSM_ERROR;
    // 删除表
    psmDropTable(data_storage->_table_name);
    return PSM_OK;
}

// 插入一条记录
PsmCode psmFinanceInsData(PsmDataStorage *data_storage, void *key, void *content)
{
    // 检查参数非法
    if (data_storage == NULL)
        return PSM_ERROR;
    // 获取财务数据
    PsmFinanceData *finance_data = (PsmFinanceData *)content;
    // 插入数据
    psmInsData(data_storage->_table_name, PSM_FINANCE_COL, "type", "date", "name", "note", "amount",
               PSM_INTEGER, finance_data->_type,
               PSM_TEXT, finance_data->_date,
               PSM_TEXT, finance_data->_name,
               PSM_TEXT, finance_data->_note,
               PSM_REAL, finance_data->_amount);
    return PSM_OK;
}

// 更新记录
PsmCode psmFinanceUpdData(PsmDataStorage *data_storage, void *key, void *content)
{
    // 检查参数非法
    if (data_storage == NULL)
        return PSM_ERROR;
    // 获取财务数据
    PsmFinanceData *finance_data = (PsmFinanceData *)key;
    PsmFinanceData *new_finance_data = (PsmFinanceData *)content;
    // 更新数据
    if (finance_data->_key <= 0)
        return psmUpdData(data_storage->_table_name, PSM_FINANCE_COL, PSM_FINANCE_COL,
                          NULL_LABLE(new_finance_data->_type, "type"), PSM_INTEGER, new_finance_data->_type,
                          NULL_LABLE(new_finance_data->_date[0], "date"), PSM_TEXT, new_finance_data->_date,
                          NULL_LABLE(new_finance_data->_name[0], "name"), PSM_TEXT, new_finance_data->_name,
                          NULL_LABLE(PSM_FINANCE_FLOAT_IS_ZERO(new_finance_data->_amount), "amount"), PSM_REAL, new_finance_data->_amount,
                          NULL_LABLE(new_finance_data->_note[0], "note"), PSM_TEXT, new_finance_data->_note,
                          // 前后
                          NULL_LABLE(finance_data->_type, "type"), PSM_INTEGER, finance_data->_type,
                          NULL_LABLE(finance_data->_date[0], "date"), PSM_TEXT, finance_data->_date,
                          NULL_LABLE(finance_data->_name[0], "name"), PSM_TEXT, finance_data->_name,
                          NULL_LABLE(PSM_FINANCE_FLOAT_IS_ZERO(finance_data->_amount), "amount"), PSM_REAL, finance_data->_amount,
                          NULL_LABLE(finance_data->_note[0], "note"), PSM_TEXT, finance_data->_note);

    return psmUpdData(data_storage->_table_name, PSM_FINANCE_COL, 1,
                      NULL_LABLE(new_finance_data->_type, "type"), PSM_INTEGER, new_finance_data->_type,
                      NULL_LABLE(new_finance_data->_date[0], "date"), PSM_TEXT, new_finance_data->_date,
                      NULL_LABLE(new_finance_data->_name[0], "name"), PSM_TEXT, new_finance_data->_name,
                      NULL_LABLE(PSM_FINANCE_FLOAT_IS_ZERO(new_finance_data->_amount), "amount"), PSM_REAL, new_finance_data->_amount,
                      NULL_LABLE(new_finance_data->_note[0], "note"), PSM_TEXT, new_finance_data->_note,
                      // 前后
                      "Id", PSM_INTEGER, finance_data->_key);
}

// 列举记录
PsmCode psmFinanceLsData(PsmDataStorage *data_storage, PsmDataSrchOrder order, void *key, void **res, int *count)
{
    // 检查参数非法
    if (data_storage == NULL)
        return PSM_ERROR;
    PsmDataSrched _data;
    // 列举数据
    psmListData(data_storage->_table_name, order, &_data);
    // 返回结果
    int nindex = _data._ncolumn;
    *count = _data._nrow;
    // 申请存放结果的内存
    PsmFinanceData *finance_data = (PsmFinanceData *)malloc(sizeof(PsmFinanceData) * (*count));
    for (int i = 0; i < _data._nrow; i++) // 逐行
    {
        finance_data[i]._key = atoi(_data._resultp[nindex++]);    // Id
        finance_data[i]._type = atoi(_data._resultp[nindex++]);   // 类型
        strcpy(finance_data[i]._date, _data._resultp[nindex++]);  // 日期
        strcpy(finance_data[i]._name, _data._resultp[nindex++]);  // 名称
        finance_data[i]._amount = atof(_data._resultp[nindex++]); // 金额
        strcpy(finance_data[i]._note, _data._resultp[nindex++]);  // 备注
    }
    *res = finance_data; // 返回结果
#ifdef SQLITE_ENABLED
    // 释放结果集
    sqlite3_free_table(_data._resultp);
#endif
    return PSM_OK;
}

// 搜索记录
PsmCode psmFinanceSrchData(PsmDataStorage *data_storage, PsmDataSrchOrder order, void *key, void **res, int *count, PsmDataSrch mode)
{
    // 检查参数非法
    if (data_storage == NULL)
        return PSM_ERROR;
    if (key == NULL)
        return psmAddrbookLsData(data_storage, order, NULL, res, count);
    // 获取联系人数据
    PsmFinanceData *finance_data = (PsmFinanceData *)key;
    PsmDataSrched _data;
    // NULL_LABLE 确保可跳过带缺省值的参数，用于标记查询条件
    if (finance_data->_key <= 0)
        psmSrchWithMatches(data_storage->_table_name, order, mode, &_data, PSM_FINANCE_COL,
                           NULL_LABLE(finance_data->_type, "type"), PSM_INTEGER, finance_data->_type,
                           NULL_LABLE(finance_data->_date[0], "date"), PSM_TEXT, finance_data->_date,
                           NULL_LABLE(finance_data->_name[0], "name"), PSM_TEXT, finance_data->_name,
                           NULL_LABLE(PSM_FINANCE_FLOAT_IS_ZERO(finance_data->_amount), "amount"), PSM_REAL, finance_data->_amount,
                           NULL_LABLE(finance_data->_note[0], "note"), PSM_TEXT, finance_data->_note);
    else
        // 如果有主键信息，则使用主键信息查询记录
        psmSrchWithMatches(data_storage->_table_name, order, mode, &_data, 1, "Id", PSM_INTEGER, finance_data->_key);
    // 返回结果
    int nindex = _data._ncolumn;
    *count = _data._nrow;
    // 申请存放结果的内存
    PsmFinanceData *new_finance_data = (PsmFinanceData *)malloc(sizeof(PsmFinanceData) * (*count));
    for (int i = 0; i < _data._nrow; i++) // 逐行
    {
        new_finance_data[i]._key = atoi(_data._resultp[nindex++]);    // Id
        new_finance_data[i]._type = atoi(_data._resultp[nindex++]);   // 类型
        strcpy(new_finance_data[i]._date, _data._resultp[nindex++]);  // 日期
        strcpy(new_finance_data[i]._name, _data._resultp[nindex++]);  // 名称
        new_finance_data[i]._amount = atof(_data._resultp[nindex++]); // 金额
        strcpy(new_finance_data[i]._note, _data._resultp[nindex++]);  // 备注
    }
    *res = new_finance_data; // 返回结果
#ifdef SQLITE_ENABLED
    // 释放结果集
    sqlite3_free_table(_data._resultp);
#endif
    return PSM_OK;
}

// 删除数据
PsmCode psmFinanceDelData(PsmDataStorage *data_storage, void *key)
{
    // 检查参数非法
    if (data_storage == NULL)
        return PSM_ERROR;
    if (key == NULL)
    {
        // 缺省删除所有记录
        psmFinanceDropTable(data_storage);
        psmFinanceCreateTable(data_storage);
        return PSM_OK;
    }
    // 删除数据
    PsmFinanceData *finance_data = (PsmFinanceData *)key;
    // 如果有主键信息，则使用主键信息删除记录
    // NULL_LABLE 确保可跳过带缺省值的参数，用于标记查询条件
    if (finance_data->_key <= 0)
        return psmDelData(data_storage->_table_name, PSM_FINANCE_COL,
                          NULL_LABLE(finance_data->_type, "type"), PSM_INTEGER, finance_data->_type,
                          NULL_LABLE(finance_data->_date[0], "date"), PSM_TEXT, finance_data->_date,
                          NULL_LABLE(finance_data->_name[0], "name"), PSM_TEXT, finance_data->_name,
                          NULL_LABLE(PSM_FINANCE_FLOAT_IS_ZERO(finance_data->_amount), "amount"), PSM_REAL, finance_data->_amount,
                          NULL_LABLE(finance_data->_note[0], "note"), PSM_TEXT, finance_data->_note);
    return psmDelData(data_storage->_table_name, 1, "Id", PSM_INTEGER, finance_data->_key);
}

// 初始化财务数据库
PsmCode psmFinanceDataInit(PsmDataStorage **_data_storage)
{
    // 创建关键结构体
    *_data_storage = (PsmDataStorage *)malloc(sizeof(PsmDataStorage));
    PsmDataStorage *data_storage = *_data_storage;
    // 错误处理
    if (data_storage == NULL)
        return PSM_ERROR;
    // 初始化数据库表名
    strcpy(data_storage->_table_name, psm_finance_table_name);
    // 初始化数据库接口
    data_storage->createTable = psmFinanceCreateTable;
    data_storage->dropTable = psmFinanceDropTable;
    data_storage->insData = psmFinanceInsData;
    data_storage->updData = psmFinanceUpdData;
    data_storage->lsData = psmFinanceLsData;
    data_storage->srchData = psmFinanceSrchData;
    data_storage->delData = psmFinanceDelData;
    data_storage->createTable(data_storage);
    return PSM_OK;
}