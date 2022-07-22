/**
 *@file psm_fitrecord_data.h
 *@author 郭宜佳
 *@brief 健身系统
 *@version 0.1
 *@date 2022-07-14
 *
 *@copyright Copyright (c) 2022
 *
 */

#include "psm_fitrecord_data.h"
#include "psm_data_system.h"
#include <string.h>
#include <stdlib.h>

// 健身表的有效列数
#define PSM_FITRECORD_COL 5

// 创建健身管理表
PsmCode psmFitrecordCreateTable(PsmDataStorage *data_storage, ...)
{
    // 检查参数合法
       if (data_storage == NULL)
           return PSM_ERROR;
       // 创建表
       psmCreateTable(data_storage->_table_name, PSM_FITRECORD_COL,
                      PSM_TEXT, "date",
                      PSM_TEXT, "duration",
                      PSM_TEXT, "note",
                      PSM_TEXT, "intensity",
                      PSM_TEXT, "type");
       return PSM_OK;
}

// 删除表
PsmCode psmFitrecordDropTable(PsmDataStorage *data_storage)
{
    // 检查参数非法
    if (data_storage == NULL)
        return PSM_ERROR;
    // 删除表
    psmDropTable(data_storage->_table_name);
    return PSM_OK;
}

// 插入一条记录
PsmCode psmFitrecordInsData(PsmDataStorage *data_storage, void *key, void *content)
{
    // 检查参数非法
    if (data_storage == NULL)
        return PSM_ERROR;
    // 获取联系人数据
    PsmFitrecordData *fitrecord_data = (PsmFitrecordData *)content;
   
    // 插入数据
    psmInsData(data_storage->_table_name, PSM_FITRECORD_COL, "date", "duration", "note", "intensity", "type",
               PSM_TEXT, fitrecord_data->_date,
               PSM_TEXT, fitrecord_data->_duration,
               PSM_TEXT, fitrecord_data->_note,
               PSM_TEXT, fitrecord_data->_intensity,
               PSM_TEXT, fitrecord_data->_type);
    return PSM_OK;
}

// 删除数据
PsmCode psmFitrecordDelData(PsmDataStorage *data_storage, void *key)
{
    // 检查参数非法
    if (data_storage == NULL)
        return PSM_ERROR;
    if (key == NULL)
    {
        // 缺省删除所有记录
        psmFitrecordDropTable(data_storage);
        psmFitrecordCreateTable(data_storage);
        return PSM_OK;
    }
    // 删除数据
    PsmFitrecordData *fitrecord_data = (PsmFitrecordData *)key;
    // 如果有主键信息，则使用主键信息删除记录
    // NULL_LABLE 确保可跳过带缺省值的参数，用于标记查询条件
    if (fitrecord_data->_key <= 0)
        return psmDelData(data_storage->_table_name, PSM_FITRECORD_COL,
                          NULL_LABLE(fitrecord_data->_date[0], "date"), PSM_TEXT, fitrecord_data->_date,
                          NULL_LABLE(fitrecord_data->_duration[0], "duration"), PSM_TEXT, fitrecord_data->_duration,
                          NULL_LABLE(fitrecord_data->_note[0], "note"), PSM_TEXT, fitrecord_data->_note,
                          NULL_LABLE(fitrecord_data->_intensity[0], "intensity"), PSM_TEXT, fitrecord_data->_intensity,
                          NULL_LABLE(fitrecord_data->_type[0], "type"), PSM_TEXT, fitrecord_data->_type);
    return psmDelData(data_storage->_table_name, 1, "Id", PSM_INTEGER, fitrecord_data->_key);
}

// 更新记录
PsmCode psmFitrecordUpdData(PsmDataStorage *data_storage, void *key, void *content)
{
    // 检查参数非法
    if (data_storage == NULL)
        return PSM_ERROR;
     if (key == NULL)
        return PSM_ERROR;
    // 获取联系人数据
    PsmFitrecordData *fitrecord_data = (PsmFitrecordData *)key;
    PsmFitrecordData *new_fitrecord_data = (PsmFitrecordData *)content;
    // NULL_LABLE 确保可跳过带缺省值的参数，用于标记查询条件
    if (fitrecord_data->_key <= 0)
        return psmUpdData(data_storage->_table_name, PSM_FITRECORD_COL, PSM_FITRECORD_COL,
                          NULL_LABLE(new_fitrecord_data->_date[0], "date"), PSM_TEXT, new_fitrecord_data->_date,
                          NULL_LABLE(new_fitrecord_data->_duration[0], "duration"), PSM_TEXT, new_fitrecord_data->_duration,
                          NULL_LABLE(new_fitrecord_data->_note[0], "note"), PSM_TEXT, new_fitrecord_data->_note,
                          NULL_LABLE(new_fitrecord_data->_intensity[0], "intensity"), PSM_TEXT, new_fitrecord_data->_intensity,
                          NULL_LABLE(new_fitrecord_data->_type[0], "type"), PSM_TEXT, new_fitrecord_data->_type,
                          NULL_LABLE(fitrecord_data->_date[0], "date"), PSM_TEXT, fitrecord_data->_date,
                          NULL_LABLE(fitrecord_data->_duration[0], "duration"), PSM_TEXT, fitrecord_data->_duration,
                          NULL_LABLE(fitrecord_data->_note[0], "note"), PSM_TEXT, fitrecord_data->_note,
                          NULL_LABLE(fitrecord_data->_intensity[0], "intensity"), PSM_TEXT, fitrecord_data->_intensity,
                          NULL_LABLE(fitrecord_data->_type[0], "type"), PSM_TEXT, fitrecord_data->_type);
    
    // 如果有主键信息，则使用主键信息更新记录
    // ?第3个参数
    psmUpdData(data_storage->_table_name, PSM_FITRECORD_COL, 1,
               NULL_LABLE(new_fitrecord_data->_date[0], "date"), PSM_TEXT, new_fitrecord_data->_date,
               NULL_LABLE(new_fitrecord_data->_duration[0], "duration"), PSM_TEXT, new_fitrecord_data->_duration,
               NULL_LABLE(new_fitrecord_data->_note[0], "note"), PSM_TEXT, new_fitrecord_data->_note,
               NULL_LABLE(new_fitrecord_data->_intensity[0], "intensity"), PSM_TEXT, new_fitrecord_data->_intensity,
               NULL_LABLE(new_fitrecord_data->_type[0], "type"), PSM_TEXT, new_fitrecord_data->_type,
               "Id", PSM_INTEGER, fitrecord_data->_key);
    return PSM_OK;
}

// 列举记录
PsmCode psmFitrecordLsData(PsmDataStorage *data_storage, PsmDataSrchOrder order, void *key, void **res, int *count)
{
    // 检查参数非法
    if (data_storage == NULL)
        return PSM_ERROR;
    PsmDataSrched _data;
    // 列举数据
    psmListData(psm_fitrecord_table_name, order, &_data);
    // 返回结果
    int nindex = _data._ncolumn;
    *count = _data._nrow;
    // 申请存放结果的内存
    PsmFitrecordData *fitrecord_data = (PsmFitrecordData *)malloc(sizeof(PsmFitrecordData) * (*count));
    for (int i = 0; i < _data._nrow; i++) // 逐行
    {
        fitrecord_data[i]._key = atoi(_data._resultp[nindex++]);         // Id
        strcpy(fitrecord_data[i]._date, _data._resultp[nindex++]);  // date
        strcpy(fitrecord_data[i]._duration, _data._resultp[nindex++]); // duration
        strcpy(fitrecord_data[i]._note, _data._resultp[nindex++]);      // note
        strcpy(fitrecord_data[i]._intensity, _data._resultp[nindex++]);      // note
        strcpy(fitrecord_data[i]._type, _data._resultp[nindex++]);      // note
    }
    *res = fitrecord_data; // 返回结果
#ifdef SQLITE_ENABLED
    // 释放结果集
    sqlite3_free_table(_data._resultp);
#endif
    return PSM_OK;
}

//搜索记录
PsmCode psmFitrecordSrchData(PsmDataStorage *data_storage, PsmDataSrchOrder order ,void *key, void **res, int *count, PsmDataSrch mode)
{
    // 检查参数非法
    if (data_storage == NULL)
        return PSM_ERROR;
    if (key == NULL)
        return psmFitrecordLsData(data_storage, order, NULL, res, count);
    // 获取联系人数据
    PsmFitrecordData *fitrecord_data = (PsmFitrecordData *)key;
    PsmDataSrched _data;
    // NULL_LABLE 确保可跳过带缺省值的参数，用于标记查询条件
    if (fitrecord_data->_key <= 0)
        psmSrchWithMatches(data_storage->_table_name, order, mode, &_data, PSM_FITRECORD_COL,
                           NULL_LABLE(fitrecord_data->_date[0], "date"), PSM_TEXT, fitrecord_data->_date,
                           NULL_LABLE(fitrecord_data->_duration[0], "duration"), PSM_TEXT, fitrecord_data->_duration,
                           NULL_LABLE(fitrecord_data->_note[0], "note"), PSM_TEXT, fitrecord_data->_note,
                           NULL_LABLE(fitrecord_data->_intensity[0], "intensity"), PSM_TEXT, fitrecord_data->_intensity,
                           NULL_LABLE(fitrecord_data->_type[0], "type"), PSM_TEXT, fitrecord_data->_type);
    else
        // 如果有主键信息，则使用主键信息查询记录
        psmSrchWithMatches(data_storage->_table_name, order, mode, &_data, 1, "Id", PSM_INTEGER, fitrecord_data->_key);
    // 返回结果
    int nindex = _data._ncolumn;
    *count = _data._nrow;
    // 申请存放结果的内存
    PsmFitrecordData *new_fitrecord_data = (PsmFitrecordData *)malloc(sizeof(PsmFitrecordData) * (*count));
    for (int i = 0; i < _data._nrow; i++) // 逐行
    {
        new_fitrecord_data[i]._key = atoi(_data._resultp[nindex++]);         // Id
        strcpy(new_fitrecord_data[i]._date, _data._resultp[nindex++]);  // date
        strcpy(new_fitrecord_data[i]._duration, _data._resultp[nindex++]); // duration
        strcpy(new_fitrecord_data[i]._note, _data._resultp[nindex++]);      // note
        strcpy(new_fitrecord_data[i]._intensity, _data._resultp[nindex++]);      // note
        strcpy(new_fitrecord_data[i]._type, _data._resultp[nindex++]);      // note
    }
    *res = new_fitrecord_data; // 返回结果
#ifdef SQLITE_ENABLED
    // 释放结果集
    sqlite3_free_table(_data._resultp);
#endif
    return PSM_OK;
}



// 初始化健身计划数据结构体
PsmCode psmFitrecordDataInit(PsmDataStorage **_data_storage)
{
    // 创建关键结构体
    *_data_storage = (PsmDataStorage *)malloc(sizeof(PsmDataStorage));
    PsmDataStorage *data_storage = *_data_storage;
    // 错误处理
    if (data_storage == NULL)
        return PSM_ERROR;
    // 初始化数据库表名
    strcpy(data_storage->_table_name, psm_fitrecord_table_name);
    // 初始化数据库接口
    data_storage->createTable = psmFitrecordCreateTable;
    data_storage->dropTable = psmFitrecordDropTable;
    data_storage->insData = psmFitrecordInsData;
    data_storage->updData = psmFitrecordUpdData;
    data_storage->lsData = psmFitrecordLsData;
    data_storage->srchData = psmFitrecordSrchData;
    data_storage->delData = psmFitrecordDelData;
    data_storage->createTable(data_storage);
    return PSM_OK;
}
