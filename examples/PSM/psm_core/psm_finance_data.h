#ifndef PSM_FINANCE_DATA_H
#define PSM_FINANCE_DATA_H
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
static const char *psm_finance_table_name = "finance";
#define PSM_MAX_FINANCE_LENGTH 64
typedef enum psm_finance_data_type
{
    PSM_FINANCE_DATA_TYPE_INCOME = 1,  // 收入
    PSM_FINANCE_DATA_TYPE_EXPENSE = 2, // 支出
} PsmFinanceDataType;

typedef struct psm_finance_data
{
    PsmFinanceDataType _type;           // 类型
    char _date[PSM_MAX_FINANCE_LENGTH]; // 日期
    char _name[PSM_MAX_FINANCE_LENGTH]; // 名称
    double _amount;                      // 金额
    char _note[PSM_MAX_FINANCE_LENGTH]; // 备注
    int _key;
} PsmFinanceData;

PsmCode psmFinanceDataInit(PsmDataStorage **_data_storage);
#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */ 
#endif