#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

/*
 * Project Euler 247: Squares Under a Hyperbola
 *
 * For a remaining region whose lower-left corner is (x,y), the next square's
 * upper-right corner lies on xy=1.  Solving (x+s)(y+s)=1 gives
 *
 *   s=(sqrt((x-y)^2+4)-x-y)/2.
 *
 * Removing that square creates exactly two independent child regions: one to
 * its right and one above it.  A max-heap of side lengths therefore reproduces
 * the global "largest remaining square" order without geometric rasterization.
 * Child indices increment left or below respectively.
 *
 * `relevant` counts queued regions with both coordinates <=3.  Once it reaches
 * zero no future (3,3) square can exist; the last popped exact match is the
 * requested largest serial number.
 */

typedef struct { double x,y,side; int left,below; } Square;
static Square *heap;static size_t heap_size,heap_capacity;

static double side_at(double x,double y){double d=x-y;return (sqrt(d*d+4.0)-x-y)/2.0;}
static void push(Square value){
    if(heap_size==heap_capacity){heap_capacity=heap_capacity?heap_capacity*2:1024;Square *grown=realloc(heap,heap_capacity*sizeof(*heap));if(!grown)exit(1);heap=grown;}
    size_t i=heap_size++;while(i){size_t parent=(i-1)/2;if(heap[parent].side>=value.side)break;heap[i]=heap[parent];i=parent;}heap[i]=value;
}
static Square pop_max(void){
    Square result=heap[0],last=heap[--heap_size];size_t i=0;
    while(2*i+1<heap_size){size_t child=2*i+1;if(child+1<heap_size&&heap[child+1].side>heap[child].side)++child;if(heap[child].side<=last.side)break;heap[i]=heap[child];i=child;}
    if(heap_size)heap[i]=last;
    return result;
}

int main(void){
    Square first={1.0,0.0,0.0,0,0};first.side=side_at(first.x,first.y);push(first);
    uint64_t serial=0,last_match=0;int relevant=1;
    while(relevant>0){
        Square current=pop_max();++serial;
        if(current.left<=3&&current.below<=3)--relevant;
        if(current.left==3&&current.below==3)last_match=serial;

        Square right={current.x+current.side,current.y,0.0,current.left+1,current.below};
        right.side=side_at(right.x,right.y);push(right);
        if(right.left<=3&&right.below<=3)++relevant;
        Square above={current.x,current.y+current.side,0.0,current.left,current.below+1};
        above.side=side_at(above.x,above.y);push(above);
        if(above.left<=3&&above.below<=3)++relevant;
    }
    printf("%llu\n",(unsigned long long)last_match);free(heap);return 0;
}
