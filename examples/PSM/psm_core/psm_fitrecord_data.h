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

#ifndef psm_fitrecord_data_h
#define psm_fitrecord_data_h
#ifdef __cplusplus
#if      __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */ 
#include "psm_base.h"

/**
 *@brief 常量表名
 *
 */
static const char *psm_fitrecord_table_name= "fitrecord";

/**
 *@brief 最大相关字符串长度
 *
 */
#define PSM_MAX_FITRECORD_DATA_COUNT  100

typedef struct psm_fitrecord_data
{
    char _date[PSM_MAX_FITRECORD_DATA_COUNT]; // 记录日期
    char _duration[PSM_MAX_FITRECORD_DATA_COUNT]; // 健身时间长短
    char _note[PSM_MAX_FITRECORD_DATA_COUNT]; // 健身备注
    char _intensity[PSM_MAX_FITRECORD_DATA_COUNT]; // 健身强度
    char _type[PSM_MAX_FITRECORD_DATA_COUNT]; // 健身类型
    int  _key; //索引
} PsmFitrecordData;

PsmCode psmFitrecordDataInit(PsmDataStorage **_data_storage);
#ifdef __cplusplus
#if __cplusplus
    }
#endif
#endif /* __cplusplus */ 
#endif /* psm_fitrecord_data_h */
