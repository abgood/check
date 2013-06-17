#ifndef CHECK_H
#define CHECK_H

/* set the platform
 * set is win
 * no set is linux
 */
// #define WINDOWS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <iconv.h>
#include <stdarg.h>

#ifndef WINDOWS
#include <netdb.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip_icmp.h>
#include <arpa/inet.h>
#else
#include <winsock2.h>
#include <ws2tcpip.h>
#define ICMP_ECHO 8
#define ICMP_ECHOREPLY 0
#endif

#include <curl/curl.h>
#include <curl/types.h>
#include <curl/easy.h>

#include "define.h"
#include "curldata.h"
#include "conver.h"
#include "quiry.h"
#include "handle.h"
#include "resolve.h"
#include "ping.h"

#endif
