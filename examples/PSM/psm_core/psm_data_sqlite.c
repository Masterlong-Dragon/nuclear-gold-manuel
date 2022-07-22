/**
 * @file psm_data_sqlite.c
 * @author 龙家可
 * @brief 数据库接口封装
 * @version 0.3
 * @date 2022-07-13
 *
 * @copyright Copyright (c) 2022
 *
 */
#include "psm_data_sqlite.h"
#include "psm_data_system.h"
#include <string.h>
#include <stdio.h>

// 数据库接口封装
sqlite3 *db = NULL;

// 对应枚举类型到实际操作类型的映射表
char *data_type[] = {"NULL", "INTEGER", "REAL", "TEXT", "BLOB"};
char *order_type[] = {" ASC", " DESC"};
// 暂时废弃的宏 完成可变参数列表到sql语句的转换（特指属性列表）
/*
#define PARSE_TO_COMMAND(AP_SET, ARGS_COUNT, SQL, SPLIT, FORMAT_INTEGER, FORMAT_REAL, FORMAT_TEXT, FORMAT_BLOB) \
    {                                                                                                           \
        char *key = NULL;                                                                                       \
        PsmDataType type = PSM_NULL;                                                                            \
        for (int i = 0; i < (ARGS_COUNT); i++)                                                                  \
        {                                                                                                       \
            key = va_arg((AP_SET), char *);                                                                     \
            type = va_arg((AP_SET), PsmDataType);                                                               \
            if (type == PSM_NULL || key == NULL || key[0] == '\0')                                              \
                return PSM_ERROR;                                                                               \
            char buf[MAX_FILENAME_LENGTH] = {0};                                                                \
            switch (type)                                                                                       \
            {                                                                                                   \
            case PSM_INTEGER:                                                                                       \
            {                                                                                                   \
                sprintf(buf, (FORMAT_INTEGER), key, va_arg((AP_SET), int));                                     \
            }                                                                                                   \
            break;                                                                                              \
            case PSM_REAL:                                                                                          \
            {                                                                                                   \
                sprintf(buf, (FORMAT_REAL), key, va_arg((AP_SET), double));                                     \
                break;                                                                                          \
            }                                                                                                   \
            case PSM_TEXT:                                                                                          \
            {                                                                                                   \
                sprintf(buf, (FORMAT_TEXT), key, va_arg((AP_SET), char *));                                     \
                break;                                                                                          \
            }                                                                                                   \
            case PSM_BLOB:                                                                                          \
            {                                                                                                   \
                sprintf(buf, (FORMAT_TEXT), key, va_arg((AP_SET), char *));                                     \
                break;                                                                                          \
            }                                                                                                   \
            default:                                                                                            \
                return PSM_ERROR;                                                                               \
            }                                                                                                   \
            strcat(SQL, buf);                                                                                   \
            if (i != (ARGS_COUNT)-1)                                                                            \
                strcat(SQL, SPLIT);                                                                             \
        }                                                                                                       \
    }
*/

// 完成可变参数列表到sql语句的转换（特指属性列表）
// jump标志变量特性：确保属性参数为空时，跳过该组参数的执行
// 在业务逻辑中通过NULL_LABLE这一宏来标记属性参数为空，这样可以跳过该组参数的执行
// 在查询等操作中可以大大提升语句简洁性
#define PARSE_TO_COMMAND_NULL_JUMP(AP_SET, ARGS_COUNT, SQL, SPLIT, FORMAT_INTEGER, FORMAT_REAL, FORMAT_TEXT, FORMAT_BLOB) \
    {                                                                                                                     \
        char *key = NULL;                                                                                                 \
        PsmDataType type = PSM_NULL;                                                                                      \
        char last_jump = 1;                                                                                               \
        for (int i = 0; i < (ARGS_COUNT); i++)                                                                            \
        {                                                                                                                 \
            key = va_arg((AP_SET), char *);                                                                               \
            type = va_arg((AP_SET), PsmDataType);                                                                         \
            char jump = (type == PSM_NULL || key == NULL || key[0] == '\0');                                              \
            char buf[MAX_FILENAME_LENGTH] = {0};                                                                          \
            switch (type)                                                                                                 \
            {                                                                                                             \
            case PSM_INTEGER:                                                                                                 \
            {                                                                                                             \
                int temp = va_arg((AP_SET), int);                                                                         \
                if (jump)                                                                                                 \
                    continue;                                                                                             \
                sprintf(buf, (FORMAT_INTEGER), key, temp);                                                                \
            }                                                                                                             \
            break;                                                                                                        \
            case PSM_REAL:                                                                                                    \
            {                                                                                                             \
                double temp = va_arg((AP_SET), double);                                                                   \
                if (jump)                                                                                                 \
                    continue;                                                                                             \
                sprintf(buf, (FORMAT_REAL), key, temp);                                                                   \
                break;                                                                                                    \
            }                                                                                                             \
            case PSM_TEXT:                                                                                                    \
            {                                                                                                             \
                char *temp = va_arg((AP_SET), char *);                                                                    \
                if (jump)                                                                                                 \
                    continue;                                                                                             \
                sprintf(buf, (FORMAT_TEXT), key, temp);                                                                   \
                break;                                                                                                    \
            }                                                                                                             \
            case PSM_BLOB:                                                                                                    \
            {                                                                                                             \
                char *temp = va_arg((AP_SET), char *);                                                                    \
                if (jump)                                                                                                 \
                    continue;                                                                                             \
                sprintf(buf, (FORMAT_BLOB), key, temp);                                                                   \
                break;                                                                                                    \
            }                                                                                                             \
            default:                                                                                                      \
                return PSM_ERROR;                                                                                         \
            }                                                                                                             \
            if (!last_jump)                                                                                               \
                strcat(SQL, SPLIT);                                                                                       \
            last_jump = jump;                                                                                             \
            strcat(SQL, buf);                                                                                             \
        }                                                                                                                 \
    }

// 初始化数据库绑定
PsmCode psmInitSqliteBinding()
{
    // 指定的基本数据操作函数
    psmInitConnect = psmInitDb;
    psmCloseConnect = psmCloseDb;
    psmCreateTable = psmDbCreateTable;
    psmDropTable = psmDbDropTable;
    psmInsData = psmDbInsData;
    psmDelData = psmDbDelData;
    psmListData = psmDbListData;
    psmSrchDataRaw = psmDbSrchDataRaw;
    psmSrchWithMatches = psmDbSrchWithMatches;
    psmUpdData = psmDbUpdData;
    return PSM_OK;
}

// 初始化数据库连接
PsmCode psmInitDb(char *db_name, void *key)
{
    // 确认参数合法
    if (db_name == NULL || db_name[0] == '\0')
        return PSM_ERROR;
    size_t len = 0;
    PSM_CHECK_DB_EXCUTION(sqlite3_open(db_name, &db));
    if (key != NULL && ((char *)key)[0] != '\0')
    {
        len = strlen(key);
        // 如果key合法，尝试连接
        PSM_CHECK_DB_EXCUTION(sqlite3_key(db, key, len));
    }
    // 尝试创建表单检测数据库连接
    PSM_CHECK_DB_EXCUTION(sqlite3_exec(db, "CREATE TABLE IF NOT EXISTS Testing(Id INTEGER)", NULL, NULL, NULL));
    // 大小写不敏感
    PSM_CHECK_DB_EXCUTION(sqlite3_exec(db, "PRAGMA case_sensitive_like = 0", NULL, NULL, NULL));
    return PSM_OK;
}

// 关闭数据库
PsmCode psmCloseDb()
{
    // 确认参数合法
    if (db == NULL)
        return PSM_OK;
    // 关闭数据库连接
    PSM_CHECK_DB_EXCUTION(sqlite3_close(db));
    return PSM_OK;
}

// 创建数据表
PsmCode psmDbCreateTable(char *name, int args_count, ...)
{
    // 检查参数合法
    if (db == NULL)
        return PSM_ERROR;
    if (name == NULL || name[0] == '\0')
        return PSM_ERROR;
    // sql语句缓存
    char sql[MAX_FILENAME_LENGTH * MAX_FILENAME_LENGTH] = {0};
    // 创建表
    sprintf(sql, "CREATE TABLE IF NOT EXISTS %s(Id INTEGER PRIMARY KEY", name);
    // 处理可变参数列表
    va_list ap;
    va_start(ap, args_count);
    char *key = NULL;
    PsmDataType type = PSM_NULL;
    // 创建数据表的字段
    for (int i = 0; i < args_count; i++)
    {
        // 依次读取
        type = va_arg(ap, PsmDataType);
        key = va_arg(ap, char *);
        if (key == NULL || key[0] == '\0')
            return PSM_ERROR;
        if (type < PSM_NULL || type > PSM_BLOB)
            return PSM_ERROR;
        // 创建字段
        sprintf(sql, "%s, %s %s", sql, key, data_type[type]);
    }
    va_end(ap);
    strcat(sql, ")");
    // psmLog(sql);
    // 执行sql语句
    PSM_CHECK_DB_EXCUTION(sqlite3_exec(db, sql, NULL, NULL, NULL));
    return PSM_OK;
}

// 删除数据表
PsmCode psmDbDropTable(char *name)
{
    // 检查参数合法
    if (db == NULL)
        return PSM_ERROR;
    if (name == NULL || name[0] == '\0')
        return PSM_ERROR;
    // sql语句缓存
    char sql[MAX_FILENAME_LENGTH * MAX_FILENAME_LENGTH] = {0};
    // 删除表
    sprintf(sql, "DROP TABLE IF EXISTS %s", name);
    // psmLog(sql);
    // 执行sql语句
    PSM_CHECK_DB_EXCUTION(sqlite3_exec(db, sql, NULL, NULL, NULL));
    return PSM_OK;
}

// 插入数据
PsmCode psmDbInsData(char *name, int args_count, ...)
{
    // 检查参数合法
    if (db == NULL)
        return PSM_ERROR;
    if (name == NULL || name[0] == '\0')
        return PSM_ERROR;
    // 创建插入数据的sql语句
    char sql[MAX_FILENAME_LENGTH * MAX_FILENAME_LENGTH] = {0};
    sprintf(sql, "INSERT INTO %s (", name);
    va_list ap;
    va_start(ap, args_count);
    char *key = NULL;
    PsmDataType type = PSM_NULL;
    // 拼接sql语句
    for (int i = 0; i < args_count; i++)
    {
        key = va_arg(ap, char *);
        if (key == NULL || key[0] == '\0')
            return PSM_ERROR;
        strcat(sql, key);
        if (i != args_count - 1)
            strcat(sql, ", ");
    }
    strcat(sql, ") VALUES (");
    // 拼接sql语句
    for (int i = 0; i < args_count; i++)
    {
        type = va_arg(ap, PsmDataType);
        char buf[MAX_FILENAME_LENGTH] = {0};
        switch (type)
        {
        case PSM_INTEGER:
            sprintf(buf, "%d", va_arg(ap, int));
            break;
        case PSM_REAL:
            sprintf(buf, "%f", va_arg(ap, double));
            break;
        case PSM_TEXT:
            sprintf(buf, "'%s'", va_arg(ap, char *));
            break;
        case PSM_BLOB:
            sprintf(buf, "'%s'", va_arg(ap, char *));
            break;
        default:
            return PSM_ERROR;
            break;
        }
        strcat(sql, buf);
        if (i != args_count - 1)
            strcat(sql, ", ");
    }
    va_end(ap);
    strcat(sql, ")");
    // psmLog(sql);
    // 执行sql语句
    PSM_CHECK_DB_EXCUTION(sqlite3_exec(db, sql, NULL, NULL, NULL));
    return PSM_OK;
}

// 查找
PsmCode psmDbSrchDataRaw(char *name, char *sql, PsmDataSrched *result)
{
    // 检查参数合法
    if (db == NULL)
        return PSM_ERROR;
    if (name == NULL || name[0] == '\0')
        return PSM_ERROR;
    if (sql == NULL || sql[0] == '\0')
        return PSM_ERROR;
    if (result == NULL)
        return PSM_ERROR;
    // 执行sql语句
    PSM_CHECK_DB_EXCUTION(sqlite3_get_table(db, sql, &(result->_resultp), &result->_nrow, &result->_ncolumn, NULL));
    return PSM_OK;
}

// 列举
PsmCode psmDbListData(char *name, PsmDataSrchOrder order, PsmDataSrched *result)
{
    // 检查参数合法
    if (db == NULL)
        return PSM_ERROR;
    if (name == NULL || name[0] == '\0')
        return PSM_ERROR;
    if (result == NULL)
        return PSM_ERROR;
    char sql[MAX_FILENAME_LENGTH * MAX_FILENAME_LENGTH] = {0};
    // 列举数据表的sql语句
    sprintf(sql, "SELECT * FROM %s", name);
    if (order._column != NULL)
    {
        strcat(sql, " ORDER BY ");
        strcat(sql, order._column);
        strcat(sql, order_type[order._order]);
    }
    return psmSrchDataRaw(name, sql, result);
}

// 简单属性匹配
PsmCode psmSrchByCol(char *name, char *col, char *content, PsmDataSrched *result)
{
    // 检查参数合法
    if (db == NULL)
        return PSM_ERROR;
    if (name == NULL || name[0] == '\0')
        return PSM_ERROR;
    if (col == NULL || col[0] == '\0')
        return PSM_ERROR;
    if (content == NULL || content[0] == '\0')
        return PSM_ERROR;
    if (result == NULL)
        return PSM_ERROR;
    // 查找数据的sql语句
    char sql[MAX_FILENAME_LENGTH * MAX_FILENAME_LENGTH] = {0};
    sprintf(sql, "SELECT * FROM %s WHERE %s = '%s'", name, col, content);
    return psmSrchDataRaw(name, sql, result);
}

// 复杂属性匹配
PsmCode psmSrchWithCondition(char *name, char *condition, PsmDataSrched *result)
{
    // 检查参数合法
    if (db == NULL)
        return PSM_ERROR;
    if (name == NULL || name[0] == '\0')
        return PSM_ERROR;
    if (condition == NULL || condition[0] == '\0')
        return PSM_ERROR;
    if (result == NULL)
        return PSM_ERROR;
    // 查找数据的sql语句
    char sql[MAX_FILENAME_LENGTH * MAX_FILENAME_LENGTH] = {0};
    sprintf(sql, "SELECT * FROM %s WHERE %s", name, condition);
    return psmSrchDataRaw(name, sql, result);
}

// 复杂属性匹配
PsmCode psmDbSrchWithMatches(char *name, PsmDataSrchOrder order, PsmDataSrch mode, PsmDataSrched *result, int args_count, ...)
{
    // 检查参数合法
    if (db == NULL)
        return PSM_ERROR;
    if (name == NULL || name[0] == '\0')
        return PSM_ERROR;
    if (result == NULL)
        return PSM_ERROR;
    if (args_count == 0)
        return PSM_ERROR;
    // 拼接sql语句
    char sql[MAX_FILENAME_LENGTH * MAX_FILENAME_LENGTH] = {0};
    va_list ap_set;
    va_start(ap_set, args_count);
    // 拼接sql语句
    switch (mode)
    {
    case FULL_MATCH:
        PARSE_TO_COMMAND_NULL_JUMP(ap_set, args_count, sql, " AND ", "%s = %d", "%s = %f", "%s = '%s'", "%s = '%s'");
        break;
    case PARTIAL_MATCH:
        PARSE_TO_COMMAND_NULL_JUMP(ap_set, args_count, sql, " AND ", "%s like %d", "%s like %f", "%s like '%%%s%%'", "%s like '%%%s%%'");
    default:
        break;
    }
    va_end(ap_set);
    if (order._column != NULL)
    {
        strcat(sql, " ORDER BY ");
        strcat(sql, order._column);
        strcat(sql, order_type[order._order]);
    }

    return psmSrchWithCondition(name, sql, result);
}

// 更新数据
PsmCode psmDbUpdData(char *name, int args_count_set, int args_count_where, ...)
{
    // 检查参数合法
    if (db == NULL)
        return PSM_ERROR;
    if (name == NULL || name[0] == '\0')
        return PSM_ERROR;
    char sql[MAX_FILENAME_LENGTH * MAX_FILENAME_LENGTH] = {0};
    sprintf(sql, "UPDATE %s SET ", name);
    va_list ap_set;
    va_start(ap_set, args_count_where);
    // 拼接sql语句
    PARSE_TO_COMMAND_NULL_JUMP(ap_set, args_count_set, sql, ", ", "%s = %d", "%s = %f", "%s = '%s'", "%s = '%s'");
    strcat(sql, " WHERE ");
    // 拼接sql语句
    PARSE_TO_COMMAND_NULL_JUMP(ap_set, args_count_where, sql, " AND ", "%s = %d", "%s = %f", "%s = '%s'", "%s = '%s'");
    va_end(ap_set);
    // psmLog(sql);
    // 执行sql语句
    PSM_CHECK_DB_EXCUTION(sqlite3_exec(db, sql, NULL, NULL, NULL));
    return PSM_OK;
}

// 删除数据
PsmCode psmDbDelData(char *name, int args_count, ...)
{
    // 检查参数合法
    if (db == NULL)
        return PSM_ERROR;
    if (name == NULL || name[0] == '\0')
        return PSM_ERROR;
    // 拼接sql语句
    char sql[MAX_FILENAME_LENGTH * MAX_FILENAME_LENGTH] = {0};
    sprintf(sql, "DELETE FROM %s WHERE ", name);
    va_list ap;
    va_start(ap, args_count);
    char *key = NULL;
    PsmDataType type = PSM_NULL;
    // 拼接sql语句
    PARSE_TO_COMMAND_NULL_JUMP(ap, args_count, sql, " AND ", "%s = %d", "%s = %f", "%s = '%s'", "%s = '%s'");
    va_end(ap);
    // psmLog(sql);
    // 执行sql语句
    PSM_CHECK_DB_EXCUTION(sqlite3_exec(db, sql, NULL, NULL, NULL));
    return PSM_OK;
}

// 获取数据库指针
PsmCode psmGetDb(sqlite3 **_db)
{
    if (db == NULL)
        return PSM_ERROR;
    *_db = db;
    return PSM_OK;
}