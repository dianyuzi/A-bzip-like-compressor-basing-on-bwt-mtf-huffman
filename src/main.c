#include "common.h"
#include "huffman.h"
#include "mtf.h"
#include "bwt.h"
#include "multiple_processing.h"
static void print_usage(const char *program)
{
    fprintf(stderr,"-INSTRUCTION-\n");
    fprintf(stderr,"[COMPRESS: %s -c <ORIGINAL> <COMPRESSION>]\n",program);
    fprintf(stderr,"[DECOMPRESS: %s -d <COMPRESSION> PATH]\n",program);
}
int main(int argc,char *argv[])
{
    if(argc<3)
    {
        printf("AN ERROR IN INPUT NUMBER.\n");
        print_usage(argv[0]);
        return EXIT_FAILURE;
    }
    const uint8_t *mode=(uint8_t *)argv[1];
    const uint32_t num=(uint32_t)argc;
    if(strcmp(mode,"-c")&&strcmp(mode,"-d"))
    {
        printf("AN ERROR IN MODE.PLEASE CHECK THE MODE\n");
        print_usage(argv[0]);
    }
    if(!strcmp(mode,"-c"))
    {
        char full_name[128];
        snprintf(full_name,sizeof(full_name),"%s.homo",argv[num-1]);
        FILE *output=(FILE *)fopen(full_name,"wb");
        if(!output)
        {
            printf("WE CAN'T OPEN FILE %s",full_name);
            return EXIT_FAILURE;
        }
        mult_compress((uint8_t **)(argv+2),num-3,output,full_name);
        printf("Yes,we have finished the task!\n");
        fclose(output);
        return EXIT_SUCCESS;
    }
    else
    {
        FILE *input=(FILE *)fopen(argv[2],"rb");
        if(!input)
        {
            printf("WE CAN'T OPEN FILE %s",argv[2]);
            return EXIT_FAILURE;
        }
        const char *path=argv[3];
        mult_decompress(input,path);
        fclose(input);
        printf("Yes,we have finished the task!\n");
        return EXIT_SUCCESS;
    }
}