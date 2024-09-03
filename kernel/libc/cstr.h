#ifndef _CSTR_H
#define _CSTR_H 1

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

const char* uint64_to_string(uint64_t value);
const char* int64_to_string(int64_t value);
const char* uint64_to_hstring(uint64_t value);
const char* uint32_to_hstring(uint32_t value);
const char* uint16_to_hstring(uint16_t value);
const char* uint8_to_hstring(uint8_t value);
const char* double_to_string_rounded(double value, uint8_t decimalPlaces);
const char* double_to_string(double value);

#ifdef __cplusplus
}
#endif

#ifdef __cplusplus
const char* to_string(uint64_t value)
{
    return uint64_to_string(value);
}

const char* to_string(int64_t value)
{
    return int64_to_string(value);
}

const char* to_hstring(uint64_t value)
{
    return uint64_to_hstring(value);
}

const char* to_hstring(uint32_t value)
{
    return uint32_to_hstring(value);
}

const char* to_hstring(uint16_t value)
{
    return uint16_to_hstring(value);
}

const char* to_hstring(uint8_t value)
{
    return uint8_to_hstring(value);
}

const char* to_string(double value, uint8_t decimalPlaces)
{
    return double_to_string_rounded(value, decimalPlaces);
}

const char* to_string(double value)
{
    return double_to_string(value);
}
#endif

#endif
