#ifndef BITIO_H
#define BITIO_H
#include<common.h>
typedef struct{
    FILE *file;
    uint8_t buf;
    int bit_count;
}BitWriter;
typedef struct{
    FILE *file;
    uint8_t buf;
    int bit_count;
}BitReader;
BitWriter *bitwriter_create(FILE *file);
void bit_write(BitWriter *w,int bit);
void bit_append(BitWriter *w);
void bitwriter_free(BitWriter *w);
BitReader *bitreader_create(FILE *file);
int bit_read(BitReader *r);
void bitreader_free(BitReader *r);
#endif
