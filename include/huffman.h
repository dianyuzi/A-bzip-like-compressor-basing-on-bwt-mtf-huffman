#ifndef HUFFMAN_H
#define HUFFMAN_H
#include "bitio.h"
typedef struct huffman_node{
    struct huffman_node *lchild;
    struct huffman_node *rchild;
    struct huffman_node *parent;
    int byte;
    int freq;
}huffman_node,*huffman_tree;
typedef struct{
    uint32_t code;
    int bit;
}huffman_code;
void huffman_encode(const uint8_t *input,uint32_t len,FILE *out);
uint32_t huffman_decode(FILE *in,uint8_t *output);
void huffman_node_free(huffman_node *node);
#endif