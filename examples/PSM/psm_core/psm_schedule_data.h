/**
 *@file psm_schedule_data.h
 *@brief 日程管理系统
 *@version 0.1
 *@date 2022-07-14
 *
 *@copyright Copyright (c) 2022
 *
 */

#ifndef psm_schedule_data_h
#define psm_schedule_data_h
#ifdef __cplusplus
#if      __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */ 

#include "psm_base.h"
#include "psm_data_system.h"
#include "psm_data_sqlite.h"

/**
 *@brief 常量表名
 *
 */
static const char *psm_schedule_data_table_name= "schedule_data";

/**
 *@brief 最大相关字符串长度
 *
 */
#define PSM_MAX_SCHEDULE_DATA_COUNT  100

typedef struct psm_schedule_data
{
    char _name[PSM_MAX_SCHEDULE_DATA_COUNT]; // 姓名
    char _time[PSM_MAX_SCHEDULE_DATA_COUNT]; // 时间
    char _address[PSM_MAX_SCHEDULE_DATA_COUNT]; // 地址
    int  _key; //索引
} PsmScheduleData;

PsmCode psmScheduleDataInit(PsmDataStorage **_data_storage);

#ifdef __cplusplus
#if __cplusplus
    }
#endif
#endif /* __cplusplus */ 

#endif /* psm_schedule_data_h */
