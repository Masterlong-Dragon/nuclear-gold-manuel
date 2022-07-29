/**
 * @file psm_addrbook_data.c
 * @brief 通讯录处理模块
 * @version 0.1
 * @date 2022-07-13
 *
 * @copyright Copyright (c) 2022
 *
 */
#include "psm_addrbook_data.h"
#include "psm_data_system.h"

// 列数
#define PSM_ADDRBOOK_COL 5

// 创建表
PsmCode psmAddrbookCreateTable(PsmDataStorage *data_storage, ...)
{
    // 检查参数非法
    if (data_storage == NULL)
        return PSM_ERROR;
    // 创建表
    psmCreateTable(data_storage->_table_name, PSM_ADDRBOOK_COL,
                   PSM_TEXT, "name",
                   PSM_TEXT, "phone",
                   PSM_INTEGER, "age",
                   PSM_TEXT, "email",
                   PSM_TEXT, "address");
    return PSM_OK;
}

// 删除表
PsmCode psmAddrbookDropTable(PsmDataStorage *data_storage)
{
    // 检查参数非法
    if (data_storage == NULL)
        return PSM_ERROR;
    // 删除表
    psmDropTable(data_storage->_table_name);
    return PSM_OK;
}

// 插入一条记录
PsmCode psmAddrbookInsData(PsmDataStorage *data_storage, void *key, void *content)
{
    // 检查参数非法
    if (data_storage == NULL)
        return PSM_ERROR;
    // 获取联系人数据
    PsmAddrbookData *addrbook_data = (PsmAddrbookData *)content;
    // 插入数据
    psmInsData(data_storage->_table_name, PSM_ADDRBOOK_COL,"name", "phone", "age", "email", "address",
               PSM_TEXT, addrbook_data->_name,
               PSM_TEXT, addrbook_data->_phone,
               PSM_INTEGER, addrbook_data->_age,
               PSM_TEXT, addrbook_data->_email,
               PSM_TEXT, addrbook_data->_address);
    return PSM_OK;
}

// 更新记录
PsmCode psmAddrbookUpdData(PsmDataStorage *data_storage, void *key, void *content)
{
    // 检查参数非法
    if (data_storage == NULL)
        return PSM_ERROR;
    // 获取联系人数据
    PsmAddrbookData *addrbook_data = (PsmAddrbookData *)key;
    PsmAddrbookData *new_addrbook_data = (PsmAddrbookData *)content;
    // NULL_LABLE 确保可跳过带缺省值的参数，用于标记查询条件
    if (addrbook_data->_key <= 0)
        return psmUpdData(data_storage->_table_name, PSM_ADDRBOOK_COL, PSM_ADDRBOOK_COL,
                          NULL_LABLE(new_addrbook_data->_name[0], "name"), PSM_TEXT, new_addrbook_data->_name,
                          NULL_LABLE(new_addrbook_data->_phone[0], "phone"), PSM_TEXT, new_addrbook_data->_phone,
                          NULL_LABLE(new_addrbook_data->_age > 0, "age"), PSM_INTEGER, new_addrbook_data->_age,
                          NULL_LABLE(new_addrbook_data->_email[0], "email"), PSM_TEXT, new_addrbook_data->_email,
                          NULL_LABLE(new_addrbook_data->_address[0], "address"), PSM_TEXT, new_addrbook_data->_address,
                          // 前后
                          NULL_LABLE(addrbook_data->_name[0], "name"), PSM_TEXT, addrbook_data->_name,
                          NULL_LABLE(addrbook_data->_phone[0], "phone"), PSM_TEXT, addrbook_data->_phone,
                          NULL_LABLE(addrbook_data->_age > 0, "age"), PSM_INTEGER, addrbook_data->_age,
                          NULL_LABLE(addrbook_data->_email[0], "email"), PSM_TEXT, addrbook_data->_email,
                          NULL_LABLE(addrbook_data->_address[0], "address"), PSM_TEXT, addrbook_data->_address);
    // 如果有主键信息，则使用主键信息更新记录
    return psmUpdData(data_storage->_table_name, PSM_ADDRBOOK_COL, 1,
                      NULL_LABLE(new_addrbook_data->_name[0], "name"), PSM_TEXT, new_addrbook_data->_name,
                      NULL_LABLE(new_addrbook_data->_phone[0], "phone"), PSM_TEXT, new_addrbook_data->_phone,
                      NULL_LABLE(new_addrbook_data->_age > 0, "age"), PSM_INTEGER, new_addrbook_data->_age,
                      NULL_LABLE(new_addrbook_data->_email[0], "email"), PSM_TEXT, new_addrbook_data->_email,
                      NULL_LABLE(new_addrbook_data->_address[0], "address"), PSM_TEXT, new_addrbook_data->_address,
                      // 前后
                      "Id", PSM_INTEGER, addrbook_data->_key);
}

// 列举记录
PsmCode psmAddrbookLsData(PsmDataStorage *data_storage, PsmDataSrchOrder order, void *key, void **res, int *count)
{
    // 检查参数非法
    if (data_storage == NULL)
        return PSM_ERROR;
    PsmDataSrched _data;
    // 列举数据
    psmListData(psm_addrbook_table_name, order, &_data);
    // 返回结果
    int nindex = _data._ncolumn;
    *count = _data._nrow;
    // 申请存放结果的内存
    PsmAddrbookData *addrbook_data = (PsmAddrbookData *)malloc(sizeof(PsmAddrbookData) * (*count));
    for (int i = 0; i < _data._nrow; i++) // 逐行
    {
        addrbook_data[i]._key = atoi(_data._resultp[nindex++]);         // Id
        strcpy(addrbook_data[i]._name, _data._resultp[nindex++]);  // name
        strcpy(addrbook_data[i]._phone, _data._resultp[nindex++]);      // phone
        addrbook_data[i]._age = atoi(_data._resultp[nindex++]);         // age
        strcpy(addrbook_data[i]._email, _data._resultp[nindex++]);      // email
        strcpy(addrbook_data[i]._address, _data._resultp[nindex++]);    // address
    }
    *res = addrbook_data; // 返回结果
#ifdef SQLITE_ENABLED
    // 释放结果集
    sqlite3_free_table(_data._resultp);
#endif
    return PSM_OK;
}

//搜索记录
PsmCode psmAddrbookSrchData(PsmDataStorage *data_storage, PsmDataSrchOrder order, void *key, void **res, int *count, PsmDataSrch mode)
{
    // 检查参数非法
    if (data_storage == NULL)
        return PSM_ERROR;
    if (key == NULL)
        return psmAddrbookLsData(data_storage, order, NULL, res, count);
    // 获取联系人数据
    PsmAddrbookData *addrbook_data = (PsmAddrbookData *)key;
    PsmDataSrched _data;
    // NULL_LABLE 确保可跳过带缺省值的参数，用于标记查询条件
    if (addrbook_data->_key <= 0)
        psmSrchWithMatches(data_storage->_table_name, order, mode, &_data, PSM_ADDRBOOK_COL,
                           NULL_LABLE(addrbook_data->_name[0], "name"), PSM_TEXT, addrbook_data->_name,
                           NULL_LABLE(addrbook_data->_phone[0], "phone"), PSM_TEXT, addrbook_data->_phone,
                           NULL_LABLE(addrbook_data->_age > 0, "age"), PSM_INTEGER, addrbook_data->_age,
                           NULL_LABLE(addrbook_data->_email[0], "email"), PSM_TEXT, addrbook_data->_email,
                           NULL_LABLE(addrbook_data->_address[0], "address"), PSM_TEXT, addrbook_data->_address);
    else
        // 如果有主键信息，则使用主键信息查询记录
        psmSrchWithMatches(data_storage->_table_name, order, mode, &_data, 1, "Id", PSM_INTEGER, addrbook_data->_key);
    // 返回结果
    int nindex = _data._ncolumn;
    *count = _data._nrow;
    // 申请存放结果的内存
    PsmAddrbookData *new_addrbook_data = (PsmAddrbookData *)malloc(sizeof(PsmAddrbookData) * (*count));
    for (int i = 0; i < _data._nrow; i++) // 逐行
    {
        new_addrbook_data[i]._key = atoi(_data._resultp[nindex++]);         // Id
        strcpy(new_addrbook_data[i]._name, _data._resultp[nindex++]);  // name
        strcpy(new_addrbook_data[i]._phone, _data._resultp[nindex++]);      // phone
        new_addrbook_data[i]._age = atoi(_data._resultp[nindex++]);         // age
        strcpy(new_addrbook_data[i]._email, _data._resultp[nindex++]);      // email
        strcpy(new_addrbook_data[i]._address, _data._resultp[nindex++]);    // address
    }
    *res = new_addrbook_data; // 返回结果
#ifdef SQLITE_ENABLED
    // 释放结果集
    sqlite3_free_table(_data._resultp);
#endif
    return PSM_OK;
}

// 删除数据
PsmCode psmAddrbookDelData(PsmDataStorage *data_storage, void *key)
{
    // 检查参数非法
    if (data_storage == NULL)
        return PSM_ERROR;
    if (key == NULL)
    {
        // 缺省删除所有记录
        psmAddrbookDropTable(data_storage);
        psmAddrbookCreateTable(data_storage);
        return PSM_OK;
    }
    // 删除数据
    PsmAddrbookData *addrbook_data = (PsmAddrbookData *)key;
    // 如果有主键信息，则使用主键信息删除记录
    // NULL_LABLE 确保可跳过带缺省值的参数，用于标记查询条件
    if (addrbook_data->_key <= 0)
        return psmDelData(data_storage->_table_name, PSM_ADDRBOOK_COL,
                          NULL_LABLE(addrbook_data->_name[0], "name"), PSM_TEXT, addrbook_data->_name,
                          NULL_LABLE(addrbook_data->_phone[0], "phone"), PSM_TEXT, addrbook_data->_phone,
                          NULL_LABLE(addrbook_data->_age > 0, "age"), PSM_INTEGER, addrbook_data->_age,
                          NULL_LABLE(addrbook_data->_email[0], "email"), PSM_TEXT, addrbook_data->_email,
                          NULL_LABLE(addrbook_data->_address[0], "address"), PSM_TEXT, addrbook_data->_address);
    return psmDelData(data_storage->_table_name, 1, "Id", PSM_INTEGER, addrbook_data->_key);
}

// 初始化通讯录数据结构体
PsmCode psmAddrbookDataInit(PsmDataStorage **_data_storage)
{
    // 创建关键结构体
    *_data_storage = (PsmDataStorage *)malloc(sizeof(PsmDataStorage));
    PsmDataStorage *data_storage = *_data_storage;
    // 错误处理
    if (data_storage == NULL)
        return PSM_ERROR;
    // 初始化数据库表名
    strcpy(data_storage->_table_name, psm_addrbook_table_name);
    // 初始化数据库接口
    data_storage->createTable = psmAddrbookCreateTable;
    data_storage->dropTable = psmAddrbookDropTable;
    data_storage->insData = psmAddrbookInsData;
    data_storage->updData = psmAddrbookUpdData;
    data_storage->lsData = psmAddrbookLsData;
    data_storage->srchData = psmAddrbookSrchData;
    data_storage->delData = psmAddrbookDelData;
    data_storage->createTable(data_storage);
    return PSM_OK;
}