#include "check.h"

int main (int argc, char **argv) {
    loc_info player_info;
    site_info site_info;

    char site[LEN_16] = {0};
    char site_name[LEN_16] = {0};
    char site_id[LEN_16] = {0};
    char indepe_sql[LEN_256] = {0};
    char common_sql[LEN_256] = {0};

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
    printf("%s\n", player_info->address);

    /* query indepe table */
    sprintf(indepe_sql, "select * from %s where site like \'%%%s%%\'", INDEPE, site_name);
    indepe_res = quiry(indepe_sql);
    /* handle mysql_res */
    handle_indepe(site_info, site_name, atoi(site_id), indepe_res);

    // /* query indepe table */
    // sprintf(common_sql, "select * from %s where site_name like \'%s\'", INDEPE, site_name);
    // indepe_res = quiry(indepe_sql);
    // /* handle mysql_res */
    // handle_indepe(site_info, indepe_res);

    /* win program at the end of the return */
#ifdef WINDOWS
    getch();
#endif
    return 0;
}
