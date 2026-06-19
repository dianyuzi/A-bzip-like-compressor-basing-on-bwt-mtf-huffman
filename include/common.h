#ifndef COMMON_H
#define COMMON_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
typedef unsigned char uint8_t;
typedef unsigned int uint32_t;
typedef unsigned short uint16_t;
typedef unsigned long uint64_t;
#define ALPHABET_SIZE 256
#define BLOCK_SIZE (1<<16)
#define MAGIC 0x114514
#define CHECK_ALLOC(ptr) \
    do{ \
        if((ptr)==NULL) \
        { \
            fprintf(stderr,"THERE IS A ERROR IN MEMORY ALLOCATION IN %s:%d\n", __FILE__, __LINE__); \
            exit(EXIT_FAILURE); \
        } \
      }while(0) 
#endif