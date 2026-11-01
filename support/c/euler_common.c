/* Corrected Project Euler implementations. Historical attempts remain in Git.
 * Each numbered C program is a thin entry point into this shared module.
 */
#include "euler_common.h"
#include <ctype.h>
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <gmp.h>

static int prime(uint64_t n) {
    if(n<2)return 0;
    if(n%2==0)return n==2;
    for(uint64_t d=3;d<=n/d;d+=2)if(n%d==0)return 0;
    return 1;
}
static unsigned gcd(unsigned a,unsigned b){while(b){unsigned t=a%b;a=b;b=t;}return a;}
static unsigned count_divisors(uint64_t n) {
    unsigned prod=1;
    for(uint64_t p=2;p<=n/p;p+=(p==2?1:2)) {
        if(n%p)continue;
        unsigned exponent=1;
        do{n/=p;++exponent;}while(n%p==0);
        prod*=exponent;
    }
    if(n>1)prod*=2;
    return prod;
}
static unsigned divisor_sum(unsigned n){
    if(n<2)return 0;
    unsigned sum=1;
    for(unsigned d=2;d<=n/d;d++)if(n%d==0){sum+=d;if(d!=n/d)sum+=n/d;}
    return sum;
}
static int palindrome(unsigned n){unsigned old=n,rev=0;do{rev=rev*10+n%10;n/=10;}while(n);return rev==old;}
static unsigned char *sieve(unsigned limit){
    unsigned char *p=malloc(limit+1);
    if(!p)return NULL;
    memset(p,1,limit+1);p[0]=p[1]=0;
    for(unsigned i=2;i<=limit/i;i++)if(p[i])for(unsigned j=i*i;j<=limit;j+=i)p[j]=0;
    return p;
}
static long long problem8(void) {
    FILE *f=fopen("tests/data/p008.txt","r");if(!f)return -1;
    char digits[1024];unsigned n=0;int ch;
    while((ch=fgetc(f))!=EOF)if(ch>='0'&&ch<='9'){
        if(n>=sizeof digits){fclose(f);return -1;}
        digits[n++]=(char)ch;
    }
    fclose(f);if(n!=1000)return -1;
    long long best=0;
    for(unsigned i=0;i+13<=n;i++){
        long long product=1;
        for(unsigned j=0;j<13;j++)product*=digits[i+j]-'0';
        if(product>best)best=product;
    }
    return best;
}
static long long problem11(void) {
    FILE *f=fopen("tests/data/p011.txt","r");if(!f)return -1;
    int grid[20][20];
    for(int y=0;y<20;y++)for(int x=0;x<20;x++)if(fscanf(f,"%d",&grid[y][x])!=1){fclose(f);return -1;}
    fclose(f);long long best=0;
    const int dx[]={1,0,1,-1},dy[]={0,1,1,1};
    for(int y=0;y<20;y++)for(int x=0;x<20;x++)for(int d=0;d<4;d++){
        int xx=x+3*dx[d],yy=y+3*dy[d];
        if(xx<0||xx>=20||yy<0||yy>=20)continue;
        long long product=1;
        for(int k=0;k<4;k++)product*=grid[y+k*dy[d]][x+k*dx[d]];
        if(product>best)best=product;
    }
    return best;
}
static long long digit_sum(const mpz_t z){
    char *s=mpz_get_str(NULL,10,z);if(!s)return -1;
    long long total=0;for(char *p=s;*p;p++)if(isdigit((unsigned char)*p))total+=*p-'0';
    free(s);return total;
}
static long long problem13(void){
    FILE *f=fopen("lsum","r");if(!f)return -1;
    mpz_t total,tmp;mpz_inits(total,tmp,NULL);char line[256];int count=0;
    while(fgets(line,sizeof line,f))if(mpz_set_str(tmp,line,10)==0){mpz_add(total,total,tmp);count++;}
    fclose(f);
    char *s=mpz_get_str(NULL,10,total);
    long long out=-1;
    if(count==100&&s&&strlen(s)>=10){char prefix[11];memcpy(prefix,s,10);prefix[10]=0;out=atoll(prefix);}
    free(s);mpz_clears(total,tmp,NULL);return out;
}
static int number_letters(unsigned n){
    static const int small[]={0,3,3,5,4,4,3,5,5,4,3,6,6,8,8,7,7,9,8,8};
    static const int tens[]={0,0,6,6,5,5,5,7,6,6};
    if(n==1000)return 11; /* one thousand */
    int length=0;
    if(n>=100){length+=small[n/100]+7;n%=100;if(n)length+=3; /* and */}
    if(n>=20){length+=tens[n/10];n%=10;}
    if(n)length+=small[n];
    return length;
}
static int leap(int year){return year%4==0&&(year%100!=0||year%400==0);}
static int namecmp(const void *a,const void *b){return strcmp(*(const char *const*)a,*(const char *const*)b);}
static long long problem22(void){
    FILE *f=fopen("names.txt","r");if(!f)return -1;
    char *names[6000];size_t count=0;int ch;
    while((ch=fgetc(f))!=EOF){
        if(ch!='"')continue;
        char buf[128];size_t len=0;
        while((ch=fgetc(f))!=EOF&&ch!='"'){
            if(len+1>=sizeof buf){fclose(f);return -1;}
            buf[len++]=(char)ch;
        }
        buf[len]=0;
        if(count>=6000){fclose(f);return -1;}
        names[count]=malloc(len+1);if(!names[count]){fclose(f);return -1;}
        memcpy(names[count],buf,len+1);count++;
    }
    fclose(f);if(count!=5163)return -1;
    qsort(names,count,sizeof names[0],namecmp);
    long long total=0;
    for(size_t i=0;i<count;i++){
        unsigned score=0;
        for(char *p=names[i];*p;p++)if(*p>='A'&&*p<='Z')score+=*p-'A'+1;
        total+=(long long)(i+1)*score;free(names[i]);
    }
    return total;
}
static long long problem29(void){
    char *values[9801];size_t idx=0;
    mpz_t v;mpz_init(v);
    for(unsigned a=2;a<=100;a++)for(unsigned b=2;b<=100;b++){
        mpz_ui_pow_ui(v,a,b);values[idx++]=mpz_get_str(NULL,10,v);
    }
    mpz_clear(v);
    qsort(values,idx,sizeof values[0],namecmp);
    long long count=0;
    for(size_t i=0;i<idx;i++)if(!i||strcmp(values[i],values[i-1])!=0)count++;
    for(size_t i=0;i<idx;i++)free(values[i]);
    return count;
}
static unsigned factorial(unsigned n){unsigned f=1;for(unsigned k=2;k<=n;k++)f*=k;return f;}
static unsigned digit_power_sum(unsigned n,const unsigned table[10]){
    unsigned total=0;do{total+=table[n%10];n/=10;}while(n);return total;
}
static int pentagonal(uint64_t n){
    if(!n)return 0;
    uint64_t m=24*n+1;
    uint64_t root=(uint64_t)sqrtl((long double)m);
    while((root+1)<=m/(root+1))root++;
    while(root>m/root)root--;
    return root*root==m&&(root+1)%6==0;
}
static unsigned distinct_factors(unsigned n){
    unsigned count=0;
    for(unsigned p=2;p<=n/p;p++){
        if(n%p)continue;
        count++;
        do{n/=p;}while(n%p==0);
    }
    if(n>1)count++;
    return count;
}
static long long problem55(void){
    mpz_t n,rev;mpz_inits(n,rev,NULL);unsigned total=0;
    for(unsigned start=1;start<10000;start++){
        mpz_set_ui(n,start);
        int reached=0;
        for(int i=0;i<50;i++){
            char *str=mpz_get_str(NULL,10,n);size_t len=strlen(str);
            for(size_t l=0;l<len/2;l++){char t=str[l];str[l]=str[len-1-l];str[len-1-l]=t;}
            mpz_set_str(rev,str,10);free(str);
            mpz_add(n,n,rev);
            str=mpz_get_str(NULL,10,n);len=strlen(str);
            reached=1;
            for(size_t l=0;l<len/2;l++)if(str[l]!=str[len-1-l]){reached=0;break;}
            free(str);
            if(reached)break;
        }
        if(!reached)total++;
    }
    mpz_clears(n,rev,NULL);return total;
}
static long long problem92(void){
    unsigned char cache[568]={0};cache[1]=1;cache[89]=89;
    const unsigned sq[]={0,1,4,9,16,25,36,49,64,81};
    unsigned answer=0;
    for(unsigned n=1;n<10000000;n++){
        unsigned x=digit_power_sum(n,sq);
        while(!cache[x])x=digit_power_sum(x,sq);
        if(cache[x]==89)answer++;
    }
    return answer;
}

long long euler_solve(int problem){
    switch(problem){
        case 1:{long long s=0;for(int n=1;n<1000;n++)if(n%3==0||n%5==0)s+=n;return s;}
        case 2:{long long a=1,b=2,s=0;while(b<=4000000){if(!(b%2))s+=b;long long t=a+b;a=b;b=t;}return s;}
        case 3:{uint64_t n=600851475143ULL,p=2,last=1;while(p<=n/p){if(n%p==0){last=p;n/=p;}else p+=(p==2?1:2);}return (long long)(n>1?n:last);}
        case 4:{int best=0;for(int a=100;a<=999;a++)for(int b=a;b<=999;b++){int product=a*b;if(product>best&&palindrome(product))best=product;}return best;}
        case 5:{unsigned result=1;for(unsigned n=2;n<=20;n++)result=result/gcd(result,n)*n;return result;}
        case 6:{long long sum=100LL*101/2,sq=100LL*101*201/6;return sum*sum-sq;}
        case 7:{unsigned count=0,n=1;while(count<10001){n++;if(prime(n))count++;}return n;}
        case 8:return problem8();
        case 9:{for(int a=1;a<334;a++)for(int b=a+1;b<500;b++){int c=1000-a-b;if(c>b&&a*a+b*b==c*c)return a*b*c;}return -1;}
        case 10:{unsigned char *p=sieve(2000000);if(!p)return -1;long long sum=0;for(int n=2;n<2000000;n++)if(p[n])sum+=n;free(p);return sum;}
        case 11:return problem11();
        case 12:{for(uint64_t n=1;;n++)if(count_divisors(n*(n+1)/2)>500)return (long long)(n*(n+1)/2);}
        case 13:return problem13();
        case 14:{unsigned best_n=1,best_len=0;for(unsigned start=1;start<1000000;start++){
            uint64_t n=start;unsigned len=1;
            while(n!=1){n=(n%2)?3*n+1:n/2;len++;}
            if(len>best_len){best_len=len;best_n=start;}
        }return best_n;}
        case 16:{mpz_t n;mpz_init(n);mpz_ui_pow_ui(n,2,1000);long long s=digit_sum(n);mpz_clear(n);return s;}
        case 17:{unsigned sum=0;for(unsigned n=1;n<=1000;n++)sum+=number_letters(n);return sum;}
        case 19:{int w=1,total=0;static const int days[]={31,28,31,30,31,30,31,31,30,31,30,31};for(int y=1900;y<=2000;y++)for(int m=0;m<12;m++){
            if(y>=1901&&w==0)total++;
            w=(w+days[m]+(m==1&&leap(y)))%7;
        }return total;}
        case 20:{mpz_t n;mpz_init(n);mpz_fac_ui(n,100);long long s=digit_sum(n);mpz_clear(n);return s;}
        case 21:{unsigned sum=0;for(unsigned n=2;n<10000;n++){unsigned pair=divisor_sum(n);if(pair!=n&&pair>0&&divisor_sum(pair)==n)sum+=n;}return sum;}
        case 22:return problem22();
        case 25:{mpz_t a,b,limit,tmp;mpz_inits(a,b,limit,tmp,NULL);mpz_set_ui(a,1);mpz_set_ui(b,1);mpz_ui_pow_ui(limit,10,999);unsigned i=2;
            while(mpz_cmp(b,limit)<0){mpz_add(tmp,a,b);mpz_set(a,b);mpz_set(b,tmp);i++;}
            mpz_clears(a,b,limit,tmp,NULL);return i;}
        case 27:{int best=0,result=0;for(int a=-999;a<1000;a++)for(int b=-999;b<1000;b++){
            int n=0;
            while(1){long long v=(long long)n*n+(long long)a*n+b;if(v<2||!prime((uint64_t)v))break;n++;}
            if(n>best){best=n;result=a*b;}
        }return result;}
        case 28:{long long sum=1;for(long long side=3;side<=1001;side+=2)sum+=4*side*side-6*(side-1);return sum;}
        case 29:return problem29();
        case 30:{unsigned pow5[10];for(unsigned n=0;n<10;n++){pow5[n]=1;for(int k=0;k<5;k++)pow5[n]*=n;}unsigned sum=0;
            for(unsigned n=2;n<=354294;n++)if(digit_power_sum(n,pow5)==n)sum+=n;
            return sum;}
        case 34:{unsigned f[10];for(int n=0;n<10;n++)f[n]=factorial(n);unsigned sum=0;
            for(unsigned n=3;n<=2540160;n++)if(digit_power_sum(n,f)==n)sum+=n;
            return sum;}
        case 35:{unsigned char *p=sieve(1000000);if(!p)return -1;unsigned count=0;
            for(unsigned n=2;n<1000000;n++)if(p[n]){
                unsigned power=1;while(power<=n/10)power*=10;
                unsigned rotated=n;int good=1;
                do{rotated=rotated/10+(rotated%10)*power;if(!p[rotated]){good=0;break;}}while(rotated!=n);
                count+=good;
            }free(p);return count;}
        case 39:{int best=0,best_p=0;for(int p=12;p<=1000;p++){int count=0;
            for(int a=1;a<p/3;a++)for(int b=a+1;b<(p-a)/2;b++){int c=p-a-b;if(a*a+b*b==c*c)count++;}
            if(count>best){best=count;best_p=p;}
        }return best_p;}
        case 40:{unsigned targets[]={1,10,100,1000,10000,100000,1000000};int pos=0;unsigned digit_pos=0;
            long long product=1;char buf[24];for(unsigned n=1;pos<7;n++){
                sprintf(buf,"%u",n);
                for(char *p=buf;*p;p++){digit_pos++;if(digit_pos==targets[pos]){product*=*p-'0';pos++;if(pos==7)break;}}
            }return product;}
        case 44:{long long best=0;for(uint64_t j=2;j<5000;j++){
            uint64_t pj=j*(3*j-1)/2;
            for(uint64_t k=j-1;k>0;k--){uint64_t pk=k*(3*k-1)/2,diff=pj-pk;
                if(best&&(long long)diff>=best)break;
                if(pentagonal(diff)&&pentagonal(pj+pk))best=(long long)diff;
            }
            if(best&&3*j-2>(uint64_t)best)break;
        }return best?best:-1;}
        case 46:{for(unsigned n=9;;n+=2)if(!prime(n)){
            int found=0;
            for(unsigned p=2;p<n;p++)if(prime(p)){
                unsigned half=(n-p)/2;if((n-p)%2)continue;
                unsigned root=(unsigned)sqrt((double)half);
                if(root*root==half){found=1;break;}
            }
            if(!found)return n;
        }}
        case 47:{unsigned streak=0;for(unsigned n=2;n<1000000;n++){
            if(distinct_factors(n)==4){if(++streak==4)return n-3;}else streak=0;
        }return -1;}
        case 48:{mpz_t sum,base,term,mod;mpz_inits(sum,base,term,mod,NULL);mpz_ui_pow_ui(mod,10,10);
            for(unsigned n=1;n<=1000;n++){mpz_set_ui(base,n);mpz_powm_ui(term,base,n,mod);mpz_add(sum,sum,term);mpz_mod(sum,sum,mod);}
            long long result=(long long)mpz_get_ui(sum);mpz_clears(sum,base,term,mod,NULL);return result;}
        case 50:{unsigned char *p=sieve(1000000);if(!p)return -1;
            unsigned count=0;uint64_t prefix[80001]={0};
            for(unsigned n=2;n<1000000;n++)if(p[n]){prefix[count+1]=prefix[count]+n;count++;}
            unsigned best_len=0,best_prime=0;
            for(unsigned i=0;i<count;i++){
                for(unsigned j=i+best_len+1;j<=count;j++){
                    uint64_t sum=prefix[j]-prefix[i];if(sum>=1000000)break;
                    if(p[sum]){best_len=j-i;best_prime=(unsigned)sum;}
                }
            }free(p);return best_prime;}
        case 55:return problem55();
        case 69:{unsigned n=1;for(unsigned p=2;;p++)if(prime(p)){if(n>1000000/p)break;n*=p;}return n;}
        case 92:return problem92();
        default:return -1;
    }
}
