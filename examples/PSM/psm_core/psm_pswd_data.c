/**
 * @file psm_pswd_data.c
 * @author 龙家可
 * @brief 处理密码存储的基本接口
 * 这是一个示范意义的接口，可以根据需要自行修改。
 * @version 0.2
 * @date 2022-07-13
 *
 * @copyright Copyright (c) 2022
 *
 */
#include "psm_pswd_data.h"
#include <string.h>
#include <stdlib.h>

// 密码表的有效列数
// 注意，数据库主键并不被算入其中
#define PSM_PSWD_COL 5

// 创建密码管理表
PsmCode psmPswdCreateTable(PsmDataStorage *data_storage, ...)
{
    // 检查参数合法
    if (data_storage == NULL)
        return PSM_ERROR;
    // 创建表
    psmCreateTable(data_storage->_table_name, PSM_PSWD_COL,
                   PSM_TEXT, "note",
                   PSM_TEXT, "account",
                   PSM_TEXT, "password",
                   PSM_TEXT, "secure_question",
                   PSM_TEXT, "secure_answer");
    return PSM_OK;
}

// 删除密码管理表
PsmCode psmPswdDropTable(PsmDataStorage *data_storage)
{
    // 检查参数合法
    if (data_storage == NULL)
        return PSM_ERROR;
    // 删除表
    psmDropTable(data_storage->_table_name);
    return PSM_OK;
}

// 插入一条密码记录
PsmCode psmPswdInsData(PsmDataStorage *data_storage, void *key, void *content)
{
    // 检查参数合法
    if (data_storage == NULL)
        return PSM_ERROR;
    PsmPswdData *pswd_data = (PsmPswdData *)content;
    if (key == NULL)
        key = pswd_data->_note;
    // 插入数据
    psmInsData(data_storage->_table_name, PSM_PSWD_COL, "note", "account", "password", "secure_question", "secure_answer",
               PSM_TEXT, key,
               PSM_TEXT, pswd_data->_account,
               PSM_TEXT, pswd_data->_pswd,
               PSM_TEXT, pswd_data->_secure_question,
               PSM_TEXT, pswd_data->_secure_answer);
    return PSM_OK;
}

// 删除密码记录
PsmCode psmPswdDelData(PsmDataStorage *data_storage, void *key)
{
    // 检查参数合法
    if (data_storage == NULL)
        return PSM_ERROR;
    if (key == NULL)
    {
        // 直接删除表中所有数据
        psmPswdDropTable(data_storage);
        psmPswdCreateTable(data_storage);
        return PSM_OK;
    }
    // 如果有主键，则使用主键删除，否则删除所有匹配内容的记录
    PsmPswdData *pswd_data = (PsmPswdData *)key;
    if (pswd_data->_key <= 0)
        return psmDelData(data_storage->_table_name, PSM_PSWD_COL,
                          NULL_LABLE(pswd_data->_note[0], "note"), PSM_TEXT, pswd_data->_note,
                          NULL_LABLE(pswd_data->_account[0], "account"), PSM_TEXT, pswd_data->_account,
                          NULL_LABLE(pswd_data->_pswd[0], "password"), PSM_TEXT, pswd_data->_pswd,
                          NULL_LABLE(pswd_data->_secure_question[0], "secure_question"), PSM_TEXT, pswd_data->_secure_question,
                          NULL_LABLE(pswd_data->_secure_answer[0], "secure_answer"), PSM_TEXT, pswd_data->_secure_answer);
    return psmDelData(data_storage->_table_name, 1, "Id", PSM_INTEGER, pswd_data->_key);
}

// 更新一条密码记录
PsmCode psmPswdUpdData(PsmDataStorage *data_storage, void *key, void *content)
{
    // 检查参数合法
    if (data_storage == NULL)
        return PSM_ERROR;
    if (key == NULL)
        return PSM_ERROR;
    // 获取关键参数
    PsmPswdData *pswd_data = (PsmPswdData *)key;
    PsmPswdData *new_pswd_data = (PsmPswdData *)content;
    // NULL_LABLE 确保可跳过带缺省值的参数，用于标记查询条件
    if (pswd_data->_key <= 0)
        return psmUpdData(data_storage->_table_name, PSM_PSWD_COL, PSM_PSWD_COL,
                          NULL_LABLE(new_pswd_data->_note[0], "note"), PSM_TEXT, new_pswd_data->_note,
                          NULL_LABLE(new_pswd_data->_account[0], "account"), PSM_TEXT, new_pswd_data->_account,
                          NULL_LABLE(new_pswd_data->_pswd[0], "password"), PSM_TEXT, new_pswd_data->_pswd,
                          NULL_LABLE(new_pswd_data->_secure_question[0], "secure_question"), PSM_TEXT, new_pswd_data->_secure_question,
                          NULL_LABLE(new_pswd_data->_secure_answer[0], "secure_answer"), PSM_TEXT, new_pswd_data->_secure_answer,
                          NULL_LABLE(pswd_data->_note[0], "note"), PSM_TEXT, pswd_data->_note,
                          NULL_LABLE(pswd_data->_account[0], "account"), PSM_TEXT, pswd_data->_account,
                          NULL_LABLE(pswd_data->_pswd[0], "password"), PSM_TEXT, pswd_data->_pswd,
                          NULL_LABLE(pswd_data->_secure_question[0], "secure_question"), PSM_TEXT, pswd_data->_secure_question,
                          NULL_LABLE(pswd_data->_secure_answer[0], "secure_answer"), PSM_TEXT, pswd_data->_secure_answer);
    // 如果有主键信息，则使用主键信息更新记录
    return psmUpdData(data_storage->_table_name, PSM_PSWD_COL, 1,
                      NULL_LABLE(new_pswd_data->_note[0], "note"), PSM_TEXT, new_pswd_data->_note,
                      NULL_LABLE(new_pswd_data->_account[0], "account"), PSM_TEXT, new_pswd_data->_account,
                      NULL_LABLE(new_pswd_data->_pswd[0], "password"), PSM_TEXT, new_pswd_data->_pswd,
                      NULL_LABLE(new_pswd_data->_secure_question[0], "secure_question"), PSM_TEXT, new_pswd_data->_secure_question,
                      NULL_LABLE(new_pswd_data->_secure_answer[0], "secure_answer"), PSM_TEXT, new_pswd_data->_secure_answer,
                      "Id", PSM_INTEGER, pswd_data->_key);
}

// 查询一条密码记录
PsmCode psmPswdLsData(PsmDataStorage *data_storage, PsmDataSrchOrder order, void *key, void **res, int *count)
{
    // 检查参数合法
    if (data_storage == NULL)
        return PSM_ERROR;
    PsmDataSrched _data;
    // 列举所有记录
    psmListData(psm_pswd_data_table_name, order, &_data);
    // 返回结果
    // 按照sqlite3的结果集格式返回结果
    // 是一个一维数组
    int nindex = _data._ncolumn;
    *count = _data._nrow;
    // 申请存放结果的内存
    PsmPswdData *pswd_data = (PsmPswdData *)malloc(sizeof(PsmPswdData) * (*count));
    for (int i = 0; i < _data._nrow; i++) // 逐行
    {
        pswd_data[i]._key = atoi(_data._resultp[nindex++]);              // 主键
        strcpy(pswd_data[i]._note, _data._resultp[nindex++]);            // 备注
        strcpy(pswd_data[i]._account, _data._resultp[nindex++]);         // 账号
        strcpy(pswd_data[i]._pswd, _data._resultp[nindex++]);            // 密码
        strcpy(pswd_data[i]._secure_question, _data._resultp[nindex++]); // 安全问题
        strcpy(pswd_data[i]._secure_answer, _data._resultp[nindex++]);   // 安全答案
    }
    // 返回结果
    *res = pswd_data;
#ifdef SQLITE_ENABLED
    // 释放结果集
    sqlite3_free_table(_data._resultp);
#endif
    return PSM_OK;
}

// 查询一条密码记录
PsmCode psmPswdSrchData(PsmDataStorage *data_storage, PsmDataSrchOrder order, void *key, void **res, int *count, PsmDataSrch mode)
{
    // 检查参数合法
    if (data_storage == NULL)
        return PSM_ERROR;
    if (key == NULL)
        return psmPswdLsData(data_storage, order, NULL, res, count);
    // 获取关键参数
    PsmDataSrched _data;
    PsmPswdData *pswd_data = (PsmPswdData *)key;
    // NULL_LABLE 确保可跳过带缺省值的参数，用于标记查询条件
    if (pswd_data->_key <= 0)
        psmSrchWithMatches(data_storage->_table_name, order, mode, &_data, PSM_PSWD_COL,
                           NULL_LABLE(pswd_data->_note[0], "note"), PSM_TEXT, pswd_data->_note,
                           NULL_LABLE(pswd_data->_account[0], "account"), PSM_TEXT, pswd_data->_account,
                           NULL_LABLE(pswd_data->_pswd[0], "password"), PSM_TEXT, pswd_data->_pswd,
                           NULL_LABLE(pswd_data->_secure_question[0], "secure_question"), PSM_TEXT, pswd_data->_secure_question,
                           NULL_LABLE(pswd_data->_secure_answer[0], "secure_answer"), PSM_TEXT, pswd_data->_secure_answer);
    else
        // 如果有主键信息，则使用主键信息查询记录
        psmSrchWithMatches(data_storage->_table_name, order, mode, &_data, 1, "Id", PSM_INTEGER, pswd_data->_key);
    // 申请存放结果的内存
    PsmPswdData *new_pswd_data = (PsmPswdData *)malloc(sizeof(PsmPswdData) * _data._nrow);
    int nindex = _data._ncolumn;
    *count = _data._nrow;
    for (int i = 0; i < _data._nrow; i++) // 逐行
    {
        new_pswd_data[i]._key = atoi(_data._resultp[nindex++]);              // 主键
        strcpy(new_pswd_data[i]._note, _data._resultp[nindex++]);            // 备注
        strcpy(new_pswd_data[i]._account, _data._resultp[nindex++]);         // 账号
        strcpy(new_pswd_data[i]._pswd, _data._resultp[nindex++]);            // 密码
        strcpy(new_pswd_data[i]._secure_question, _data._resultp[nindex++]); // 安全问题
        strcpy(new_pswd_data[i]._secure_answer, _data._resultp[nindex++]);   // 安全答案
    }
    // 返回结果
    *res = new_pswd_data;
#ifdef SQLITE_ENABLED
    // 释放结果集
    sqlite3_free_table(_data._resultp);
#endif
    return PSM_OK;
}

// 初始化密码管理表
PsmCode psmPswdDataInit(PsmDataStorage **_data_storage)
{
    // 创建关键结构体
    *_data_storage = (PsmDataStorage *)malloc(sizeof(PsmDataStorage));
    PsmDataStorage *data_storage = *_data_storage;
    // 错误处理
    if (data_storage == NULL)
        return PSM_ERROR;
    // 初始化数据库表名
    strcpy(data_storage->_table_name, psm_pswd_data_table_name);
    // 初始化数据库接口
    data_storage->createTable = psmPswdCreateTable;
    data_storage->dropTable = psmPswdDropTable;
    data_storage->insData = psmPswdInsData;
    data_storage->updData = psmPswdUpdData;
    data_storage->lsData = psmPswdLsData;
    data_storage->srchData = psmPswdSrchData;
    data_storage->delData = psmPswdDelData;
    data_storage->createTable(data_storage);
    return PSM_OK;
}