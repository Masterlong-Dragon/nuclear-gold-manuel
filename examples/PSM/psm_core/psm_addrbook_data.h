/**
 * @file 通讯录处理模块
 * @author 龙家可
 * @brief 
 * @version 0.1
 * @date 2022-07-13
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#ifndef PSM_ADDRBOOK_DATA_H
#define PSM_ADDRBOOK_DATA_H
#ifdef __cplusplus
#if      __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */ 
#include "psm_base.h"
/**
 * @brief 常量表名
 * 
 */
static const char *psm_addrbook_table_name = "addrbook";
#define PSM_MAX_ADDRBOOK_DATA_COUNT 64
/**
 * @brief 通讯录数据结构体
 * 
 */
typedef struct psm_addrbook_data
{
    char _name[PSM_MAX_ADDRBOOK_DATA_COUNT];// 名
    char _phone[PSM_MAX_ADDRBOOK_DATA_COUNT];// 电话
    int _age;// 年龄
    char _email[PSM_MAX_ADDRBOOK_DATA_COUNT];// 邮箱
    char _address[PSM_MAX_ADDRBOOK_DATA_COUNT];// 地址
    int _key;// 索引
} PsmAddrbookData;

/**
 * @brief 初始化密码管理系统
 * 
 * @param _data_storage 
 * @return PsmCode 
 */
PsmCode psmAddrbookDataInit(PsmDataStorage **_data_storage);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */ 
#endif