#include <stdio.h>
#include <stdlib.h>

typedef struct 
{
    size_t *add;
    size_t *sum;
    int size;
}tree;

tree heapbase[16];


void pushup(tree *t,size_t rt){
    (*t).sum[rt] = (*t).sum[rt<<1]+(*t).sum[rt<<1|1];
}

void pushdown(tree *t,size_t rt,size_t ln,size_t rn){
    if((*t).add[rt]){
        (*t).add[rt<<1] += (*t).add[rt];
        (*t).add[rt<<1|1] += (*t).add[rt];
        (*t).sum[rt<<1] +=  (*t).add[rt]*ln;
        (*t).sum[rt<<1|1] += (*t).add[rt]*rn;
        (*t).add[rt] = 0;
    }
}

void build(tree *t,size_t l,size_t r,size_t rt,size_t *arr){
    if(l == r){
        (*t).sum[rt] = arr[l];
        return;
    }
    size_t m = (l+r) >> 1;
    build(t,l,m,rt<<1,arr);
    build(t,m+1,r,rt<<1|1,arr);
    pushup(t,rt);
}

size_t query(tree *t,size_t L,size_t R,size_t l,size_t r,size_t rt){
    if(L <= l && r <= R){
        return (*t).sum[rt];
    }
    size_t m = (l+r) >> 1;
    pushdown(t,rt,m-l+1,r-m);
    size_t ans = 0;
    if(L <= m) ans += query(t,L,R,l,m,rt<<1);
    if(R > m) ans += query(t,L,R,m+1,r,rt<<1|1);
    return ans; 
}

void update(tree *t,size_t L,size_t R,size_t C,size_t l,size_t r,size_t rt){
    if(L <= l && r <= R){
        (*t).sum[rt] += C*(r-l+1);
        (*t).add[rt] += C;
        return ;
    }
    int m = (l+r) >> 1;
    pushdown(t,rt,l-m+1,r-m);
    if(L <= m) update(t,L,R,C,l,m,rt<<1);
    if(R > m) update(t,L,R,C,m+1,r,rt<<1|1);
    pushup(t,rt);
}


void menu(){
    puts("1.Create");
    puts("2.Add");
    puts("3.Del");
    puts("4.Sum");
    puts("5.Exit");
    puts(">>");
}

size_t getNum(){
    char num[32];
    num[read(0,num,30)]=0;
    return atoll(num);
}


void create(){
    puts("idx?");
    unsigned int idx = getNum();
    if(idx < 16){
        puts("how many?");
        size_t size = getNum();
        if(size < 1){
            puts("error!");
            return;
        }
        void *sum = malloc(size*8*4);
        void *add = malloc(size*8*4);
        heapbase[idx].size = size;
        if(!sum || !add){
            puts("error!");
            return;
        }
        heapbase[idx].sum = sum;    
        heapbase[idx].add = add;
        size_t *arr = malloc(size*8+8);
        for(int i = 1;i <= size;i++){
            puts("num?");
            size_t num = getNum();
            arr[i] = num;
        }
        build(&heapbase[idx],1,size,1,arr);
        
        free(arr);
        return;
    }
    puts("error!");
}

void del(){
    puts("idx?");
    unsigned int idx = getNum();
    if(idx < 16 && heapbase[idx].sum){
        free(heapbase[idx].sum);
        free(heapbase[idx].add);
        heapbase[idx].sum = 0;
        heapbase[idx].size = 0;
        heapbase[idx].add = 0;
        puts("success!");
        return;
    }else
        puts("error!");
}

void add(){
    puts("idx?");
    unsigned int idx = getNum();
    if(idx < 16 && heapbase[idx].sum){
        puts("l:");
        size_t l = getNum();
        puts("r:");
        size_t r = getNum();
        puts("c:");
        size_t c = getNum();
        if(l <= 0 || r > heapbase[idx].size || l > r){
            puts("error");
            return;
        }
        update(&heapbase[idx],l,r,c,1,heapbase[idx].size,1);
    }else
        puts("error!");
}

void show(){
    puts("idx?");
    unsigned int idx = getNum();
    if(idx < 16 && heapbase[idx].sum){
        puts("l:");
        size_t l = getNum();
        puts("r:");
        size_t r = getNum();
        if(l <= 0 || r > heapbase[idx].size || l > r){
            puts("error");
            return;
        }
        printf("%d : %lu\n",idx,query(&heapbase[idx],l,r,1,heapbase[idx].size,1));
        puts("success!");
        return;
    }
}

int main(){
    setbuf(stdout,0);
    setbuf(stdin,0);
    setbuf(stderr,0);
    while (1)
    {
        menu();
        int cmd = getNum();
        switch(cmd){
            case 1:
                create();
                break;
            case 2:
                add();
                break;
            case 3:
                del();
                break;
            case 4:
                show();
                break;
            case 5:
                exit(0);
            default:
                exit(0);
        }
    }
    return 0;
}