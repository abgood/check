#include "check.h"

void chk_ping(char *domain) {
    // printf("%s\n", domain);
}

void check_ping(char *domain, int res_flag) {
    /* res domain ping */
    if (res_flag) {
        chk_ping(CDN);
    }
    /* s domain ping */
    chk_ping(domain);
}
