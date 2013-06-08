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

#ifndef WINDOWS
#include <netdb.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#else
#include <winsock2.h>
#endif

#include <curl/curl.h>
#include <curl/types.h>
#include <curl/easy.h>

#include "define.h"
#include "curldata.h"
#include "conver.h"

#endif
