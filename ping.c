#include "check.h"

void chk_ping(char *domain) {
    printf("%s\n", domain);
}

void check_ping(site_info info, char *domain) {
    char s_domain[LEN_32] = {0};

    sprintf(s_domain, "%s%d%s", S_PREFIX, info->site_id, domain);

    /* res domain ping */
    if (info->resource) {
        chk_ping(CDN);
    }
    /* s domain ping */
    chk_ping(s_domain);
}
