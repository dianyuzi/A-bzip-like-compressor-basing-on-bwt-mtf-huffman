#include "bwt.h"
int main(int argc,char *argv[])
{
    printf("INSTRUCTION\n");
    printf("PLEASE INPUT FOLLOWING BELOW:\n");
    printf("ENCODE: %s -e test.txt output.txt\n",argv[0]);
    printf("DECOED: %s -d test.txt output.txt\n",argv[0]);
    if(argc!=4)
    {
        printf("AN INPUT ERROR.PLEASE CHEACK THE NUMBER OF INPUT\n");
        exit(EXIT_FAILURE);
    }
    const char *mode=argv[1];
    if(strcmp(mode,"-e") && strcmp(mode,"-d"))
    {
        printf("AN UNKNOWN MODE.PLEASE PAST A CORRECT MODE\n");
        exit(EXIT_FAILURE);
    }
    uint8_t *block=(uint8_t *)malloc((BLOCK_SIZE+1)*sizeof(uint8_t));
    CHECK_ALLOC(block);
    char *in_path=argv[2];
    char *out_path=argv[3];
    FILE *in=fopen(in_path,"rb");
    FILE *out=fopen(out_path,"wb");
    uint32_t primary_index=0;
    uint32_t block_count=0;
    uint32_t n;
    if(!strcmp(argv[1],"-e"))
    {
        while((n=fread(block,sizeof(uint8_t),BLOCK_SIZE,in))>0)
        {
            block_count++;
            uint8_t *output=bwt_encode(block,n,&primary_index);
            n++;
            fwrite(&primary_index,sizeof(uint32_t),1,out);
            fwrite(&n,sizeof(uint32_t),1,out);
            fwrite(output,sizeof(uint8_t),n+1,out);
            free(output);
        }
    }
    else
    {
        while((n=fread(&primary_index,sizeof(uint32_t),1,in)>0))
        {
            block_count++;
            fread(&n,sizeof(uint32_t),1,in);
            fread(block,sizeof(uint8_t),n,in);
            uint8_t *output=bwt_decode(block,n,primary_index);
            fwrite(output,sizeof(uint8_t),n-1,out);
            free(output);
        }
    }
    free(block);
    fclose(in);
    fclose(out);
    printf("YES,WE HAVA PROSESSEd %d BLOCKS\n",block_count);
    return EXIT_SUCCESS;
}