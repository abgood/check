#ifndef CHECK_RESOLVE_h
#define CHECK_RESOLVE_h

#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "info.h"

/* 检查域名解析 */
int check_resolve(site_info);

/* 通过域名得到ip */
int get_ip(char *, site_info);

#endif
