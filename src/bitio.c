#include"bitio.h"
BitWriter *bitwriter_create(FILE *file)
{
    BitWriter *w =(BitWriter *)malloc(sizeof(BitWriter));
    CHECK_ALLOC(w);
    w->file=file;
    w->buf=0;
    w->bit_count=0;
    return w;
}
void bit_write(BitWriter *w,int bit)
{
    if(bit)
    {
        w->buf|=(1<<(7-w->bit_count));
    } 
    w->bit_count++;
    if(w->bit_count==8)
    {
        fwrite(&w->buf,1,1,w->file);
        w->buf =0;
        w->bit_count =0;
    }
}
void bit_append(BitWriter *w)
{
    if(w->bit_count>0)
    {
        fwrite(&w->buf,1,1,w->file);
        w->buf=0;
        w->bit_count=0;
    }
}
void bitwriter_free(BitWriter *w)
{
    free(w);
}
BitReader *bitreader_create(FILE *file)
{
    BitReader *r=(BitReader *)malloc(sizeof(BitReader));
    CHECK_ALLOC(r);
    r->file=file;
    r->buf=0;
    r->bit_count=0;
    return r;
}
int bit_read(BitReader *r)
{
    if(r->bit_count==0)
    {
        if(fread(&r->buf,1,1,r->file)!=1)
        {
            return -1;
        }
        r->bit_count =8;
    }
    int bit=(r->buf&0x80)?1:0;
    r->buf<<=1;
    r->bit_count--;
    return bit;
}
void bitreader_free(BitReader *r)
{
    free(r);
}