#include "huffman.h"
typedef struct{
    huffman_node **data;
    int size;
    int cap;
}MinHeap;
static MinHeap *heap_create(int cap)
{
    MinHeap *h = (MinHeap *)malloc(sizeof(MinHeap));
    CHECK_ALLOC(h);
    h->data=(huffman_node **)malloc(cap * sizeof(huffman_node *));
    CHECK_ALLOC(h->data);
    h->size=0;
    h->cap =cap;
    return h;
}
static void heap_up(MinHeap *h,int i)
{
    while(i>0)
    {
        int parent =(i-1)/2;
        if(h->data[parent]->freq<=h->data[i]->freq)
        break;
        huffman_node *temp =h->data[parent];
        h->data[parent]=h->data[i];
        h->data[i]=temp;
        i=parent;
    }
}
static void heap_down(MinHeap *h,int i)
{
    while(1)
    {
        int lchild = 2*i+1;
        int rchild = 2*i+2;
        int min=i;
        if(lchild<h->size&&h->data[lchild]->freq<h->data[min]->freq)
        {
            min=lchild;
        }
        if(rchild<h->size&&h->data[rchild]->freq<h->data[min]->freq)
        {
            min=rchild;
        }
        if(i==min) break;
        huffman_node *temp=h->data[i];
        h->data[i]=h->data[min];
        h->data[min]=temp;
        i=min;
    }
}
static void heap_push(MinHeap *h,huffman_node *n)
{
    h->data[h->size++]=n;
    heap_up(h,h->size-1);
}
static huffman_node *heap_pop(MinHeap *h)
{
    huffman_node *temp=h->data[0];
    h->data[0]=h->data[--h->size];
    if(h->size>0)
    heap_down(h,0);
    return temp;
} 
static void heap_free(MinHeap *h)
{
    free(h->data);
    free(h);
}
static huffman_node *node_create(int byte,uint32_t freq)
{
    huffman_node *n=(huffman_node *)malloc(sizeof(huffman_node));
    CHECK_ALLOC(n);
    n->byte=byte;
    n->freq=freq;
    n->lchild=NULL;
    n->rchild=NULL;
    return n;
}
static huffman_node *huffman_build(const uint8_t *input,uint32_t len)
{
    uint32_t freq[ALPHABET_SIZE]={0};
    for(uint32_t i=0;i<len;i++)
    {
        freq[input[i]]++;
    }
    MinHeap *heap = heap_create(ALPHABET_SIZE);
    for(uint32_t i=0;i<ALPHABET_SIZE;i++)
    {
        if(freq[i])
        {
            heap_push(heap,node_create((uint8_t)i,freq[i]));
        }
    }
    if(heap->size==1)
    {
        huffman_node *one=heap_pop(heap);
        huffman_node *root =node_create(-1,one->freq);
        root->lchild=one;
        heap_free(heap);
        return root;
    }
    while(heap->size>1)
    {
        huffman_node *n1=heap_pop(heap);
        huffman_node *n2=heap_pop(heap);
        huffman_node *n3=node_create(-1,n1->freq+n2->freq);
        n3->lchild=n1;
        n3->rchild=n2;
        heap_push(heap,n3);
    }
    huffman_node *root=heap_pop(heap);
    heap_free(heap);
    return root;
}
static void generate_code(huffman_node *n,huffman_code *codetable,uint32_t current_code,int current_bit)
{
    if(n==NULL) return;
    if(n->byte!=-1)
    {
        codetable[n->byte].code=current_code;
        codetable[n->byte].bit=current_bit;
        return;
    }
    generate_code(n->lchild,codetable,(current_code<<1)|0,current_bit+1);
    generate_code(n->rchild,codetable,(current_code<<1)|1,current_bit+1);
}
void huffman_node_free(huffman_node *n)
{
    if(n==NULL) return;
    huffman_node_free(n->lchild);
    huffman_node_free(n->rchild);
    free(n);
}
void huffman_encode(const uint8_t *input,uint32_t len,FILE *out)
{
    huffman_node *root=huffman_build(input,len);
    huffman_code codetable[ALPHABET_SIZE]={{0,0}};
    generate_code(root,codetable,0,0);
    huffman_node_free(root);
    uint32_t total_bit =0;
    uint32_t freq[ALPHABET_SIZE] ={0};
    for(uint32_t i=0;i<len;i++)
    {
        freq[input[i]]++;
    }
    for(uint32_t i=0;i<ALPHABET_SIZE;i++)
    {
        total_bit +=freq[i]*(uint32_t)codetable[i].bit;
    }
    fwrite(&len,sizeof(uint32_t),1,out);
    fwrite(&total_bit,sizeof(uint32_t),1,out);
    uint16_t symbol_count =0;
    for(uint32_t i=0;i<ALPHABET_SIZE;i++)
    {
        if(codetable[i].bit>0)
        symbol_count++;
    }
    fwrite(&symbol_count,sizeof(uint16_t),1,out);
    for(int i=0;i<ALPHABET_SIZE;i++)
    {
        if(codetable[i].bit>0)
        {
            uint8_t byte =(uint8_t)i;
            uint8_t bit=(uint8_t)codetable[i].bit;
            uint32_t code = codetable[i].code;
            fwrite(&byte,1,1,out);
            fwrite(&bit,1,1,out);
            fwrite(&code,sizeof(uint32_t),1,out);
        }
    }
    BitWriter *w=bitwriter_create(out);
    for(uint32_t i=0;i<len;i++)
    {
        uint8_t byte=input[i];
        uint32_t code=codetable[byte].code;
        int bit =codetable[byte].bit;
        for(int b=bit-1;b>=0;b--)
        {
            bit_write(w,(code>>b)&1);
        }
    }
    bit_append(w);
    bitwriter_free(w);
}
uint32_t huffman_decode(FILE *in,uint8_t *output)
{
    uint32_t len,total_bit;
    fread(&len,sizeof(uint32_t),1,in);
    fread(&total_bit,sizeof(uint32_t),1,in);
    uint16_t byte_count;
    fread(&byte_count,sizeof(uint16_t),1,in);
    huffman_node *n=node_create(-1,0);
    for(int i=0;i<byte_count;i++)
    {
        uint8_t byte,bit;
        uint32_t code;
        if(fread(&byte,sizeof(uint8_t),1,in)==0)
        return 0;
        fread(&bit,sizeof(uint8_t),1,in);
        fread(&code,sizeof(uint32_t),1,in);
        huffman_node *root=n;
        for(int b=bit-1;b>=0;b--)
        {
            int t=code>>b&1;
            if(!t)
            {
                if(root->lchild==NULL)
                root->lchild=node_create(-1,0);
                root=root->lchild;   
            }
            else
            {
                if(root->rchild==NULL)
                root->rchild=node_create(-1,0);
                root=root->rchild;   
            }
            
        }
        root->byte=byte;
    }
    BitReader *r=bitreader_create(in);
    huffman_node *root=n;
    uint32_t out_index=0;
    for(uint32_t i=0;i<total_bit;i++)
    {
        int bit=bit_read(r);
        if(bit<0)
        break;
        root=(bit==0)?root->lchild:root->rchild;
        if(root->byte!=-1)
        {
            output[out_index++]=(uint8_t)root->byte;
            root=n;
        }
    }
    bitreader_free(r);
    huffman_node_free(n);
    return out_index;
}