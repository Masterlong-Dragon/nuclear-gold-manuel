#include "psm_data_system.h"
/**
 * @brief �������ݿ�����
 *
 */
PsmCode(*psmInitConnect)(char* db_name, void* key);

/**
 * @brief �ر����ݿ�
 *
 * @param db_name ���ݿ�����
 * @param key ����
 * @return PsmCode ִ����
 */
PsmCode(*psmCloseConnect)();

/**
 * @brief ������
 * �ɱ�������������ƺ��������ͣ�˳��Ϊ�������ԣ������ƣ������ԣ������ƣ�...
 * ��Ҫ��ָ�����������磺psmCreateTable("table_name", 2, "id", PSM_TEXT, "name", PSM_TEXT);
 *
 * @param name ������
 * @param args_count ��������
 * @param ... �����б�
 * @return PsmCode ִ����
 */
PsmCode(*psmCreateTable)(char* name, int args_count, ...);

/**
 * @brief ɾ����
 * ��Ҫ�ṩ�������
 *
 * @param name ������
 * @return PsmCode ִ����
 */
PsmCode(*psmDropTable)(char* name);

/**
 * @brief ��������
 * �ɱ�����������������У��������ͺ����ƣ�˳��Ϊ����˳�����������У����������������ͣ��������ƣ��������ͣ��������ƣ�...
 * ���磺psmDbInsert("table_name", 2, "id", "name", PSM_TEXT, "1", PSM_TEXT, "����");
 *
 * @param name ������
 * @param args_count ��������
 * @param ... �����б�
 * @return PsmCode ִ����
 */
PsmCode(*psmInsData)(char* name, int args_count, ...);

/**
 * @brief ��������
 * �ȴ�����²��ֵĲ����������ٴ�������ɸѡ���ֵĲ�������
 * ����˳������²��ֵĲ������ٰ���˳��������ɸѡ���ֵĲ���
 * ���磺psmDbUpdate("table_name", 1, 1, "name", PSM_TEXT, "����", "name", PSM_TEXT, "����");
 *
 * @param name ������
 * @param key �ؼ��֣�ͨ��NULLȱʡ��
 * @param content ����
 * @param args_count ��������
 * @param ... �����б�
 * @return PsmCode ִ����
 */
PsmCode(*psmUpdData)(char* name, int args_count_set, int args_count_where, ...);

/**
 * @brief ֱ��ͨ��sql�����ѯ��ñ��������ص���һ�������
 * ���⣬��һ��ƫ�ײ�Ĳ�����������ʹ��
 *
 * @param name ������
 * @param sql sql����
 * @param result �����
 * @return PsmCode ִ����
 */
PsmCode(*psmSrchDataRaw)(char* name, char* sql, PsmDataSrched* result);

/**
 * @brief ��������ƥ���ѯ
 * �ɱ����������ѯ������˳��Ϊ�������ƣ������ԣ�ƥ�����ݣ������ƣ������ԣ�ƥ�����ݣ�...
 * ���磺psmDbSrchWithCondition("table_name", 2, "name", PSM_TEXT, "����", "age", PSM_INTEGER, 20);
 *
 * @param name ������
 * @param order ˳��
 * @param result �����
 * @param args_count �ɱ��������
 * @param ... �����б�
 * @return PsmCode ִ����
 */
PsmCode(*psmSrchWithMatches)(char* name, PsmDataSrchOrder order, PsmDataSrch mode, PsmDataSrched* result, int args_count, ...);

/**
 * @brief �г�����ȫ�����ݣ����ص���һ�������
 *
 * @param name ������
 * @param order ˳��
 * @param result �����
 * @return PsmCode ִ����
 */
PsmCode(*psmListData)(char* name, PsmDataSrchOrder order, PsmDataSrched* result);

/**
 * @brief ɾ������
 * �ɱ��������ɾ��������˳��Ϊ�������ƣ������ԣ�ƥ�����ݣ������ƣ������ԣ�ƥ�����ݣ�...
 * ���磺psmDbDelData("table_name", 2, "name", PSM_TEXT, "����", "age", PSM_INTEGER, 20);
 *
 * @param name ������
 * @param args_count ��������
 * @param ... �����б�
 * @return PsmCode ִ����
 */
PsmCode(*psmDelData)(char* name, int args_count, ...);