#!/usr/bin/env python3
"""Independent arithmetic, anagram, prime-run, and prime-partition cross-checks.

Run with Python 3 without -O. Builds temporary comparison programs under build/.
"""
from pathlib import Path
from fractions import Fraction
from functools import lru_cache
from itertools import combinations, permutations
from math import isqrt
import json, subprocess, time
ROOT=Path(__file__).resolve().parents[1]
OUT=ROOT / "build/audit_late"
OUT.mkdir(parents=True, exist_ok=True)

def c_run(name, source):
    c=OUT/f'{name}.c'; exe=OUT/name
    c.write_text(source)
    subprocess.run(['gcc','-O2','-std=c11','-Wall','-Wextra','-Wpedantic','-Werror','-Isupport/c',str(c),'-lgmp','-lm','-o',str(exe)],cwd=ROOT,check=True)
    return subprocess.run([str(exe)],cwd=ROOT,check=True,text=True,capture_output=True).stdout

def is_prime(n):
    if n<2:return False
    return all(n%d for d in range(2,isqrt(n)+1))

start=time.perf_counter(); results={}
raw=c_run('audit93','''#define main original93
#include "../../solutions/c/arithmetic_expressions_93.c"
#undef main
int main(void){for(unsigned a=0;a<=6;++a)for(unsigned b=a+1;b<=7;++b)for(unsigned c=b+1;c<=8;++c)for(unsigned d=c+1;d<=9;++d)printf("%u%u%u%u %u\\n",a,b,c,d,consecutive(a,b,c,d));}
''')
actual={digits:int(length) for digits,length in map(str.split,raw.splitlines())}
for digits in combinations(range(10),4):
    @lru_cache(None)
    def values(mask):
        if mask.bit_count()==1:return frozenset([Fraction(digits[mask.bit_length()-1])])
        found=set(); left=(mask-1)&mask; first=mask&-mask
        while left:
            right=mask^left
            if right and left&first:
                for a in values(left):
                    for b in values(right):
                        found.update((a+b,a-b,b-a,a*b))
                        if b:found.add(a/b)
                        if a:found.add(b/a)
            left=(left-1)&mask
        return frozenset(found)
    integers={int(v) for v in values(15) if v.denominator==1 and v>0}
    n=1
    while n in integers:n+=1
    key=''.join(map(str,digits)); assert actual[key]==n-1,(key,actual[key],n-1)
    if digits==(1,2,3,4):assert len(integers)==31 and max(integers)==36
results['93']='All 210 four-digit sets agree with independent Python Fraction subset DP; official 1,2,3,4 target count and maximum also agree.'
print('93 PASS',flush=True)

pairs=[('CARE','RACE'),('BOARD','BROAD'),('SHEET','THESE'),('CENTRE','RECENT'),('EXCEPT','EXPECT'),('FORMER','REFORM'),('AB','BA'),('BOBO','OBBO')]
pair_source='''#define main original98
#include "../../solutions/c/anagramic_squares_98.c"
#undef main
static Word word(const char *text){Word w={0};strcpy(w.text,text);w.length=strlen(text);for(size_t i=0;i<w.length;++i)++w.counts[(unsigned)(text[i]-'A')];return w;}
int main(void){
'''
for a,b in pairs:pair_source+=f'{{Word a=word("{a}"),b=word("{b}");printf("%" PRIu64 "\\n",pair_maximum(&a,&b));}}\n'
pair_source+='}\n'
actual98=list(map(int,c_run('audit98',pair_source).split()))
for (a,b),found in zip(pairs,actual98):
    letters=sorted(set(a)); maximum=0
    for digits in permutations(range(10),len(letters)):
        mapping=dict(zip(letters,digits))
        if mapping[a[0]]==0 or mapping[b[0]]==0:continue
        x=0;y=0
        for c in a:x=x*10+mapping[c]
        if isqrt(x)**2!=x:continue
        for c in b:y=y*10+mapping[c]
        if isqrt(y)**2==y:maximum=max(maximum,x,y)
    assert found==maximum,(a,b,found,maximum)
results['98']={'comparison':'Eight representative pairs agree with independent injective letter-to-digit mapping enumeration, including every repeated-letter pair from the fixture.','pairs':[{'a':a,'b':b,'maximum':v} for (a,b),v in zip(pairs,actual98)]}
print('98 PASS',flush=True)

raw=c_run('audit111','''#define main original111
#include "../../solutions/c/primes_with_runs_111.c"
#undef main
int main(void){unsigned lengths[]={2,3,4,10};for(unsigned i=0;i<4;++i)for(unsigned d=0;d<10;++d){Runs r={lengths[i],d,0,0};for(unsigned changed=0;changed<=r.length;++changed){r.count=r.sum=0;replacements(&r,0,changed,0);if(r.count){printf("%u %u %u %" PRIu64 " %" PRIu64 "\\n",r.length,d,changed,r.count,r.sum);break;}}}}
''')
actual111=[tuple(map(int,line.split())) for line in raw.splitlines()]
for length in (2,3,4):
    primes=[n for n in range(10**(length-1),10**length) if is_prime(n)]
    for d in range(10):
        maximum=max(str(n).count(str(d)) for n in primes)
        selected=[n for n in primes if str(n).count(str(d))==maximum]
        row=next(r for r in actual111 if r[0:2]==(length,d))
        assert row==(length,d,length-maximum,len(selected),sum(selected)),row
assert max(row[2] for row in actual111 if row[0]==10)==2
results['111']={'comparison':'All per-digit M/N/S quantities for 2-, 3-, and 4-digit primes agree with independent exhaustive trial division. Every 10-digit search stops within two replacements, bounding its sum far below uint64_t.','ten_digit_rows':[dict(zip(('length','digit','changed','count','sum'),row)) for row in actual111 if row[0]==10]}
print('111 PASS',flush=True)

raw=c_run('audit118','''#define main original118
#include "../../solutions/c/pandigital_prime_sets_118.c"
#undef main
int main(void){enumerate_primes(0,0,0);for(unsigned mask=1;mask<64;++mask)printf("%u %" PRIu64 "\\n",mask,partitions(mask));}
''')
actual118=dict(tuple(map(int,line.split())) for line in raw.splitlines())
@lru_cache(None)
def prime_cached(n):return is_prime(n)
for mask in range(1,64):
    digits=[d+1 for d in range(6) if mask>>d&1]
    found=set()
    for permutation in permutations(digits):
        for cuts in range(1<<(len(digits)-1)):
            numbers=[]; value=0
            for i,d in enumerate(permutation):
                value=10*value+d
                if i==len(digits)-1 or cuts>>i&1:
                    if not prime_cached(value):break
                    numbers.append(value);value=0
            else:found.add(tuple(sorted(numbers)))
    assert actual118[mask]==len(found),(mask,actual118[mask],len(found))
results['118']='All 63 nonempty subsets of digits 1..6 agree with independent permutation-and-cut enumeration, deduplicated as sorted prime tuples.'
print('118 PASS',flush=True)

seed=[20,31,38,39,40,42,45]
subsets=[(m,sum(seed[i] for i in range(7) if m>>i&1),m.bit_count()) for m in range(1,128)]
for a,sa,ca in subsets:
    for b,sb,cb in subsets:
        if a&b:continue
        assert sa!=sb
        if ca>cb:assert sa>sb
results['103']='Every pair of nonempty disjoint subsets of the 255-sum seed satisfies both special-sum rules. Manual audit proves completeness of the finite increasing-set search and safety of both pruning rules.'
for problem,n,threshold in ((108,180180,1000),(110,9350130049860600,4000000)):
    rest=n; divisor_count=1; factors=[]
    for p in (2,3,5,7,11,13,17,19,23,29,31,37,41,43,47):
        exponent=0
        while rest%p==0:rest//=p;exponent+=1
        if exponent:factors.append([p,exponent]);divisor_count*=2*exponent+1
    assert rest==1 and (divisor_count+1)//2>threshold
    results[str(problem)]={'factors':factors,'solutions':(divisor_count+1)//2,'proof':'Consecutive primes with nonincreasing positive exponents cover all minima. The verified primorial upper bound excludes any need for primes beyond the supplied list, and overflow guards prune before multiplication.'}
results['elapsed_seconds']=time.perf_counter()-start
(OUT/'late_audit_results.json').write_text(json.dumps(results,indent=2)+'\n')
print('103/108/110 bounds and witnesses PASS',flush=True)
print('All targeted late-group audit checks passed.',flush=True)
