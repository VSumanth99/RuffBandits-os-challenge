#ifndef BYTE_UTILS_H
#define BYTE_UTILS_H
#include <stdint.h>
union ArrayInteger8 {
    uint8_t array[8];
    uint64_t number;
} start_arr, end_arr;
typedef union ArrayInteger8 ArrayInteger8;
#endif
