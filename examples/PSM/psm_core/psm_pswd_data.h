/**
 * @file psm_pswd_data.h
 * @brief 处理密码存储的基本接口
 * @version 0.1
 * @date 2022-07-13
 *
 * @copyright Copyright (c) 2022
 *
 */
#ifndef PSM_PSWD_DATA_H
#define PSM_PSWD_DATA_H
#ifdef __cplusplus
#if      __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */ 
#include "psm_base.h"
#include "psm_data_sqlite.h"

/**
 * @brief 常量表名
 *
 */
const static char *psm_pswd_data_table_name = "pswd_data";

/**
 * @brief 最大相关字符串长度
 * 
 */
#define PSM_MAX_PSWD_DATA_COUNT 256

/**
 * @brief 密码数据结构体
 * 
 */
typedef struct psm_pswd_data
{
    char _note[PSM_MAX_PSWD_DATA_COUNT];// 备注
    char _account[PSM_MAX_PSWD_DATA_COUNT];// 账号
    char _pswd[PSM_MAX_PSWD_DATA_COUNT];// 密码
    char _secure_question[PSM_MAX_PSWD_DATA_COUNT];// 安全问题
    char _secure_answer[PSM_MAX_PSWD_DATA_COUNT];// 安全答案
    int _key;// 索引
} PsmPswdData;

/**
 * @brief 初始化密码管理系统
 * 
 * @param _data_storage 表结构体
 * @return PsmCode 执行码
 */
PsmCode psmPswdDataInit(PsmDataStorage **_data_storage);
#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */ 
#endif