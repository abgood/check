#include "check.h"

size_t copy_data(void *ptr, size_t size, size_t nmemb, void *stream) {
    size_t num;
    // num = fread(ptr, size, nmemb, stream);
    FILE *fp = fopen("tmp", "w");
    num = fwrite(ptr, size, nmemb, (FILE *)fp);
    fclose(fp);
    return num;
}

void read_last_line(char *last_line) {
    FILE *fp = fopen("tmp", "rb");

    if (fseek(fp, -1, SEEK_END) != 0) {
        fprintf(stderr, "read file error!\n");
    }

    /* find the last but one "\n" */
    while (1) {
        if (fgetc(fp) != '\n') {
            fseek(fp, -2, SEEK_CUR);
        } else {
            break;
        }
    }

    /* read last line */
    if (!fgets(last_line, LEN_1024, fp)) {
        fprintf(stderr, "read last one line error!\n");
        exit(1);
    }

    fclose(fp);
}

void curl_data(loc_info info) {
    CURL *curl;
    char *output;
    char last_line[LEN_1024] = {0};
    char *url = "http://iframe.ip138.com/ic.asp";

    char ip[LEN_32] = {0};
    char address[LEN_512] = {0};
    char agent[LEN_64] = {0};

    info->ip = (char *)malloc(LEN_32);
    info->address = (char *)malloc(LEN_512);
    info->agent = (char *)malloc(LEN_64);

    curl_global_init(CURL_GLOBAL_ALL);
    curl = curl_easy_init();
    curl_easy_setopt(curl, CURLOPT_URL, url);
    // curl_easy_setopt(curl, CURLOPT_READFUNCTION, &copy_data);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &copy_data);
    curl_easy_perform(curl);
    curl_easy_cleanup(curl);

    /* read last one line from tmp file */
    read_last_line(last_line);

    /* code convert */
    output = conver_code(last_line, "gb2312", "utf-8");
    sscanf(output, "%*[^[][%[^]]%*[^：]：%[^ ] %[^<]", ip, address, agent);

    strncpy(info->ip, ip, strlen(ip));
    strncpy(info->address, address, strlen(address));
    strncpy(info->agent, agent, strlen(agent));
}
