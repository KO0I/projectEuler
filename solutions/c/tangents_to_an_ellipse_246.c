#include <math.h>
#include <stdint.h>
#include <stdio.h>

/*
 * Project Euler 246: Tangents to an Ellipse
 *
 * The ellipse has center (3000,1500), a^2=56,250,000 and b^2=31,250,000.
 * In centered coordinates, eliminating the two tangent slopes with Vieta's
 * formulas turns angle>45 degrees into the exact integer inequality
 *
 * (x^2+y^2)^2 - 300000000*x^2 - 400000000*y^2
 *                  + 14687500000000000 < 0.
 *
 * Its positive left side outside the relevant band also implies the point is
 * exterior to the ellipse, so no separate tangent-existence test is needed.
 * For fixed x this expression is an upward quadratic in y^2: false, then one
 * contiguous true interval, then false.  We locate a point near its vertex and
 * binary-search the two integer boundaries.  All final decisions use exact
 * __int128 arithmetic; floating point is used only to seed the vertex search.
 */

typedef __int128 I128;

static int qualifies(int64_t x,int64_t y){
    I128 xx=(I128)x*x, yy=(I128)y*y, radius=xx+yy;
    const I128 a2=56250000,b2=31250000;
    /* Tangents require strict exteriority.  Up through the director circle
       their included angle is at least 90 degrees and needs no tan test. */
    if(b2*xx+a2*yy<=a2*b2)return 0;
    if(radius<=a2+b2)return 1;
    I128 value=radius*radius-(I128)300000000*xx
        -(I128)400000000*yy+(I128)14687500000000000LL;
    return value<0;
}

int main(void){
    uint64_t count=0;
    /* The outer x-intercept is below 16000; 20000 is an intentionally loose,
       visibly safe bound.  Symmetry in y is counted arithmetically. */
    for(int64_t x=-20000;x<=20000;++x){
        int64_t vertex_square=200000000-x*x;
        int middle=vertex_square>0?(int)sqrt((double)vertex_square):0;
        if(!qualifies(x,middle)&&!qualifies(x,middle+1))continue;
        if(!qualifies(x,middle))++middle;

        int low=0,high=middle;
        while(low<high){int mid=low+(high-low)/2;if(qualifies(x,mid))high=mid;else low=mid+1;}
        int first=low;
        low=middle;high=25000; /* outer y-intercept is below 19000 */
        while(low<high){int mid=low+(high-low+1)/2;if(qualifies(x,mid))low=mid;else high=mid-1;}
        int last=low;
        count+=(uint64_t)(first==0?2*last+1:2*(last-first+1));
    }
    printf("%llu\n",(unsigned long long)count);
    return 0;
}
