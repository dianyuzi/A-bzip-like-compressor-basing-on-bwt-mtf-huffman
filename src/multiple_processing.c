#include "multiple_processing.h"
#include "huffman.h"
#include "bwt.h"
#include "mtf.h"
void heart_scan(const char *path,uint8_t **file_list,uint32_t *count)
{
    struct  stat st;
    if(stat(path,&st)!=0)
    {
        printf("AN ERROR IN PATH!CHECK THE PATH PLEASE!\n");
        return;
    }
    DIR *target=opendir(path);
    if(!target)
    {
        file_list[(*count)++]=strdup(path);
        return;
    }
    struct dirent *entry;
    while((entry = readdir(target)))
    {
        if(!(strcmp(entry->d_name,".")&&strcmp(entry->d_name,"..")))
        {
            continue;
        }
        char full_path[1024];
        snprintf(full_path,sizeof(full_path),"%s/%s",path,entry->d_name);
        heart_scan(full_path,file_list,count);
    }
    closedir(target);
}
void mkdir_create(const uint8_t *path)
{
    uint8_t temp[1024];
    strncpy(temp,path,sizeof(temp));
    uint8_t *last_slash=strrchr(temp,'/');
    if(!last_slash)
    return;
    *last_slash='\0';
    for(char *p=temp+1;*p;p++)
    {
        if(*p=='/')
        {
            *p='\0';
            _mkdir(temp);
            *p='/';
        }
    }
    _mkdir(temp);
}
uint64_t get_file_size(const char *path)
{
    struct stat st;
    stat(path,&st);
    return (uint64_t)st.st_size;
}
void mult_compress(uint8_t **f,uint32_t input_count,FILE *output,char *output_name)
{
    uint8_t *file_list[1024];
    uint32_t file_count=0;
    for(int i=0;i<input_count;i++)
    {
        heart_scan(f[i],file_list,&file_count);
    }
    if(file_count==0)
    {
        printf("WE CAN'T FIND THE FILE.\n");
        return;
    }
    uint32_t magic =MAGIC;
    uint64_t total_size=0;
    fwrite(&magic,sizeof(uint32_t),1,output);
    fwrite(&file_count,sizeof(uint32_t),1,output);
    for(int i=0;i<file_count;i++)
    {
        total_size+=get_file_size(file_list[i]);
        uint16_t path_len=(uint16_t)strlen(file_list[i]);
        uint64_t file_size =get_file_size(file_list[i]);
        fwrite(&path_len,sizeof(uint16_t),1,output);
        fwrite(file_list[i],sizeof(uint8_t),path_len,output);
        fwrite(&file_size,sizeof(uint64_t),1,output);
    }
    uint8_t *block=(uint8_t *)malloc(BLOCK_SIZE*sizeof(uint8_t));
    CHECK_ALLOC(block);
    uint32_t block_count=0;
    for(int i=0;i<file_count;i++)
    {
        FILE *target_file=fopen(file_list[i],"rb");
        if(!target_file)
        {
            printf("WE CAN'T OPEN FILE %s",file_list[i]);
            continue;
        }
        uint32_t num;
        while((num=fread(block,sizeof(uint8_t),BLOCK_SIZE,target_file))>0)
        {
            block_count++;
            uint32_t primary_index=0;
            uint8_t *bwt_code=bwt_encode(block,num,&primary_index);
            fwrite(&primary_index,sizeof(uint32_t),1,output);
            uint8_t *mtf_code=mtf_encode(bwt_code,num+1);
            huffman_encode(mtf_code,num+1,output);
            free(bwt_code);
            free(mtf_code);
        }
        fclose(target_file);
    }
    fclose(output);
    printf("%ld bytes.\n",get_file_size(output_name));
    uint64_t total_compressed = get_file_size(output_name);
    free(block);
    printf("Yes,we hava processed %d files,and have compressed %d blcoks.\n",file_count,block_count);
    printf("The original file's size is %ld bytes.\n",total_size);
    printf("The file's size after processing is %ld bytes.\n",total_compressed);
    if(total_compressed<total_size)
    {
        double compress_rate=((double)(total_size-total_compressed))/((double)(total_size));
        printf("The compressed rate is %.2f %%.\n",compress_rate*100);
    }
    else
    {
        printf("It's a disarray file or files set,we can just pack them!\n");
    }   
}
void mult_decompress(FILE *input,const char *target_path)
{
    uint32_t magic;
    DIR *open_path=opendir(target_path);
    if(!open_path)
    {
        printf("A FLASE PATH.PLEASE CHECK THE PATH!\n");
        exit(EXIT_FAILURE);
    }
    fread(&magic,sizeof(uint32_t),1,input);
    if(magic!=MAGIC)
    {
        printf("AN ERROR IN FILE INPUT!INPUT A CORRECT FILE PLEASE!\n");
        return;
    }
    uint32_t file_count;
    fread(&file_count,sizeof(uint32_t),1,input);
    uint64_t size[file_count];
    uint8_t paths[file_count][1024];
    for(int i=0;i<file_count;i++)
    {
        uint16_t path_len;
        fread(&path_len,sizeof(uint16_t),1,input);
        fread(paths[i],sizeof(uint8_t),path_len,input);
        paths[i][path_len]='\0';
        fread(&size[i],sizeof(uint64_t),1,input);
    }
    uint8_t *block=(uint8_t *)malloc((BLOCK_SIZE+1)*sizeof(uint8_t));
    CHECK_ALLOC(block);
    uint32_t block_count=0;
    char full_path[1024];
    uint16_t create_correct=file_count;
    for(int i=0;i<file_count;i++)
    {
        snprintf(full_path,sizeof(full_path),"%s/%s",target_path,paths[i]);
        mkdir_create(full_path);
        uint64_t remaining=size[i];
        FILE *output=fopen(full_path,"wb");
        if(!output)
        {
            printf("A ERROR IN CREATING FILE %s.\n",paths[i]);
            create_correct--;
            continue;
        }
        while(remaining>0)
        {
            block_count++;
            uint32_t primary_index;
            fread(&primary_index,sizeof(uint32_t),1,input);
            uint32_t num = huffman_decode(input,block);
            uint8_t *mtf_code=mtf_decode(block,num);
            uint8_t *bwt_code=bwt_decode(mtf_code,num,primary_index);
            fwrite(bwt_code,sizeof(uint8_t),num-1,output);
            remaining-=(num-1);
            free(mtf_code);
            free(bwt_code);
        }    
        fclose(output);
    }
    free(block);
    printf("Yes,captain!we have processed %d blocks,and created %d files.\n",block_count,create_correct);
}