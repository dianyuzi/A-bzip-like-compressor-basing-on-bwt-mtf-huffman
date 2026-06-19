#ifndef BWT_H
#define BWT_H
#include "common.h"
uint8_t *bwt_encode(const uint8_t *input,uint32_t len,uint32_t *primary_index);
uint8_t *bwt_decode(const uint8_t *L,uint32_t len,uint32_t primary_index);
#endif