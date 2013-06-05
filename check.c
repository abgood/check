#include "check.h"

int main (int argc, char **argv) {
    char site[LEN_16] = {0};
    site_info site_info;

    /* 输入site */
    printf("请输入登陆异常的游戏site(eg. 37wan_0001): ");
    scanf("%s", site);

    /* site相关信息 */
    if (!(site_info = collect_info(site))) {
        printf("site信息查询失败,请检查\n");
        exit(1);
    }

    printf("%s\n", site_info->telecom_ip);
    printf("%s\n", site_info->unicom_ip);
    printf("%s\n", site_info->domain);
    printf("%d\n", site_info->port);
    printf("%d\n", site_info->bind);
    printf("%d\n", site_info->resource);
    return 0;
}
