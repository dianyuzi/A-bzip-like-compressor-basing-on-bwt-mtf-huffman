#include"mtf.h"
static void init_list(uint8_t *list)
{
    for(int i=0;i<ALPHABET_SIZE;i++)
    {
        list[i]=(uint8_t)i;
    }
}
static void move_first(uint8_t *list,int rank)
{
    uint8_t symbol = list[rank];
    memmove(&list[1],&list[0],rank);
    list[0] = symbol;                                                                                                  
}
uint8_t *mtf_encode(const uint8_t *input,uint32_t len)
{
    uint8_t list[ALPHABET_SIZE];
    uint8_t *output=(uint8_t *)malloc(len);
    CHECK_ALLOC(output);
    init_list(list);
    for(uint32_t i =0;i<len;i++)
    {
        uint8_t c =input[i];
        int rank = 0;
        while(list[rank]!=c)
        {
            rank++;
        }
        output[i]=(uint8_t)rank;
        move_first(list,rank);
    }
    return output;
}
uint8_t *mtf_decode(const uint8_t *input,uint32_t len)
{
    uint8_t list[ALPHABET_SIZE];
    uint8_t *output=malloc(len);
    CHECK_ALLOC(output);
    init_list(list);
    for(uint32_t i=0;i<len;i++)
    {
        uint8_t rank =input[i];
        output[i]=list[rank];
        move_first(list,rank);
    }
    return output;
}
