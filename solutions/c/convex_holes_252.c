#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * Project Euler 252: Convex Holes
 *
 * Every empty convex polygon has a canonical lowest/leftmost anchor.  Sorting
 * the other vertices by polar angle around that anchor turns its boundary into
 * an increasing convex chain.  Fan triangulation from the anchor reduces the
 * polygon to empty triangles.
 *
 * For each directed edge (a,b), an eight-word bitset records all 500 points
 * strictly to its left.  A counterclockwise triangle abc is empty exactly when
 * left(a,b)&left(b,c)&left(c,a) is empty.  Boundary points are deliberately
 * excluded from these sets.  dp[j,k] is the greatest doubled area of a valid
 * chain ending in vertices j,k; it extends dp[h,j] only through a left turn.
 * An anchor-to-j segment containing an intermediate collinear point cannot
 * become an internal fan diagonal, so those extensions are rejected.
 * All geometry uses integer cross products and doubled areas.
 */

enum{N=500,WORDS=8};typedef struct{int x,y;}Point;
static Point point[N];static uint64_t *left_bits;static unsigned char *inner_collinear;
static int sort_anchor;
static int64_t cross3(int a,int b,int c){return(int64_t)(point[b].x-point[a].x)*(point[c].y-point[a].y)-(int64_t)(point[b].y-point[a].y)*(point[c].x-point[a].x);}
static int polar_compare(const void *pa,const void *pb){int a=*(const int*)pa,b=*(const int*)pb;int64_t c=cross3(sort_anchor,a,b);if(c)return c>0?-1:1;int64_t da=(int64_t)(point[a].x-point[sort_anchor].x)*(point[a].x-point[sort_anchor].x)+(int64_t)(point[a].y-point[sort_anchor].y)*(point[a].y-point[sort_anchor].y);int64_t db=(int64_t)(point[b].x-point[sort_anchor].x)*(point[b].x-point[sort_anchor].x)+(int64_t)(point[b].y-point[sort_anchor].y)*(point[b].y-point[sort_anchor].y);return da<db?-1:da>db;}
static uint64_t *edge_bits(int a,int b){return left_bits+((size_t)a*N+b)*WORDS;}
static int empty_triangle(int a,int b,int c){uint64_t *ab=edge_bits(a,b),*bc=edge_bits(b,c),*ca=edge_bits(c,a);for(int w=0;w<WORDS;++w)if(ab[w]&bc[w]&ca[w])return 0;return 1;}

int main(void){
    uint64_t state=290797;for(int i=0;i<N;++i){state=state*state%50515093;point[i].x=(int)(state%2000)-1000;state=state*state%50515093;point[i].y=(int)(state%2000)-1000;}
    left_bits=calloc((size_t)N*N*WORDS,sizeof(*left_bits));inner_collinear=calloc((size_t)N*N,1);if(!left_bits||!inner_collinear)return 1;
    for(int a=0;a<N;++a)for(int b=0;b<N;++b)if(a!=b){uint64_t *bits=edge_bits(a,b);for(int p=0;p<N;++p)if(p!=a&&p!=b){int64_t c=cross3(a,b,p);if(c>0)bits[p>>6]|=UINT64_C(1)<<(p&63);else if(c==0){int64_t dot=(int64_t)(point[p].x-point[a].x)*(point[p].x-point[b].x)+(int64_t)(point[p].y-point[a].y)*(point[p].y-point[b].y);if(dot<0)inner_collinear[(size_t)a*N+b]=1;}}}

    int64_t global_best=0;int vertices[N];int64_t *dp=malloc((size_t)N*N*sizeof(*dp));if(!dp)return 1;
    for(int anchor=0;anchor<N;++anchor){int m=0;for(int v=0;v<N;++v)if(point[v].y>point[anchor].y||(point[v].y==point[anchor].y&&point[v].x>point[anchor].x))vertices[m++]=v;if(m<2)continue;sort_anchor=anchor;qsort(vertices,(size_t)m,sizeof(*vertices),polar_compare);memset(dp,0,(size_t)m*m*sizeof(*dp));
        for(int k=1;k<m;++k)for(int j=0;j<k;++j){int vj=vertices[j],vk=vertices[k];int64_t area=cross3(anchor,vj,vk);if(area<=0||!empty_triangle(anchor,vj,vk))continue;int64_t best=area;
            if(!inner_collinear[(size_t)anchor*N+vj])for(int h=0;h<j;++h){int64_t previous=dp[(size_t)h*m+j];if(previous&&cross3(vertices[h],vj,vk)>0&&previous+area>best)best=previous+area;}
            dp[(size_t)j*m+k]=best;if(best>global_best)global_best=best;
        }
    }
    printf("%lld.%d\n",(long long)(global_best/2),(int)(global_best&1?5:0));free(dp);free(inner_collinear);free(left_bits);return 0;
}
