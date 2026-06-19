#include "bitio.h"
int main(int argc,char *argv[])
{
    int num=argc;
    printf("INSTRUCTION.\n");
    printf("%s file.txt output.txt",argv[0]);
    char *input_path=argv[1];
    char *output_path=argv[2];
    if(argc!=3)
    {
        printf("INPUT ERROR!CHECK INPUT NUMBER PLEASE.\n");
    }
    FILE *input=fopen(input_path,"rb");
    FILE *output=fopen(output_path,"wb");
    BitReader *reader=bitreader_create(input);
    BitWriter *writer=bitwriter_create(output);
    int i;
    while((i=bit_read(reader))>=0)
    {
        bit_write(writer,i);
    }
    return 0;
}