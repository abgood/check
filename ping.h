#ifndef CHECK_PING_H
#define CHECK_PING_H

/* icmp stat report struct */
typedef struct icmp_static_name {
    unsigned int send_num;
    unsigned int recv_num;
    float max_time;
    float min_time;
    float avg_time;
} icmp_static;

/* check ping */
void check_ping(char *, int);

#endif
