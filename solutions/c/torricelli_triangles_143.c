/* Project Euler 143: https://projecteuler.net/problem=143
 * Compatible Fermat distances p,q satisfy p^2+pq+q^2=square.  Primitive
 * Eisenstein triples generate the graph; Torricelli triples are its 3-cycles. */
#include "euler_extra.h"

enum { LIMIT143 = 120000 };
struct neighbors { unsigned *items, count, capacity; };
static struct neighbors graph143[LIMIT143 + 1];

static void add_neighbor(unsigned from,unsigned to){
    struct neighbors *list=&graph143[from];
    if(list->count==list->capacity){
        unsigned capacity=list->capacity?2*list->capacity:4;
        unsigned *items=realloc(list->items,capacity*sizeof(*items));
        if(!items){fputs("allocation failure\n",stderr);exit(EXIT_FAILURE);}
        list->items=items;list->capacity=capacity;
    }
    list->items[list->count++]=to;
}
static int cmp143(const void *l,const void *r){unsigned a=*(const unsigned*)l,b=*(const unsigned*)r;return a<b?-1:a!=b;}
static bool adjacent(unsigned a,unsigned b){
    struct neighbors *list=&graph143[a];
    return bsearch(&b,list->items,list->count,sizeof(*list->items),cmp143)!=NULL;
}
int main(void){
    for(unsigned m=2;(uint64_t)m*m+2*m<LIMIT143;++m) for(unsigned n=1;n<m;++n){
        if(euler_gcd(m,n)!=1)continue;
        unsigned p=m*m-n*n,q=2*m*n+n*n;
        if((m-n)%3==0){p/=3;q/=3;}
        if(!p||!q)continue;
        for(unsigned k=1;(uint64_t)k*(p+q)<=LIMIT143;++k){
            unsigned a=k*p,b=k*q;
            add_neighbor(a,b);add_neighbor(b,a);
        }
    }
    for(unsigned p=1;p<=LIMIT143;++p){
        struct neighbors *list=&graph143[p];
        qsort(list->items,list->count,sizeof(*list->items),cmp143);
        unsigned unique=0;
        for(unsigned i=0;i<list->count;++i)if(!i||list->items[i]!=list->items[i-1])list->items[unique++]=list->items[i];
        list->count=unique;
    }
    bool *sums=calloc(LIMIT143+1,1);if(!sums)return EXIT_FAILURE;
    for(unsigned p=1;p<=LIMIT143;++p){
        struct neighbors *list=&graph143[p];
        for(unsigned i=0;i<list->count;++i){unsigned q=list->items[i];if(q<=p)continue;
            for(unsigned j=i+1;j<list->count;++j){unsigned r=list->items[j];
                if(r<=q)continue;
                unsigned sum=p+q+r;
                if(sum>LIMIT143)break;
                if(adjacent(q,r))sums[sum]=true;
            }
        }
    }
    uint64_t answer=0;for(unsigned n=1;n<=LIMIT143;++n)if(sums[n])answer+=n;
    printf("%" PRIu64 "\n",answer);
    free(sums);for(unsigned n=1;n<=LIMIT143;++n)free(graph143[n].items);
    return 0;
}
