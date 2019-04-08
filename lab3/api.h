#include <stdbool.h>

enum Format {
    INTERNET,   // RFC3339
    UNIX        // unix timestamp
};

enum Type {
    UTC,    // utc time
    LOCAL   // local time
};

int print_time(enum Format format, enum Type type, bool is_post);
