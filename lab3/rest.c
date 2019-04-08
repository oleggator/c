#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <string.h>

#include "http.h"
#include "api.h"

int main(int argc, char *argv[]) {
    enum Format format;
    enum Type type;
    bool is_post = false;

    char opt_string[] = "pf:t:";
    int opt = getopt(argc, argv, opt_string);
    while(opt != -1) {
        switch(opt) {
        case 'f':
            if (!strcmp(optarg, "unix")) {
                format = UNIX;
            } else if (!strcmp(optarg, "internet")) {
                format = INTERNET;
            } else {
                printf("wrong -f: %s\n", optarg);
                return EXIT_FAILURE;
            }
            break;
        case 't':
            if (!strcmp(optarg, "local")) {
                type = LOCAL;
            } else if (!strcmp(optarg, "utc")) {
                type = UTC;
            } else {
                printf("wrong -t: %s\n", optarg);
                return EXIT_FAILURE;
            }
            break;
        case 'p':
            is_post = true;
            break;
        default:
            return EXIT_FAILURE;
        }
         
        opt = getopt(argc, argv, opt_string);
    }

    print_time(format, type, is_post);

    return EXIT_SUCCESS;
}
