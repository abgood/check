#include "check.h"

int main (int argc, char **argv) {
    loc_info player_info;
    site_info site_info;
    my_ulonglong lines;

    char site[LEN_16] = {0};
    char site_name[LEN_16] = {0};
    char site_id[LEN_16] = {0};
    char indepe_sql[LEN_256] = {0};
    char common_sql[LEN_256] = {0};
    char domain[LEN_32] = {0};

    MYSQL_RES *indepe_res;
    MYSQL_RES *common_res;

    /* init player_info */
    if (!(player_info = malloc(sizeof(struct local_info)))) {
        fprintf(stderr, "player_info struct malloc memory error\n");
        exit(1);
    }

    /* init site_info */
    if (!(site_info = malloc(sizeof(struct info_list)))) {
        fprintf(stderr, "site_info struct malloc memory error\n");
        exit(1);
    }

    printf("Please enter the game site if you have any questions: ");
    scanf("%s", site);

    sscanf(site, "%[^_]%*c%s", site_name, site_id);

    /* curl html to get player information */
    curl_data(player_info);
    remove(TMP_FILE);

    /* query indepe table */
    sprintf(indepe_sql, "select * from %s where site like \'%%%s%%\'", INDEPE, site_name);
    indepe_res = quiry(indepe_sql);
    if ((lines = mysql_num_rows(indepe_res)) == (my_ulonglong)0) {
        printf("Don\'t find you input %s in %s table!!!\n", site, INDEPE);
        exit(1);
    }
    /* handle mysql_res */
    handle_indepe(site_info, site_name, atoi(site_id), indepe_res);

    if (strlen(site_info->telecom_ip) < LEN_8) {
        printf("%s for %s not found!!!\n", site_name, site_id);
        exit(1);
    }

    /* query common table */
    sprintf(common_sql, "select * from %s where site_name like \'%%%s%%\' or site_name like \'%%%s%%\'", COMMON, site_info->site_name, site_name);
    common_res = quiry(common_sql);
    if ((lines = mysql_num_rows(common_res)) == (my_ulonglong)0) {
        printf("Don\'t find you input %s in %s table!!!\n", site, COMMON);
        exit(1);
    }
    /* handle mysql_res */
    handle_common(site_info, site_name, atoi(site_id), common_res);

    strncpy(domain, site_info->domain, strlen(site_info->domain));

    /* domain resolve */
    check_resolve(site_info, player_info);

    /* check ping */
    check_ping(site_info, domain);

    /* win program at the end of the return */
#ifdef WINDOWS
    getch();
#endif
    return 0;
}
