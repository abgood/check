#ifndef CHECK_INFO
#define CHECK_INFO

/* site相关信息 */
typedef struct info_list {
    char *telecom_ip;       /* 电信ip */
    char *unicom_ip;        /* 联通ip */
    char *domain;           /* 域名后半段 */
    int site_id;            /* 区id,未绑定域名就是主区id,否则为从区id */
    int port;               /* 游戏端口 */
} *site_info;

#endif
