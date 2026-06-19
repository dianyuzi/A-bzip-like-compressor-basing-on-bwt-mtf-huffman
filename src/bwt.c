#include "bwt.h"
static const uint8_t *g_data =NULL;
static uint32_t g_len =0;
static int cmp_rotation(const void *a,const void *b)
{
    uint32_t i =*(const uint32_t *)a;
    uint32_t j =*(const uint32_t *)b;
    for(uint32_t k=0;k<g_len;k++)
    {
        uint8_t ci=g_data[(i+k)%g_len];
        uint8_t cj=g_data[(j+k)%g_len];
        if(ci!=cj)
        return (int)ci-(int)cj;
    }
    return 0;
}
uint8_t *bwt_encode(const uint8_t *input,uint32_t len,uint32_t *primary_index)
{
    uint32_t n=len+1;
    uint8_t *data=(uint8_t *)malloc(n);
    CHECK_ALLOC(data);
    memcpy(data,input,len);
    data[len]=0x00;
    uint32_t *sa=(uint32_t *)malloc(n*sizeof(uint32_t));
    CHECK_ALLOC(sa);
    for(uint32_t i=0;i<n;i++)
    sa[i]=i;
    g_data=data;                                                                                                        
    g_len=n;
    qsort(sa,n,sizeof(uint32_t),cmp_rotation);
    uint8_t *l=(uint8_t *)malloc(n);
    CHECK_ALLOC(l);
    *primary_index =0;
    for(uint32_t i=0;i<n;i++)
    {
        l[i]=data[(sa[i]+n-1)%n];
        if(sa[i]==0)
        *primary_index =i;
    }
    free(data);
    free(sa);
    return l;
}
uint8_t *bwt_decode(const uint8_t *L,uint32_t len,uint32_t primary_index)
{
    uint32_t freq[ALPHABET_SIZE]={0};
    for(uint32_t i=0;i<len;i++)
    {
        freq[L[i]]++;
    }
    uint32_t f_start[ALPHABET_SIZE]={0};
    uint32_t cumsum=0;
    for(int c =0;c<ALPHABET_SIZE;c++)
    {
        f_start[c]=cumsum;
        cumsum +=freq[c];
    }
    uint32_t *LF=(uint32_t *)malloc(len*sizeof(uint32_t));
    uint32_t rank_cnt[ALPHABET_SIZE] = {0};
    CHECK_ALLOC(LF);
    for(uint32_t i=0;i<len;i++)
    {
        uint8_t c=L[i];
        LF[i] = f_start[c]+rank_cnt[c];
        rank_cnt[c]++;
    }
    uint32_t *T=(uint32_t *)malloc(len*sizeof(uint32_t));
    CHECK_ALLOC(T);
    for(uint32_t i=0;i<len;i++)
    {
        T[LF[i]]=i;
    }
    uint8_t *output=(uint8_t *)malloc(len);
    CHECK_ALLOC(output);
    uint32_t cur=primary_index;
    uint32_t out_len=0;
    for(uint32_t i=0;i<len;i++)
    {
        cur=T[cur];
        output[out_len++]=L[cur];
    }
    free(LF);
    free(T);
    return output;
}