#ifndef MULT_POCE
#define MULT_POCE
#include "common.h"
#include <dirent.h>
#include <sys/stat.h>
#include <direct.h>
void heart_scan(const char *path,uint8_t **file_list,uint32_t *count);
void mkdir_create(const uint8_t *path);
uint64_t get_file_size(const char *path);
void mult_compress(uint8_t **f,uint32_t input_count,FILE *output,char *output_name);
void mult_decompress(FILE *input,const char *target_path);
#endif