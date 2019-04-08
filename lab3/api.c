#include <stdio.h>
#include <string.h>

#include "api.h"
#include "http.h"

#define API_URI "http://www.iu3.bmstu.ru:8090/WebApi/time"

int callback(int status, char *resp_body);

int print_time(enum Format format, enum Type type, bool is_post) {
    char type_string[6] = "";
    switch (type) {
    case LOCAL:
        strcpy(type_string, "local");
        break;
    case UTC:
        strcpy(type_string, "utc");
        break;
    default:
        break;
    }

    char format_string[9] = "";
    switch (format) {
    case INTERNET:
        strcpy(format_string, "internet");
        break;
    case UNIX:
        strcpy(format_string, "unix");
        break;
    default:
        break;
    }

    if (is_post) {
        char post_body[1024] = "";
        snprintf(post_body, sizeof(post_body), "type=%s&format=%s\n",
            type_string, format_string);
        post(API_URI, post_body, callback);
    } else {
        char uri_with_query[128] = "";
        snprintf(uri_with_query, sizeof(uri_with_query),
            "%s?type=%s&format=%s",
            API_URI, type_string, format_string);
        get(uri_with_query, callback);
    }

    return 0;
}

int callback(int status, char *resp_body) {
    char time_string[64] = "";
    sscanf(resp_body, "{\"time\":\"%64[^\"]\"}", time_string);
    printf("%s\n", time_string);
    return 0;
}
