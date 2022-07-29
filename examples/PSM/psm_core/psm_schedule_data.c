/**
 *@file psm_schedule_data.c
 *@brief 日程管理系统
 *@version 0.1
 *@date 2022-07-14
 *
 *@copyright Copyright (c) 2022
 *
 */


#include "psm_schedule_data.h"
#include "psm_data_system.h"
#include <string.h>
#include <stdlib.h>

// 日程管理表的有效列数
#define PSM_SCHEDULE_COL 3

// 创建健身管理表
PsmCode psmScheduleCreateTable(PsmDataStorage *data_storage, ...)
{
    // 检查参数合法
       if (data_storage == NULL)
           return PSM_ERROR;
       // 创建表
       psmCreateTable(data_storage->_table_name, PSM_SCHEDULE_COL,
                      PSM_TEXT, "name",
                      PSM_TEXT, "time",
                      PSM_TEXT, "address");
       return PSM_OK;
}

// 删除表
PsmCode psmScheduleDropTable(PsmDataStorage *data_storage)
{
    // 检查参数非法
    if (data_storage == NULL)
        return PSM_ERROR;
    // 删除表
    psmDropTable(data_storage->_table_name);
    return PSM_OK;
}

// 插入一条记录
PsmCode psmScheduleInsData(PsmDataStorage *data_storage, void *key, void *content)
{
    // 检查参数非法
    if (data_storage == NULL)
        return PSM_ERROR;
    // 获取联系人数据
    PsmScheduleData *schedule_data = (PsmScheduleData *)content;

    //if (key == NULL)
    //   key = schedule_data->_note;
    
    // 插入数据
    psmInsData(data_storage->_table_name, PSM_SCHEDULE_COL, "name", "time", "address",
               PSM_TEXT, schedule_data->_name,
               PSM_TEXT, schedule_data->_time,
               PSM_TEXT, schedule_data->_address);
    return PSM_OK;
}

// 删除密码记录
PsmCode psmScheduleDelData(PsmDataStorage *data_storage, void *key)
{
    // 检查参数合法
    if (data_storage == NULL)
        return PSM_ERROR;
    if (key == NULL)
    {
        // 直接删除表中所有数据
        psmScheduleDropTable(data_storage);
        psmScheduleCreateTable(data_storage);
        return PSM_OK;
    }
    // 如果有主键，则使用主键删除，否则删除所有匹配内容的记录
    PsmScheduleData *schedule_data = (PsmScheduleData *)key;
    if (schedule_data->_key <= 0)
        return psmDelData(data_storage->_table_name, PSM_SCHEDULE_COL,
                            NULL_LABLE(schedule_data->_name[0], "name"), PSM_TEXT, schedule_data->_name,
                            NULL_LABLE(schedule_data->_time[0], "time"), PSM_TEXT, schedule_data->_time,
                            NULL_LABLE(schedule_data->_address[0], "address"), PSM_TEXT, schedule_data->_address);
    return psmDelData(data_storage->_table_name, 1, "Id", PSM_INTEGER, schedule_data->_key);
}

// 更新一条密码记录
PsmCode psmScheduleUpdData(PsmDataStorage *data_storage, void *key, void *content)
{
    // 检查参数合法
    if (data_storage == NULL)
        return PSM_ERROR;
    if (key == NULL)
        return PSM_ERROR;
    // 获取关键参数
    PsmScheduleData *schedule_data = (PsmScheduleData *)key;
    PsmScheduleData *new_schedule_data = (PsmScheduleData *)content;
    // NULL_LABLE 确保可跳过带缺省值的参数，用于标记查询条件
    if (schedule_data->_key <= 0)
        return psmUpdData(data_storage->_table_name, PSM_SCHEDULE_COL, PSM_SCHEDULE_COL,
                          NULL_LABLE(new_schedule_data->_name[0], "name"), PSM_TEXT, new_schedule_data->_name,
                          NULL_LABLE(new_schedule_data->_time[0], "time"), PSM_TEXT, new_schedule_data->_time,
                          NULL_LABLE(new_schedule_data->_address[0], "address"), PSM_TEXT, new_schedule_data->_address,
                          NULL_LABLE(schedule_data->_name[0], "name"), PSM_TEXT, schedule_data->_name,
                          NULL_LABLE(schedule_data->_time[0], "time"), PSM_TEXT, schedule_data->_time,
                          NULL_LABLE(schedule_data->_address[0], "address"), PSM_TEXT, schedule_data->_address);
    // 如果有主键信息，则使用主键信息更新记录
    return psmUpdData(data_storage->_table_name, PSM_SCHEDULE_COL, 1,
                          NULL_LABLE(new_schedule_data->_name[0], "name"), PSM_TEXT, new_schedule_data->_name,
                          NULL_LABLE(new_schedule_data->_time[0], "time"), PSM_TEXT, new_schedule_data->_time,
                          NULL_LABLE(new_schedule_data->_address[0], "address"), PSM_TEXT, new_schedule_data->_address,
                          "Id", PSM_INTEGER, schedule_data->_key);
}

// 查询一条密码记录
PsmCode psmScheduleLsData(PsmDataStorage *data_storage, PsmDataSrchOrder order, void *key, void **res, int *count)
{
    // 检查参数合法
    if (data_storage == NULL)
        return PSM_ERROR;
    PsmDataSrched _data;
    psmListData(data_storage->_table_name, order, &_data);
    // 列举所有记录
    // 返回结果
    // 按照sqlite3的结果集格式返回结果
    // 是一个一维数组
    int nindex = _data._ncolumn;
    *count = _data._nrow;
    // 申请存放结果的内存
    PsmScheduleData *schedule_data = (PsmScheduleData *)malloc(sizeof(PsmScheduleData) * (*count));
    for (int i = 0; i < _data._nrow; i++) // 逐行
    {
        schedule_data[i]._key = atoi(_data._resultp[nindex++]);               // 主键
        strcpy(schedule_data[i]._name, _data._resultp[nindex++]);            // 对象
        strcpy(schedule_data[i]._time, _data._resultp[nindex++]);            // 时间
        strcpy(schedule_data[i]._address, _data._resultp[nindex++]);         // 地址
    }
    // 返回结果
    *res = schedule_data;
#ifdef SQLITE_ENABLED
    // 释放结果集
    sqlite3_free_table(_data._resultp);
#endif
    return PSM_OK;
}

// 查询一条密码记录
PsmCode psmScheduleSrchData(PsmDataStorage *data_storage, PsmDataSrchOrder order, void *key, void **res, int *count, PsmDataSrch mode)
{
    // 检查参数合法
    if (data_storage == NULL)
        return PSM_ERROR;
    if (key == NULL)
        return psmScheduleLsData(data_storage, order, NULL, res, count);
    // 获取关键参数
    PsmDataSrched _data;
    PsmScheduleData *schedule_data = (PsmScheduleData *)key;
    
    // NULL_LABLE 确保可跳过带缺省值的参数，用于标记查询条件
    if (schedule_data->_key <= 0)
        psmSrchWithMatches(data_storage->_table_name, order, mode, &_data, PSM_SCHEDULE_COL,
                           NULL_LABLE(schedule_data->_name[0], "name"), PSM_TEXT, schedule_data->_name,
                           NULL_LABLE(schedule_data->_time[0], "time"), PSM_TEXT, schedule_data->_time,
                           NULL_LABLE(schedule_data->_address[0], "address"), PSM_TEXT, schedule_data->_address);
    else
        // 如果有主键信息，则使用主键信息查询记录
        psmSrchWithMatches(data_storage->_table_name, order, mode, &_data, 1, "Id", PSM_INTEGER, schedule_data->_key);
    // 申请存放结果的内存
    PsmScheduleData *new_schedule_data = (PsmScheduleData *)malloc(sizeof(PsmScheduleData) * _data._nrow);
    int nindex = _data._ncolumn;
    *count = _data._nrow;
    for (int i = 0; i < _data._nrow; i++) // 逐行
    {
        new_schedule_data[i]._key = atoi(_data._resultp[nindex++]);              // 主键
        strcpy(new_schedule_data[i]._name, _data._resultp[nindex++]);            // 对象
        strcpy(new_schedule_data[i]._time, _data._resultp[nindex++]);            // 时间
        strcpy(new_schedule_data[i]._address, _data._resultp[nindex++]);         // 地址
    }
    // 返回结果
    *res = new_schedule_data;
#ifdef SQLITE_ENABLED
    // 释放结果集
    sqlite3_free_table(_data._resultp);
#endif
    return PSM_OK;
}

// 初始化日程管理表
PsmCode psmScheduleDataInit(PsmDataStorage **_data_storage)
{
    // 创建关键结构体
    *_data_storage = (PsmDataStorage *)malloc(sizeof(PsmDataStorage));
    PsmDataStorage *data_storage = *_data_storage;
    // 错误处理
    if (data_storage == NULL)
        return PSM_ERROR;
    // 初始化数据库表名
    strcpy(data_storage->_table_name, psm_schedule_data_table_name);
    // 初始化数据库接口
    data_storage->createTable = psmScheduleCreateTable;
    data_storage->dropTable = psmScheduleDropTable;
    data_storage->insData = psmScheduleInsData;
    data_storage->updData = psmScheduleUpdData;
    data_storage->lsData = psmScheduleLsData;
    data_storage->srchData = psmScheduleSrchData;
    data_storage->delData = psmScheduleDelData;
    data_storage->createTable(data_storage);
    return PSM_OK;
}
