#include "check.h"

/* 查询结果 */
MYSQL_RES *mysql_result(MYSQL *conn, char *string) {
    MYSQL_RES *res;

    // 查询数据库
    if (mysql_query(conn,string))
    {
        fprintf(stderr, "查询失败\n");
        return NULL;
    }
    // 生成结果集
    if (!(res=mysql_store_result(conn)))
    {
        fprintf(stderr, "结果集生成失败\n");
        return NULL;
    }

    return res;
}

/* 查询site_id结果 */
void handle_site_id(site_info info, MYSQL_RES *site_name_result, int site_id) {
    char agent[LEN_64] = {0};
    MYSQL_ROW row;
    int info_cols;
    int site_id_start;
    int site_id_end;

    info_cols = mysql_num_fields(site_name_result);
    while ((row = mysql_fetch_row(site_name_result))) {
        strncpy(agent, row[1], strlen(row[1]));
        printf("%s\n", agent);
    }
}

site_info collect_info(char *site) {
    MYSQL mysql, *conn;
    MYSQL_RES *site_name_result;
    char *site_name;
    char *site_id_tmp;
    int site_id;
    char site_name_sql[LEN_1024] = {0};

    /* 动态分配site相关信息内存 */
    site_info info;
    if ((info = malloc(sizeof(struct info_list))) == NULL) {
        printf("info构动态分配内存错误\n");
        exit(1);
    }

    /* 数据库信息 */
    const char *ip = "192.168.56.101";
    const char *db_pawd = "123456";
    int port = 3306;

    /* 临时site */
    char site_tmp[LEN_16] = {0};
    strncpy(site_tmp, site, strlen(site));

    /* 获得site_name和site_id */
    site_name = strtok(site_tmp, SITE_DELIMITER);
    site_id_tmp = strtok(NULL, SITE_DELIMITER);
    site_id = atoi(site_id_tmp);

    /* 初始mysql句柄结构 */
    mysql_init(&mysql);

    /* 连接mysql */
    mysql_init(&mysql);
    if ((conn = mysql_real_connect(&mysql, ip, "root", db_pawd, "web_check", port, NULL, 0)) == NULL) {
        if (mysql_error(&mysql))
            fprintf(stderr, "connection error %d : %s\n", mysql_errno(&mysql), mysql_error(&mysql));
        printf("Fail to connect mysql, ip:%s\tport:%d\n", ip, port);
        exit(1);
    }

    /* 查询site_name结果 */
    sprintf(site_name_sql, "select * from %s where site like \'%%%s%%\'", INDEPENDENT, site_name);
    site_name_result = mysql_result(conn, site_name_sql);

    /* 查询site_id结果 */
    handle_site_id(info, site_name_result, site_id);

    return info;
}
