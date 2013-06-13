#include "check.h"

/* 域名解析检查 */
void chk_resolve(char *domain, char *telecom_ip, char *unicom_ip, char *agent, char *prefix) {
    /* get local domain resolve ip */
    struct hostent *host_name;
    char *host_ip;

    if ((host_name = gethostbyname(domain)) == NULL) {
        fprintf(stderr, "gethostbyname fail!!!\n");
        exit(1);
    }

    /* player local resolve ip */
    host_ip = inet_ntoa(*((struct in_addr *)host_name->h_addr));

    if (strcmp(agent, "电信") == 0) {
        if (strcmp(host_ip, telecom_ip) == 0) {
            printf("%s domain resolve %s ip, success!!!\n\n", prefix, agent);
        }

        if (strcmp(host_ip, unicom_ip) == 0) {
            printf("%s domain not resolve %s ip, failed!!!\n", prefix, agent);
            printf("resolve: %s %s\n\n", telecom_ip, domain);
            exit(1);
        }
    }

    if (strcmp(agent, "联通") == 0) {
        if (strcmp(host_ip, unicom_ip) == 0) {
            printf("%s domain resolve %s ip, success!!!\n\n", prefix, agent);
        }

        if (strcmp(host_ip, telecom_ip) == 0) {
            printf("%s domain not resolve %s ip, failed!!!\n", prefix, agent);
            printf("resolve: %s %s\n\n", unicom_ip, domain);
            exit(1);
        }
    }

}

/* domain resolve */
void check_resolve(site_info info, loc_info player) {
    char s_domain[LEN_32] = {0};
    char res_domain[LEN_32] = {0};
    char ass_domain[LEN_32] = {0};
    char domain[LEN_32] = {0};

    strncpy(domain, info->domain, strlen(info->domain));

    sprintf(s_domain, "%s%d%s", S_PREFIX, info->site_id, domain);
    sprintf(res_domain, "%s%d%s", RES_PREFIX, info->site_id, domain);
    sprintf(ass_domain, "%s%d%s", ASS_PREFIX, info->site_id, domain);

    /* s域名解析检查 */
    chk_resolve(s_domain, info->telecom_ip, info->unicom_ip, player->agent, S_PREFIX);

    /* res域名解析检查 */
    chk_resolve(res_domain, info->telecom_ip, info->unicom_ip, player->agent, RES_PREFIX);

    /* res域名解析检查 */
    chk_resolve(ass_domain, info->telecom_ip, info->unicom_ip, player->agent, ASS_PREFIX);
}
