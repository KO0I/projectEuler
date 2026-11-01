//! Direct enumeration and dynamic programming solutions (new Rust problems).
use crate::extra_helpers::*;
use num_bigint::BigUint;
use num_traits::{One, Zero};

pub fn solve(n: u32) -> Option<i64> {
    Some(match n {
        15 => {let mut ways=[1u64;21];for _ in 1..=20 {for j in 1..=20 {ways[j]+=ways[j-1];}} ways[20] as i64},
        18 => triangle_max(matrix(include_str!("../../../../tests/data/p018_triangle.txt"))) as i64,
        23 => {let divs=proper_divisor_sums(28123);
            let abundant:Vec<usize>=(1..=28123).filter(|&x|divs[x]>x).collect();
            let mut represented=vec![false;28124];
            for (i,&a) in abundant.iter().enumerate() {for &b in &abundant[i..] {
                if a+b>28123 {break;}represented[a+b]=true;
            }}(1..=28123).filter(|&x|!represented[x]).sum::<usize>() as i64},
        24 => {let mut pool:Vec<u64>=(0..10).collect();let mut rank=999999usize;let mut value=0u64;
            for remaining in (1..=10).rev(){let factorial=(1..remaining).product::<usize>();
                let idx=rank/factorial;rank%=factorial;value=value*10+pool.remove(idx);
            }value as i64},
        26 => {let mut best=(0usize,0i64);
            for denominator in 2usize..1000 {let mut seen=vec![usize::MAX;denominator];
                let mut r=1usize%denominator;let mut position=0;
                while r!=0 && seen[r]==usize::MAX {seen[r]=position;position+=1;r=(r*10)%denominator;}
                let length=if r==0 {0}else{position-seen[r]};
                if length>best.0 {best=(length,denominator as i64);}
            }best.1},
        31 => {let mut ways=[0u64;201];ways[0]=1;
            for coin in [1,2,5,10,20,50,100,200] {for amount in coin..=200 {
                ways[amount]+=ways[amount-coin];
            }}ways[200] as i64},
        32 => {let mut found=vec![false;10000];let mut sum=0;
            for a in 1..100u64 {for b in a+1..=9999/a {let product=a*b;
                let text=format!("{a}{b}{product}");
                if text.len()==9 && pandigital_1_to_9(text.parse().unwrap()) && !found[product as usize] {
                    found[product as usize]=true;sum+=product;
                }
            }}sum as i64},
        33 => {let mut numerator=1u64;let mut denominator=1u64;
            for a in 10u64..100 {for b in a+1..100 {
                let ad=[a/10,a%10];let bd=[b/10,b%10];
                let mut valid=false;
                for i in 0..2 {for j in 0..2 {if ad[i]>0 && ad[i]==bd[j] && bd[1-j]>0 && a*bd[1-j]==b*ad[1-i] {valid=true;}}}
                if valid {numerator*=a;denominator*=b;}
            }}(denominator/gcd(numerator,denominator)) as i64},
        36 => (1u64..1_000_000).step_by(2).filter(|&v|palindrome(v,10)&&palindrome(v,2)).sum::<u64>() as i64,
        37 => {fn traverse(n:u64,sum:&mut u64,count:&mut usize) {
                let mut power=1;while power<=n/10 {power*=10;}
                if n>=10 && {let mut p=power;let mut ok=true;while p>1 {
                    if !prime(n%p) {ok=false;break;}p/=10;
                }ok} {*sum+=n;*count+=1;}
                for d in [1,3,7,9] {let next=10*n+d;if prime(next) {traverse(next,sum,count);}}
            }
            let mut sum=0;let mut count=0;
            for root in [2,3,5,7] {traverse(root,&mut sum,&mut count);}
            assert_eq!(count,11);sum as i64},
        38 => {let mut best=0u64;
            for start in 1..10000 {let mut joined=String::new();let mut n=1;
                while joined.len()<9 {joined.push_str(&(start*n).to_string());n+=1;}
                if joined.len()==9 && n>2 {
                    let value=joined.parse().unwrap();if pandigital_1_to_9(value) {best=best.max(value);}
                }
            }best as i64},
        41 => {let mut best=0u64;
            for len in [7usize,4] {let mut p:Vec<u8>=(1..=len as u8).collect();
                loop {let n=p.iter().fold(0u64,|v,&d|v*10+d as u64);
                    if prime(n) {best=best.max(n);}
                    if !next_permutation(&mut p) {break;}
                }
                if best>0 {break;}
            }best as i64},
        42 => {let words=include_str!("../../../../tests/data/p042_words.txt");
            words.split(',').filter(|w| {let w=w.trim().trim_matches('"');
                let sum:u64 = w.bytes().map(|b|(b-b'A'+1) as u64).sum();is_square(8*sum+1)
            }).count() as i64},
        43 => {fn search(at:usize,used:u16,n:u64)->u64 {
                const DIV:[u64;7]=[2,3,5,7,11,13,17];
                if at==10 {return n;}
                let mut total=0;
                for d in (if at==0 {1} else {0})..10 {
                    if used & (1<<d)!=0 {continue;}
                    let next=n*10+d;
                    if at>=3 && next%1000%DIV[at-3]!=0 {continue;}
                    total+=search(at+1,used|(1<<d),next);
                }total
            }search(0,0,0) as i64},
        45 => {let mut n=144u64;
            loop {let hex=n*(2*n-1);let discr=24*hex+1;let root=isqrt(discr);
                if root*root==discr && (root+1)%6==0 {break hex as i64;}
                n+=1;
            }},
        49 => {let p=sieve(9999);let mut answer=0;
            'outer:for a in 1000..10000usize {if !p[a] {continue;}
                for b in a+1..10000 {let c=2*b-a;if c>=10000 {break;}
                    if a==1487 && b==4817 {continue;}
                    if p[b]&&p[c]&&decimal_signature(a as u64)==decimal_signature(b as u64)
                        && decimal_signature(a as u64)==decimal_signature(c as u64) {
                        answer=(a as i64)*100_000_000+(b as i64)*10000+c as i64;break 'outer;
                    }
                }
            }answer},
        51 => {let mut answer=None;
            'length:for len in 2..=7 {let upper=10usize.pow(len as u32)-1;
                let lower=10usize.pow(len as u32-1);let p=sieve(upper);
                for n in lower..=upper {if !p[n] {continue;}
                    let d:Vec<usize>=(0..len).map(|k|(n/10usize.pow(k as u32))%10).collect();
                    for mask in 1..(1<<len) {let selected:Vec<usize>=(0..len).filter(|k|mask&(1<<k)!=0).collect();
                        if !selected.iter().all(|&k|d[k]==d[selected[0]]) {continue;}
                        let coefficient:usize=selected.iter().map(|&k|10usize.pow(k as u32)).sum();
                        let base=n-d[selected[0]]*coefficient;
                        let mut first=0;let mut count=0;
                        for replacement in 0..10 {if replacement==0 && (mask&(1<<(len-1))!=0) {continue;}
                            let candidate=base+replacement*coefficient;
                            if p[candidate] {if count==0 {first=candidate;}count+=1;}
                        }
                        if count==8 && first==n {answer=Some(n as i64);break 'length;}
                    }
                }
            }answer.expect("eight-prime family")},
        52 => {let mut lower=1u64;let mut upper=9u64;
            loop {let answer=(lower..=upper/6).find(|&n|{
                    let signature=decimal_signature(n);
                    (2..=6).all(|k|decimal_signature(k*n)==signature)
                });
                if let Some(v)=answer {break v as i64;}
                lower=upper+1;upper=upper*10+9;
            }},
        53 => {let mut row=[0u64;101];row[0]=1;let mut count=0;
            for n in 1..=100 {for j in (1..=n).rev(){row[j]=(row[j]+row[j-1]).min(1_000_001);}
                count+=(0..=n).filter(|&j|row[j]>1_000_000).count();
            }count as i64},
        56 => {let mut best=0;
            for a in 1u32..100 {let mut power=BigUint::one();for _ in 1..100 {
                power*=a;best=best.max(digits_sum_big(&power));
            }}best as i64},
        57 => {let (mut top,mut bottom)=(BigUint::from(3u32),BigUint::from(2u32));let mut count=0;
            for _ in 0..1000 {if top.to_string().len()>bottom.to_string().len() {count+=1;}
                let next=&top+&bottom*2u32;bottom+=&top;top=next;
            }count},
        58 => {let mut diagonal_primes=0;
            for side in (3u64..).step_by(2) {let square=side*side;
                for corner in 1..=3 {if prime(square-corner*(side-1)) {diagonal_primes+=1;}}
                if 10*diagonal_primes<2*side-1 {break side as i64;}
            }},
        63 => {let mut count=0;for base in 1u32..10 {
                let mut value=BigUint::one();for exponent in 1..=22 {value*=base;
                    let length=value.to_string().len();if length==exponent {count+=1;}
                    if length<exponent {break;}
                }
            }count},
        64 => {let mut count=0;for n in 2u64..=10000 {
                let a0=isqrt(n);if a0*a0==n {continue;}
                let (mut m,mut d,mut a)=(0u64,1u64,a0);let mut period=0;
                loop {m=d*a-m;d=(n-m*m)/d;a=(a0+m)/d;period+=1;
                    if a==2*a0 {break;}
                }
                if period%2==1 {count+=1;}
            }count},
        65 => {let (mut numerator,mut previous)=(BigUint::one(),BigUint::zero());
            for k in (0..100).rev() {let a=if k==0 {2} else if k%3==2 {2*(k/3+1)} else {1};
                let next=&numerator*(a as u32)+&previous;previous=numerator;numerator=next;
            }digits_sum_big(&numerator) as i64},
        67 => triangle_max(matrix(include_str!("../../../../tests/data/p067_triangle.txt"))) as i64,
        70 => {let phi=totients(10_000_000);let mut best_n=0u64;let mut best_phi=1u64;
            for n in 2usize..10_000_000 {let p=phi[n] as u64;
                if decimal_signature(n as u64)!=decimal_signature(p) {continue;}
                if best_n==0 || (n as u64)*best_phi < best_n*p {best_n=n as u64;best_phi=p;}
            }best_n as i64},
        71 => {let mut best_n=0u64;let mut best_d=1u64;
            for d in 2..=1_000_000u64 {let n=(3*d-1)/7;
                if n*best_d>best_n*d {best_n=n;best_d=d;}
            }best_n as i64},
        72 => {let p=totients(1_000_000);p[2..].iter().map(|&v|v as i64).sum()},
        73 => {fn count(a:u32,b:u32,c:u32,d:u32)->u64 {if b+d>12000 {return 0;}
                1+count(a,b,a+c,b+d)+count(a+c,b+d,c,d)
            }count(1,3,1,2) as i64},
        75 => {let mut ways=vec![0u8;1_500_001];
            for m in 2u64..=866 {for n in 1..m {if (m-n)%2==0||gcd(m,n)!=1 {continue;}
                let perimeter=2*m*(m+n);if perimeter>1_500_000 {break;}
                for p in (perimeter as usize..=1_500_000).step_by(perimeter as usize) {ways[p]=ways[p].saturating_add(1);}
            }}ways.iter().filter(|&&n|n==1).count() as i64},
        76 => {let mut ways=[0u64;101];ways[0]=1;
            for part in 1..100 {for n in part..=100 {ways[n]+=ways[n-part];}}ways[100] as i64},
        77 => {let mut n=2usize;
            loop {let mut ways=vec![0u64;n+1];ways[0]=1;
                for p in primes(n) {let p=p as usize;for x in p..=n {ways[x]+=ways[x-p];}}
                if ways[n]>5000 {break n as i64;}n+=1;
            }},
        78 => {let modulo=1_000_000i64;let mut partitions=vec![1i64];
            let mut answer=0;
            for n in 1..100_000usize {let mut p=0i64;let mut k=1i64;
                loop {let g1=k*(3*k-1)/2;let g2=k*(3*k+1)/2;
                    if g1 as usize>n {break;}
                    let sign=if k%2==1 {1}else{-1};
                    p+=sign*partitions[n-g1 as usize];
                    if g2 as usize<=n {p+=sign*partitions[n-g2 as usize];}
                    k+=1;
                }p=p.rem_euclid(modulo);partitions.push(p);
                if p==0 {answer=n as i64;break;}
            }answer},
        85 => {let mut closest=u64::MAX;let mut best=0;
            for w in 1..=2000u64 {for h in 1..=w {
                let number=w*(w+1)*h*(h+1)/4;
                let diff=number.abs_diff(2_000_000);
                if diff<closest {closest=diff;best=w*h;}
                if number>2_100_000 {break;}
            }}best as i64},
        86 => {let mut total=0u64;
            for maximum in 1u64.. {for sum in 2..=2*maximum {
                if is_square(maximum*maximum+sum*sum) {
                    let low=if sum>maximum {sum-maximum}else{1};
                    let high=(sum/2).min(maximum);
                    if high>=low {total+=high-low+1;}
                }
            }if total>1_000_000 {break maximum as i64;}
            }},
        87 => {let limit=50_000_000u64;let roots=primes(isqrt(limit) as usize);
            let mut seen=vec![false;limit as usize];
            for &p in &roots {let a=p*p;if a>=limit {break;}
                for &q in &roots {let b=q*q*q;if a+b>=limit {break;}
                    for &r in &roots {let c=r*r*r*r;if a+b+c>=limit {break;}
                        seen[(a+b+c) as usize]=true;
                    }
                }
            }seen.into_iter().filter(|&b|b).count() as i64},
        89 => {let text=include_str!("../../../../tests/data/p089_roman.txt");
            fn value(ch:u8)->usize {match ch {b'I'=>1,b'V'=>5,b'X'=>10,b'L'=>50,b'C'=>100,b'D'=>500,b'M'=>1000,_=>0}}
            fn canonical(mut number:usize)->String {let mut result=String::new();
                for (num,sym) in [(1000,"M"),(900,"CM"),(500,"D"),(400,"CD"),(100,"C"),(90,"XC"),(50,"L"),(40,"XL"),(10,"X"),(9,"IX"),(5,"V"),(4,"IV"),(1,"I")] {
                    while number>=num {result.push_str(sym);number-=num;}
                }result
            }
            text.lines().filter(|l|!l.is_empty()).map(|line|{
                let b=line.trim().as_bytes();let mut total=0;for i in 0..b.len() {
                    let v=value(b[i]);if i+1<b.len()&&v<value(b[i+1]) {total-=v as i64;}else{total+=v as i64;}
                }b.len() as i64-canonical(total as usize).len() as i64
            }).sum::<i64>()},
        91 => {let mut count=0;
            for x1 in 0i64..=50 {for y1 in 0i64..=50 {if x1==0&&y1==0 {continue;}
                for x2 in 0i64..=50 {for y2 in 0i64..=50 {if x2==0&&y2==0 || x1==x2&&y1==y2 {continue;}
                    let dot0=x1*x2+y1*y2;let dot1=x1*(x1-x2)+y1*(y1-y2);
                    let dot2=x2*(x2-x1)+y2*(y2-y1);
                    if dot0==0||dot1==0||dot2==0 {count+=1;}
                }}
            }}count/2},
        94 => {let mut sum=0i64;let (mut x,mut y)=(2i64,0i64);
            loop {let (nx,ny)=(2*x+3*y,x+2*y);x=nx;y=ny;
                let perimeter=if x%3==2 {x+2}else{x-2};
                if perimeter>1_000_000_000 {break;}
                let a=if x%3==2 {(x+1)/3} else {(x-1)/3};
                let b=if x%3==2 {a+1} else {a-1};
                if perimeter>0 && a>1 && b>0 && (b*y)%4==0 {sum+=perimeter;}
            }sum},
        97 => ((28433u64 * mod_pow(2,7_830_457,10_000_000_000)+1)%10_000_000_000) as i64,
        100 => {let (mut blue,mut total)=(15i64,21i64);
            while total<=1_000_000_000_000 {let next_blue=3*blue+2*total-2;
                let next_total=4*blue+3*total-3;blue=next_blue;total=next_total;
            }blue},
        106 => {let n=12u64;(2..=6).map(|k|{
                let pairs=choose(n,2*k)*choose(2*k,k)/2;
                let comparable=choose(2*k,k)/(k+1);
                (pairs-choose(n,2*k)*comparable) as i64
            }).sum()},
        109 => {let mut scores=Vec::new();for n in 1..=20 {scores.push(n);scores.push(2*n);scores.push(3*n);}
            scores.extend([25,50]);let mut doubles:Vec<i64>=(1..=20).map(|n|2*n).collect();doubles.push(50);
            let mut count=0;for &d in &doubles {if d<100 {count+=1;}
                for &a in &scores {if a+d<100 {count+=1;}}
                for (i,&a) in scores.iter().enumerate() {for &b in &scores[i..] {
                    if a+b+d<100 {count+=1;}
                }}
            }count},
        112 => {fn bouncy(n:u64)->bool {let d=digits(n);
                !(d.windows(2).all(|v|v[0]<=v[1]) || d.windows(2).all(|v|v[0]>=v[1]))
            }
            let mut count=0;let mut n=99u64;
            loop {n+=1;if bouncy(n){count+=1;}if 100*count==99*n {break n as i64;}}
        },
        113 => {let mut up=[0u64;10];let mut down=[0u64;10];for d in 1..=9 {up[d]=1;down[d]=1;}
            let mut sum=0u64;
            for _ in 1..=100 {sum+=up.iter().sum::<u64>()+down.iter().sum::<u64>()-9;
                let mut next_up=[0u64;10];let mut next_down=[0u64;10];
                for old in 0..=9 {for d in old..=9 {next_up[d]+=up[old];}
                    for d in 0..=old {next_down[d]+=down[old];}
                }up=next_up;down=next_down;
            }sum as i64},
        114 => {let mut ways=[0u64;51];ways[0]=1;
            for n in 1..=50 {ways[n]=ways[n-1];for red in 3..=n {ways[n]+=if red==n {1}else{ways[n-red-1]};}}
            ways[50] as i64},
        115 => {let mut ways=vec![1u64];
            for n in 1.. {let mut count=ways[n-1];for block in 50..=n {
                    count+=if block==n {1}else{ways[n-block-1]};
                }ways.push(count);
                if count>1_000_000 {break n as i64;}
            }},
        116 => {let mut sum=0u64;for size in [2usize,3,4] {let mut ways=[0u64;51];ways[0]=1;
                for n in 1..=50 {ways[n]=ways[n-1]+if n>=size {ways[n-size]}else{0};}
                sum+=ways[50]-1;
            }sum as i64},
        117 => {let mut ways=[0u64;51];ways[0]=1;
            for n in 1..=50 {for size in 1..=4 {if size<=n {ways[n]+=ways[n-size];}}}
            ways[50] as i64},
        120 => (3u64..=1000).map(|a|if a%2==0 {a*(a-2)}else{a*(a-1)}).sum::<u64>() as i64,
        _ => return None,
    })
}
