#include "check.h"

icmp_static icmp_stat;
static unsigned char send_buff[LEN_1024 * LEN_1024] = {0};
static unsigned char recv_buff[LEN_1024 * LEN_1024] = {0};

struct timeval send_time = {0};
struct timeval recv_time = {0};

#ifdef WINDOWS
typedef struct icmp 
{
    BYTE   icmp_type;
    BYTE   icmp_code;                 // Type sub code
    USHORT icmp_cksum;
    USHORT icmp_id;
    USHORT icmp_seq;
    ULONG  timestamp;
} icmp;

// IP header structure
typedef struct ip
{
    //Suppose the BYTE_ORDER is LITTLE_ENDIAN
    unsigned int   ip_hl:4;        // Length of the header
    unsigned int   version:4;      // Version of IP
    unsigned char  tos;            // Type of service
    unsigned short total_len;      // Total length of the packet
    unsigned short id;             // Unique identification
    unsigned short frag_offset;    // Fragment offset
    unsigned char  ip_ttl;            // Time to live
    unsigned char  protocol;       // Protocol (TCP, UDP etc)
    unsigned short checksum;       // IP checksum
    unsigned int   sourceIP;       // Source IP
    unsigned int   destIP;         // Destination IP
} ip;

int gettimeofday(struct timeval *tp, void *tzp)
{
    time_t clock;
    struct tm tm;
    SYSTEMTIME wtm;
    GetLocalTime(&wtm);
    tm.tm_year     = wtm.wYear - 1900;
    tm.tm_mon     = wtm.wMonth - 1;
    tm.tm_mday     = wtm.wDay;
    tm.tm_hour     = wtm.wHour;
    tm.tm_min     = wtm.wMinute;
    tm.tm_sec     = wtm.wSecond;
    tm. tm_isdst    = -1;
    clock = mktime(&tm);
    tp->tv_sec = clock;
    tp->tv_usec = wtm.wMilliseconds * 1000;
    return (0);
}
#endif

/* calc icmp check sum */
unsigned short calc_icmp_cksum(const void *packet, int len) {
    unsigned short chksum = 0;
    unsigned short *offset = NULL;
    offset = (unsigned short *)packet;

    while (len > 1) {
        chksum += *offset++;
        len -= sizeof(unsigned short);
    }

    if (len == 1) {
        chksum += *((char *)offset);
    }

    chksum = (chksum >> 16) + (chksum & 0xffff);
    chksum += (chksum >> 16);

    return ~chksum;
}

/* fill in message */
int new_icmp(const int num, const int len) {
    struct icmp *p_icmp = NULL;
    memset(send_buff, 0, sizeof(send_buff));
    p_icmp = (struct icmp *)send_buff;
    p_icmp->icmp_type = ICMP_ECHO;
    p_icmp->icmp_code = 0;
    p_icmp->icmp_seq = htons((unsigned short)num);
    p_icmp->icmp_id = htons((unsigned short)getpid());
    p_icmp->icmp_cksum = 0;
    p_icmp->icmp_cksum = calc_icmp_cksum(p_icmp, len + 8);
    return len + 8;
}

/* calc time subversion */
unsigned int time_sub(const struct timeval *recv_time, const struct timeval *send_time) {
    unsigned int sec = 0;
    int u_sec = 0;

    sec = recv_time->tv_sec - send_time->tv_sec;
    u_sec = recv_time->tv_usec - send_time->tv_usec;

    if (u_sec < 0) {
        u_sec += 1000000;
        sec--;
    }

    return sec * 1000 + (unsigned int)(u_sec / 1000);
}

/* parse icmp message */
int parse_icmp(const struct sockaddr_in *from_addr, char *recv_buff, const int recv_len) {
    int ip_head_len = 0;
    int icmp_len = 0;
    struct ip *p_ip = NULL;
    struct icmp *p_icmp = NULL;

    p_ip = (struct ip *)recv_buff;
    ip_head_len = p_ip->ip_hl << 2;
    p_icmp = (struct icmp *)(recv_buff + ip_head_len);
    icmp_len = recv_len - ip_head_len;

    if (icmp_len < 8) {
        printf("Bad icmp echo-reply\n");
        return -1;
    }

    if ((p_icmp->icmp_type != ICMP_ECHOREPLY) || (p_icmp->icmp_id != htons((unsigned short)getpid()))) {
        return -1;
    }
    sleep(1);

    printf("%d bytes from %s: icmp_seq=%u ttl=%d time=%d ms\n", icmp_len, inet_ntoa(from_addr->sin_addr), ntohs(p_icmp->icmp_seq), p_ip->ip_ttl, time_sub(&recv_time, &send_time));
    return 1;
}

/* recv message */
void recv_icmp(const int fd) {
    int ret = 0;
    int recv_len = 0;
    unsigned int interval = 0;
    socklen_t from_len = sizeof(struct sockaddr_in);
    struct sockaddr_in from_addr = {0};

    memset(recv_buff, 0, LEN_1024 * LEN_1024);
    recv_len = recvfrom(fd, (void *)recv_buff, sizeof(recv_buff), 0, (struct sockaddr *)&from_addr, &from_len);
    gettimeofday(&recv_time, NULL);

    if (recv_len < 0) {
        if (errno == EAGAIN) {
            printf("Request time out.\n");
            icmp_stat.max_time = ~0;
        } else {
            printf("icmp receive error!!!\n");
        }
        return;
    }

    icmp_stat.recv_num++;
    interval = time_sub(&recv_time, &send_time);
    icmp_stat.avg_time = (icmp_stat.avg_time * (icmp_stat.send_num - 1) + interval) / icmp_stat.send_num;

    if (icmp_stat.min_time > interval) {
        icmp_stat.min_time = interval;
    }

    if (icmp_stat.max_time < interval) {
        icmp_stat.max_time = interval;
    }

    /* parse receive message */
    ret = parse_icmp(&from_addr, (char *)recv_buff, recv_len);
    if (ret < 0) {
        return;
    }
}

/* send icmp message */
void send_icmp(const int fd, const struct sockaddr_in *dest_addr) {
    unsigned int echo_num = 0;
    int send_len = 0;
    int ret = 0;

    while (echo_num < ICMP_ECHO_MAX) {
        /* fill in message */
        send_len = new_icmp(echo_num, ICMP_DATA_LEN);

        icmp_stat.send_num++;
        gettimeofday(&send_time, NULL);

        /* send icmp message */
        ret = sendto(fd, send_buff, send_len, 0, (struct sockaddr *)dest_addr, sizeof(struct sockaddr_in));
        if (ret < 0) {
            printf("send icmp error!!!\n");
            continue;
        }

        /* wait recv message */
        recv_icmp(fd);

        echo_num++;
    }
}

/* echo stat report */
void show_stat(const icmp_static *stat) {
    float send_num, recv_num;
    send_num = stat->send_num;
    recv_num = stat->recv_num;
    printf("\n--- ping statistics ---\n");
    printf("%u packets transmitted, %u received, %0.0f%% packet loss\n", (int)send_num, (int)recv_num, (send_num - recv_num) / send_num * 100);
    printf("Time: max_time = %.1fms, min_time = %.1fms, avg_time = %.2fms\n", stat->max_time, stat->min_time, stat->avg_time);
}

void chk_ping(char *domain) {
    int fd;
    int rcv_buff = LEN_1024 * LEN_1024;

    struct protoent *icmp_pro_type = NULL;
    struct timeval time_out = {0};
    struct sockaddr_in dest_addr;
    struct hostent *host_name = NULL;

    icmp_stat.send_num = 0;
    icmp_stat.recv_num = 0;
    icmp_stat.max_time = -1.0;
    icmp_stat.min_time = 1000000.0;
    icmp_stat.avg_time = 0.0;

    /* get ICMP protocol type */
    icmp_pro_type = getprotobyname("icmp");

    /* create ICMP socket */
    fd = socket(PF_INET, SOCK_RAW, icmp_pro_type->p_proto);
    if (fd < 0) {
        out_error("create icmp socket failed!!!\n");
    }

    /* set socket options */
    time_out.tv_sec = ICMP_REQUEST_TIMEOUT;
    setsockopt(fd, SOL_SOCKET, SO_RCVBUF, &rcv_buff, sizeof(rcv_buff));
    setsockopt(fd, SOL_SOCKET, SO_RCVTIMEO, &time_out, sizeof(struct timeval));

    /* set socket destination address */
    if ((host_name = gethostbyname(domain)) == NULL) {
        out_error("ping gethostbyname error!!!\n");
    }
    memcpy((char *)&dest_addr.sin_addr, (char *)(host_name->h_addr), host_name->h_length);
    dest_addr.sin_family = host_name->h_addrtype;

    printf("\nPING %s(%s): %d bytes in ICMP packets\n", domain, inet_ntoa(dest_addr.sin_addr), ICMP_DATA_LEN);

    /* send icmp message */
    send_icmp(fd, &dest_addr);

    /* echo stat report */
    show_stat(&icmp_stat);

    close(fd);
}

void check_ping(char *domain, int res_flag) {
    /* res domain ping */
    if (res_flag) {
        chk_ping(CDN);
    }
    /* s domain ping */
    chk_ping(domain);
}
