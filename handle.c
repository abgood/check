#include "check.h"

void handle_indepe(site_info output, char *input_name, int input_id, MYSQL_RES *res) {
    MYSQL_ROW row;
    char agent[LEN_32] = {0};
    char m_name[LEN_16] = {0};
    char m_id[LEN_8] = {0};
    char *s_n_end;
    char *s_i_end;

    while ((row = mysql_fetch_row(res))) {
        strncpy(agent, row[1], strlen(row[1]));
        
        /* get master site_name */
        if ((s_n_end = strstr(agent, D_SITE_NAME))) {
            strncpy(m_name, agent, s_n_end - agent);
        }
        output->site_name = m_name;

        /* get master site_id */
        if ((s_i_end = strstr(agent, D_SITE_ID))) {
            strncpy(m_id, s_n_end + 1, s_i_end - s_n_end - 1);
        }
        output->site_id = atoi(m_id);

        /* find input_id in record */
    }
}
