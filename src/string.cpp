#include "sds/string.h"

int sds::byte_size(char const* s) noexcept {
    if (!s) { return 0; }

    char const* p = s;
    int len = 1; // count null-terminator
    while (*p != '\0') {
        ++p;
        ++len;
    }
    return len;
}
