#include "check.h"

void handle_indepe(site_info output, char *input_name, int input_id, MYSQL_RES *res) {
    MYSQL_ROW row;
    char agent[LEN_128] = {0};
    char m_name[LEN_16] = {0};
    char m_id[LEN_8] = {0};
    char id_token_start[LEN_16] = {0};
    char id_token_end[LEN_16] = {0};
    char id_start_tmp[LEN_64] = {0};
    char *s_n_end;
    char *s_i_end;
    char *i_n_start;
    char *m_i_start;
    char *id_start;
    char *token;
    int int_id_start;
    int int_id_end;

    /* site_info 动态分配site_name内存 */
    output->site_name = (char *)malloc(LEN_16 * sizeof(char));

    while ((row = mysql_fetch_row(res))) {
        memset(output->site_name, '\0', LEN_16);
        memset(agent, '\0', LEN_128);
        strncpy(agent, row[1], strlen(row[1]));
        
        /* get master site_name */
        if ((s_n_end = strstr(agent, D_SITE_NAME))) {
            strncpy(m_name, agent, s_n_end - agent);
        }
        strncpy(output->site_name, m_name, strlen(m_name));

        /* get master site_id */
        if ((s_i_end = strstr(agent, D_SITE_ID))) {
            strncpy(m_id, s_n_end + 1, s_i_end - s_n_end - 1);
        }
        output->site_id = atoi(m_id);

        /* find input_id in record */
        i_n_start = strstr(agent, input_name);
        m_i_start = strstr(i_n_start, D_SITE_NAME);
        /* find each site_name from id to start */
        if ((id_start = strstr(m_i_start, D_SITE_ID))) {
            id_start += 1;
        } else {
            id_start = m_i_start + 1;
        }

        /* if have "~", compare id fields */
        if (!strstr(id_start, D_ID_RANGE)) {
            if (input_id == atoi(id_start)) {
                printf("%s\n", agent);
            }
            continue;
        } else {
            strncpy(id_start_tmp, id_start, strlen(id_start));
            token = strtok(id_start_tmp, D_ID_FIELD);
            while (token) {
                sscanf(token, "%[^~]~%[^;];", id_token_start, id_token_end);

                int_id_start = atoi(id_token_start);
                int_id_end = atoi(id_token_end);

                /* get ip */
                if (input_id >= int_id_start && input_id <= int_id_end) {
                    output->telecom_ip = row[2];
                    output->unicom_ip = row[3];
                    output->port = atoi(row[4]);
                    output->resource = atoi(row[5]);
                }

                token = strtok(NULL, D_ID_FIELD);
            }
        }
    }
}
