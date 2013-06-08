#include "check.h"

int main (int argc, char **argv) {
    char error_site[LEN_16] = {0};
    loc_info player_info;

    /* init player_info */
    if (!(player_info = malloc(sizeof(struct local_info)))) {
        fprintf(stderr, "player_info struct malloc memory error\n");
        exit(1);
    }

    printf("Please enter the game site if you have any questions: ");
    scanf("%s", error_site);

    /* curl html to get player information */
    curl_data(player_info);

    /* win program at the end of the return */
#ifdef WINDOWS
    getch();
#endif
    return 0;
}
