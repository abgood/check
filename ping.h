#ifndef CHECK_PING_H
#define CHECK_PING_H

#include "define.h"

#ifdef WINDOWS

#define ICMP_ECHO 8
#define ICMP_ECHOREPLY 0

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
#endif

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
