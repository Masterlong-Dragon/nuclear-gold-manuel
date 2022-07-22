/**
 * @file psm_base.c
 * @author 龙家可
 * @brief 基础接口
 * @version 0.1
 * @date 2022-07-12
 *
 * @copyright Copyright (c) 2022
 *
 */
#include "psm_base.h"
#include <string.h>

// 输出对象
FILE *psm_log;
FILE *psm_err_log;

// 初始化开发环境
PsmCode psmInitSys()
{
    // 常规默认定义
    psm_log = stdout;
    psm_err_log = stderr;
    return PSM_OK;
}

// 设置有效输出对象
PsmCode psmSetLog(FILE *fp)
{

    if (fp == NULL)
        return PSM_ERROR;
    psm_log = fp;
    return PSM_OK;
}

// 设置有效输出对象
PsmCode psmSetErrLog(FILE *fp)
{

    if (fp == NULL)
        return PSM_ERROR;
    psm_err_log = fp;
    return PSM_OK;
}

// 打印日志
int psmLog(char *format, ...)
{
    int ret = 0;
    va_list args;
    va_start(args, format);
    ret = vfprintf(psm_log, format, args);
    va_end(args);
    fprintf(psm_log, "\n");
    return ret;
}

// 打印错误日志
int psmErrLog(char *format, ...)
{
    int ret = 0;
    va_list args;
    va_start(args, format);
    ret = vfprintf(psm_err_log, format, args);
    va_end(args);
    fprintf(psm_err_log, "\n");
    return ret;
}
