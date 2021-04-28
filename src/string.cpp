#include "sds/string.h"

int sds::byte_size(char const* s) noexcept
{
    if (!s) { return 0; }

    char const* p = s;
    int len = 1; // count null-terminator
    while (*p != '\0') {
        ++p;
        ++len;
    }
    return len;
}

bool sds::ascii_cmp(char const* s, char const* ascii, s32 len) noexcept
{
    SDS_ASSERT(s);
    SDS_ASSERT(ascii);
    SDS_ASSERT(len > 0);

    char const* p1 = s;
    char const* p2 = ascii;
    s32 n = 0;
    while (n < len) {
        SDS_ASSERT(is_ascii(*p2));
        if (*p1 == '\0' || *p2 == '\0' || *p1 != *p2) { return false; }
        ++n;
        ++p1;
        ++p2;
    }

    return true;
}

bool sds::ascii_cmp(char const* s, char const* ascii) noexcept
{
    return ascii_cmp(s, ascii, sds::str_size(ascii));
}
