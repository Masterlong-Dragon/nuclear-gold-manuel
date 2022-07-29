/**
 * @file psm_base.h
 * @brief 基础接口
 * @version 0.1
 * @date 2022-07-12
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#ifndef PSM_CONFIG
#define PSM_CONFIG
#ifdef __cplusplus
#if      __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */ 
#define MAX_FILENAME_LENGTH 50 // 最大表名

#define SQLITE_ENABLED

#define PSM_ERROR 0 // 错误码
#define PSM_OK 1    // 成功码
#include <stdarg.h>
#include <stdio.h>
/**
 * @brief
 * 数据基本种类
 */
typedef unsigned char PsmCode;              // 错误消息
typedef unsigned long long PsmDataSize;     // 表内元素数量
typedef struct psm_data_sys PsmDataStorage; // 基础数据表单

/**
 * @brief 打印普通日志
 * 
 * @param format 格式化字符串
 * @param ... 参数列表
 * @return int 
 */
int psmLog(char *format, ...);

/**
 * @brief 打印错误日志
 * 
 * @param format 格式化字符串
 * @param ... 参数列表
 * @return int 
 */
int psmErrLog(char *format, ...);

/**
 * @brief 初始化开发环境
 * 
 * @return PsmCode 执行码
 */
PsmCode psmInitSys();

/**
 * @brief Set the Log object
 * 
 * @param fp 定向对象
 * @return PsmCode 执行码
 */
PsmCode psmSetLog(FILE *fp);

/**
 * @brief Set the Err Log object
 * 
 * @param fp 定向对象
 * @return PsmCode 执行码
 */
PsmCode psmSetErrLog(FILE *fp);
#ifdef __cplusplus
#if __cplusplus
    }
#endif
#endif /* __cplusplus */ 
#endif