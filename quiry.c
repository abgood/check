#include "check.h"

MYSQL_RES *quiry(char *sql) {
    MYSQL *conn;
    MYSQL_RES *res;

    char *ip = "192.168.56.101";
    char *db_pawd = "123456";
    int port = 3306;

    conn = mysql_init(NULL);

    if (!(mysql_real_connect(conn, ip, "root", db_pawd, "web_check", port, NULL, 0))) {
        if (mysql_error(conn))
            fprintf(stderr, "connection error %d : %s\n", mysql_errno(conn), mysql_error(conn));
        printf("Fail to connect mysql, ip:%s\tport:%d\n", ip, port);
        exit(1);
    }

    // 查询数据库
    if (mysql_query(conn,sql))
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

    mysql_close(conn);
    return res;
}
