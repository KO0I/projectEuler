/* Project Euler 147: Rectangles in Cross-hatched Grids
 *
 * Axis-aligned rectangles have the closed form C(w+1,2)C(h+1,2).  For tilted
 * rectangles, scale both coordinates by two: diagonal grid edges then become
 * ordinary integer steps.  Each admissible start, parity, width, and height
 * describes one tilted rectangle; boundary tests stop a direction as soon as
 * all larger shapes would also leave the board.
 *
 * The function swaps dimensions so the longer side has one canonical role.
 * Summing both orientations for every sub-grid 1<=w<=47, 1<=h<=43 counts the
 * requested collection.  The total is held in uint64_t. */
#include "euler_extra.h"

static uint64_t upright(unsigned w,unsigned h){return (uint64_t)w*(w+1)*h*(h+1)/4;}
static uint64_t diagonal(unsigned width,unsigned height){
    unsigned a=width,b=height;if(a<b){unsigned t=a;a=b;b=t;}
    uint64_t count=0;
    for(unsigned i=0;i<a;++i) for(unsigned j=0;j<b;++j) for(unsigned parity=0;parity<2;++parity){
        unsigned sx=2*i+1+parity, sy=2*j+2-parity;
        bool stop=false; unsigned max_height=UINT_MAX;
        for(unsigned rw=0;!stop;++rw){
            unsigned x=sx+rw;
            if(rw>=sy) break;
            unsigned y=sy-rw;
            for(unsigned rh=0;rh<max_height;++rh){
                unsigned ex=x+rh,ey=y+rh;
                if(ex>=2*a||ey>=2*b){if(max_height>rh)max_height=rh;stop=(rh==0);break;}
                ++count;
            }
        }
    }
    return count;
}
int main(void){
    uint64_t total=0;
    for(unsigned w=1;w<=47;++w)for(unsigned h=1;h<=43;++h)total+=upright(w,h)+diagonal(w,h);
    printf("%" PRIu64 "\n",total);return 0;
}
