#include "huffman.h"
int main(int argc,char *argv[])
{
    printf("INSTRUCTION\n");
    printf("PLEASE INPUT %s -c test.txt test.homo FOR A COMPRESSION.\n",argv[0]);
    printf("PLEASE INPUT %s -d test.homo test.txt FOR A DECOMPRESSION.\n",argv[0]);
    const uint32_t number=argc;
    if(argc!=4)
    {
        printf("A ERROE IN INPUT NUMBER\n");
        exit(EXIT_FAILURE);
    }
    const char *mode=argv[1];
    if(strcmp(mode,"-c") && strcmp(mode,"-d"))
    {
        printf("A ERROR IN MODE.A UNKNOWN MODE.\n");
        exit(EXIT_FAILURE);
    }
    const char *in_path =argv[2];
    const char *out_path =argv[3];
    uint8_t *block=(uint8_t *)malloc(BLOCK_SIZE*sizeof(uint8_t));
    CHECK_ALLOC(block);
    FILE *in=fopen(in_path,"rb");
    FILE *out=fopen(out_path,"wb");
    if(in==NULL||out==NULL)
    {
        printf("A ERROR IN FILE OPEN\n");
        exit(EXIT_FAILURE);
    }
    uint32_t block_count = 0;
    uint32_t n,len;
    if(strcmp(mode,"-c")==0)
    {
        while((n = (uint32_t)fread(block,sizeof(uint8_t),BLOCK_SIZE,in))>0)
        {
            block_count++;
            huffman_encode(block,n,out);
        }
    }
    else
    {
        while((len=huffman_decode(in,block))>0)
        {
            block_count++;
            fwrite(block,sizeof(uint8_t),len,out);
        }
    }
    free(block);
    fclose(in);
    fclose(out);
    printf("YES,WE PROCESS THE FILE SUCESSFULLY %d BlOCKS HAVE BEEN PROCESSED IN TOTAL.\n",block_count);
    return EXIT_SUCCESS;
}