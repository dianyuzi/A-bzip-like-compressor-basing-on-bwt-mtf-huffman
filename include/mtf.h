#ifndef MTF_H
#define MTF_H
#include "common.h"
uint8_t *mtf_encode(const uint8_t *input,uint32_t len);
uint8_t *mtf_decode(const uint8_t *input,uint32_t len);
#endif