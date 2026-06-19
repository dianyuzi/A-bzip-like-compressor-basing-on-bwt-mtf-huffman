#include "mtf.h"
int main(int argc,char *argv[])
{
    printf("INSTRUCTION.\n");
    printf("PLEASE INPUT FOLLOWING BELOW\n");
    printf("ENCODE: %s -e input.txt output.txt\n",argv[0]);
    printf("DECODE: %s -d input.txt output.txt\n",argv[0]);
    if(argc!=4)
    {
        printf("AN ERROR IN INPUT.PLEASE CHECK THE NUMBER INPUTING\n");
        exit(EXIT_FAILURE);
    }
    char *mode=argv[1];
    char *in_path=argv[2];
    char *out_path=argv[3];
    if(strcmp(mode,"-e")&&strcmp(mode,"-d"))
    {
        printf("AN ERROR IN MODE.AN UNKNOWN MODE\n");
        exit(EXIT_FAILURE);
    }
    uint8_t *block=malloc(BLOCK_SIZE*sizeof(uint8_t));
    CHECK_ALLOC(block);
    FILE *in=fopen(in_path,"rb");
    FILE *out=fopen(out_path,"wb");
    if(in==NULL||out==NULL)
    {
        printf("AN ERROR IN FILE OPENING OR CREATING.PLEASE CHECK FILE PATH!\n");
        exit(EXIT_FAILURE);
    }
    uint32_t block_count=0;
    if(strcmp(mode,"-e")==0)
    {
        uint32_t n;
        while((n =fread(block,sizeof(uint8_t),BLOCK_SIZE,in))>0)
        {
            block_count++;
            uint8_t *out_code =mtf_encode(block,n);
            fwrite(out_code,sizeof(uint8_t),n,out);
        }
    }
    else
    {
        uint32_t n;
        while((n =fread(block,sizeof(uint8_t),BLOCK_SIZE,in))>0)
        {
            uint8_t *out_data =mtf_decode(block,n);
            fwrite(out_data,sizeof(uint8_t),n,out);
        }
    }
    printf("YES,WE HAVE PROCESSED %d BLOCKS!",block_count);
    free(block);
    fclose(in);
    fclose(out);
    return EXIT_SUCCESS;
}