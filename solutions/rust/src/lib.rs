//! Correct Project Euler solutions using native Rust and num-bigint.
//! Each bin/pN.rs is independently executable; all share tested helpers.
pub mod extra_helpers;
mod extra;
use num_bigint::BigUint;
use num_traits::{One, Zero};
use std::collections::HashSet;

fn prime(n: u64) -> bool {
    if n < 2 { return false; }
    if n % 2 == 0 { return n == 2; }
    let mut d = 3;
    while d <= n / d { if n % d == 0 { return false; } d += 2; }
    true
}
fn sieve(limit: usize) -> Vec<bool> {
    let mut p = vec![true; limit + 1]; p[0] = false; p[1] = false;
    for n in 2..=limit {
        if n > limit / n { break; }
        if p[n] { for m in (n*n..=limit).step_by(n) { p[m] = false; } }
    }
    p
}
fn gcd(mut a: u64, mut b: u64) -> u64 { while b != 0 { (a,b)=(b,a%b); } a }
fn divisor_count(mut n: u64) -> u32 {
    let mut count = 1;
    let mut p = 2;
    while p <= n/p {
        if n % p == 0 {
            let mut factor = 1;
            while n % p == 0 { n /= p; factor += 1; }
            count *= factor;
        }
        p += if p == 2 {1} else {2};
    }
    if n > 1 { count *= 2; }
    count
}
fn divisor_sum(n: u32) -> u32 {
    if n < 2 { return 0; }
    let mut sum = 1;
    for d in 2..=n { if d > n/d { break; }
        if n % d == 0 {sum += d;if n/d != d {sum += n/d;} }
    }
    sum
}
fn is_palindrome(n: u32) -> bool {
    let original=n; let (mut x,mut rev)=(n,0);
    while x>0 {rev=rev*10+x%10;x/=10;}
    rev==original
}
fn digit_sum(n: &BigUint) -> i64 {
    n.to_string().bytes().map(|c| i64::from(c-b'0')).sum()
}
fn digit_power_sum(mut n: u32, table: &[u32;10]) -> u32 {
    let mut sum=0;
    loop {sum+=table[(n%10) as usize]; n/=10; if n==0 {break;} }
    sum
}
fn distinct_factors(mut n: u32) -> u32 {
    let mut count=0;
    let mut p=2;
    while p<=n/p {
        if n%p==0 {count+=1;while n%p==0 {n/=p;} }
        p+=1;
    }
    if n>1 {count+=1;}
    count
}
fn pentagonal(n: u64) -> bool {
    if n==0 {return false;}
    let target=24*n+1;
    let mut root=(target as f64).sqrt() as u64;
    while root+1<=target/(root+1) {root+=1;}
    while root>target/root {root-=1;}
    root*root==target && (root+1)%6==0
}
fn factorial(n: u32) -> u32 { (2..=n).product() }
fn letters(mut n: usize) -> u32 {
    let small=[0,3,3,5,4,4,3,5,5,4,3,6,6,8,8,7,7,9,8,8];
    let tens=[0,0,6,6,5,5,5,7,6,6];
    if n==1000 {return 11;}
    let mut count=0;
    if n>=100 {count+=small[n/100]+7;n%=100;if n!=0 {count+=3;} }
    if n>=20 {count+=tens[n/10];n%=10;}
    if n>0 {count+=small[n];}
    count
}
fn leap(y:i32)->bool {y%4==0 && (y%100!=0||y%400==0)}

pub fn solve(n: u32) -> Option<i64> {
    Some(match n {
        1 => (1i64..1000).filter(|v|v%3==0||v%5==0).sum(),
        2 => {let (mut a,mut b,mut sum)=(1i64,2i64,0i64);while b<=4_000_000 {
            if b%2==0 {sum+=b;} (a,b)=(b,a+b);
        } sum},
        3 => {let mut n=600_851_475_143u64;let mut d=2u64;let mut last=1u64;
            while d<=n/d {if n%d==0 {last=d;n/=d;} else {d+=if d==2 {1}else{2};}}
            (if n>1 {n}else{last}) as i64},
        4 => {let mut best=0u32;for a in 100..=999 {for b in a..=999 {
            let prod=a*b;if prod>best&&is_palindrome(prod){best=prod;}
        }}i64::from(best)},
        5 => (2..=20u64).fold(1u64,|acc,v|acc/gcd(acc,v)*v) as i64,
        6 => {let sum=100i64*101/2;sum*sum-100*101*201/6},
        7 => {let mut found=0;let mut candidate=1u64;while found<10001 {
            candidate+=1;if prime(candidate){found+=1;}
        }candidate as i64},
        8 => {let digits:Vec<u64>=include_str!("../../../tests/data/p008.txt")
                .bytes().filter(|b|b.is_ascii_digit()).map(|b|u64::from(b-b'0')).collect();
            assert_eq!(digits.len(),1000);
            digits.windows(13).map(|w|w.iter().copied().product::<u64>()).max().unwrap() as i64},
        9 => {let mut answer=0;for a in 1..334i64 {for b in a+1..500 {
            let c=1000-a-b;if c>b&&a*a+b*b==c*c {answer=a*b*c;break;}
        }if answer!=0 {break;}} answer},
        10 => {let p=sieve(2_000_000);(2..2_000_000).filter(|&v|p[v]).map(|v|v as i64).sum()},
        11 => {let v:Vec<i64>=include_str!("../../../tests/data/p011.txt")
            .split_whitespace().map(|v|v.parse().unwrap()).collect();
            assert_eq!(v.len(),400);let dirs=[(1,0),(0,1),(1,1),(-1,1)];let mut best=0;
            for y in 0..20i32 {for x in 0..20i32 {for (dx,dy) in dirs {
                let xx=x+3*dx;let yy=y+3*dy;
                if xx<0||xx>=20||yy<0||yy>=20 {continue;}
                let mut product=1;
                for step in 0..4i32 {let j=((y+step*dy)*20+x+step*dx) as usize;product*=v[j];}
                if product>best {best=product;}
            }}}best},
        12 => {let mut n=1u64;loop {let tri=n*(n+1)/2;
            if divisor_count(tri)>500 {break tri as i64;}n+=1;
        }},
        13 => {let mut total=BigUint::zero();let mut lines=0;
            for line in include_str!("../../../lsum").lines() {let number=line.trim();if number.is_empty(){continue;}
                total+=number.parse::<BigUint>().unwrap();lines+=1;
            }assert_eq!(lines,100);total.to_string()[..10].parse::<i64>().unwrap()},
        14 => {let mut best_len=0;let mut best_start=0;
            for start in 1u64..1_000_000 {let (mut v,mut length)=(start,1);
                while v!=1 {v=if v%2==0 {v/2}else{3*v+1};length+=1;}
                if length>best_len {best_len=length;best_start=start;}
            }best_start as i64},
        16 => digit_sum(&BigUint::from(2u8).pow(1000)),
        17 => (1usize..=1000).map(letters).map(i64::from).sum(),
        19 => {let mut day=1;let mut count=0;let days=[31,28,31,30,31,30,31,31,30,31,30,31];
            for year in 1900..=2000 {for month in 0..12 {if year>=1901&&day==0 {count+=1;}
                day=(day+days[month]+i32::from(month==1&&leap(year)))%7;
            }}count},
        20 => {let mut fact=BigUint::one();for n in 2u32..=100 {fact*=n;}digit_sum(&fact)},
        21 => (2u32..10000).filter(|&a|{let b=divisor_sum(a);b!=a&&b>0&&divisor_sum(b)==a}).map(i64::from).sum(),
        22 => {let mut names:Vec<&str>=include_str!("../../../names.txt").split(',')
                .map(|s|s.trim().trim_matches('"')).collect();
            assert_eq!(names.len(),5163);names.sort_unstable();
            names.iter().enumerate().map(|(i,name)|{
                let score:i64=name.bytes().filter(|c|c.is_ascii_uppercase())
                    .map(|c|i64::from(c-b'A'+1)).sum();score*(i as i64+1)
            }).sum()},
        25 => {let mut a=BigUint::one();let mut b=BigUint::one();let limit=BigUint::from(10u8).pow(999);
            let mut index=2;while b<limit {let next=&a+&b;a=b;b=next;index+=1;}index},
        27 => {let mut longest=0;let mut product=0;
            for a in -999i64..1000 {for b in -999i64..1000 {
                let mut len=0;loop {let candidate=len*len+a*len+b;
                    if candidate<2||!prime(candidate as u64){break;}len+=1;
                }if len>longest {longest=len;product=a*b;}
            }}product},
        28 => {let mut sum=1i64;for side in (3i64..=1001).step_by(2) {
            sum+=4*side*side-6*(side-1);
        }sum},
        29 => {let mut values=HashSet::new();for a in 2u32..=100 {for b in 2..=100 {
            values.insert(BigUint::from(a).pow(b));
        }}values.len() as i64},
        30 => {let p5: [u32;10]=std::array::from_fn(|v|(v as u32).pow(5));
            (2u32..=354294).filter(|&n|digit_power_sum(n,&p5)==n).map(i64::from).sum()},
        34 => {let table:[u32;10]=std::array::from_fn(|v|factorial(v as u32));
            (3u32..=2_540_160).filter(|&n|digit_power_sum(n,&table)==n).map(i64::from).sum()},
        35 => {let p=sieve(1_000_000);let mut count=0;
            for n in 2u32..1_000_000 {if !p[n as usize] {continue;}
                let mut magnitude=1u32;while magnitude<=n/10{magnitude*=10;}
                let mut rotated=n;let mut good=true;
                loop {rotated=rotated/10+(rotated%10)*magnitude;
                    if !p[rotated as usize] {good=false;break;}
                    if rotated==n {break;}
                }if good {count+=1;}
            }count},
        39 => {let mut best_p=0;let mut best_count=0;
            for p in 12i64..=1000 {let mut count=0;
                for a in 1..p/3 {for b in a+1..(p-a)/2 {
                    let c=p-a-b;if a*a+b*b==c*c {count+=1;}
                }}if count>best_count {best_count=count;best_p=p;}
            }best_p},
        40 => {let positions=[1,10,100,1000,10000,100000,1_000_000];
            let mut at=0;let mut digit=0;let mut product=1i64;
            for n in 1.. {for c in n.to_string().bytes() {
                digit+=1;
                if digit==positions[at] {product*=i64::from(c-b'0');at+=1;
                    if at==positions.len(){break;}
                }
            }if at==positions.len(){break;}}
            product},
        44 => {let mut best=None;for j in 2u64..5000 {
            let pj=j*(3*j-1)/2;
            for k in (1..j).rev() {
                let pk=k*(3*k-1)/2;let diff=pj-pk;
                if best.is_some_and(|current|diff>=current) {break;}
                if pentagonal(diff)&&pentagonal(pj+pk){best=Some(diff);}
            }
            if let Some(current)=best {if 3*j-2>current {break;}}
        }best.unwrap_or(0) as i64},
        46 => {let mut n=9;loop {
            if !prime(n) {let mut representation=false;
                for p in 2..n {if !prime(p) {continue;}
                    if (n-p)%2!=0 {continue;}
                    let half=(n-p)/2;let r=(half as f64).sqrt() as u64;
                    if r*r==half {representation=true;break;}
                }if !representation {break n as i64;}
            }n+=2;
        }},
        47 => {let mut consecutive=0;let mut answer=0;
            for n in 2u32..1_000_000 {if distinct_factors(n)==4 {
                consecutive+=1;if consecutive==4 {answer=n-3;break;}
            }else{consecutive=0;}}i64::from(answer)},
        48 => {let modulus=BigUint::from(10u64).pow(10);let mut total=BigUint::zero();
            for n in 1u32..=1000 {
                let b=BigUint::from(n);total=(total+b.modpow(&b,&modulus))%&modulus;
            }total.to_string().parse::<i64>().unwrap()},
        50 => {let p=sieve(1_000_000);let mut prefix=vec![0u64];
            for (n,&is_prime) in p.iter().enumerate().skip(2).take(999998) {
                if is_prime {prefix.push(*prefix.last().unwrap()+n as u64);}
            }
            let (mut length,mut best)=(0usize,0i64);
            for i in 0..prefix.len()-1 {
                for j in (i+length+1)..prefix.len() {
                    let sum=prefix[j]-prefix[i];if sum>=1_000_000 {break;}
                    if p[sum as usize] {length=j-i;best=sum as i64;}
                }
            }best},
        55 => {let mut count=0;
            for start in 1u32..10000 {let mut n=BigUint::from(start);let mut palindrome=false;
                for _ in 0..50 {let reverse:String=n.to_string().chars().rev().collect();
                    n+=reverse.parse::<BigUint>().unwrap();
                    let s=n.to_string();if s.bytes().eq(s.bytes().rev()) {palindrome=true;break;}
                }if !palindrome {count+=1;}
            }count},
        69 => {let mut product=1i64;for p in 2i64.. {
            if prime(p as u64) {if product>1_000_000/p {break;}product*=p;}
        }product},
        92 => {let mut cache=[0u8;568];cache[1]=1;cache[89]=89;
            let squares=[0u32,1,4,9,16,25,36,49,64,81];let mut count=0;
            for n in 1u32..10_000_000 {let mut v=digit_power_sum(n,&squares);
                while cache[v as usize]==0 {v=digit_power_sum(v,&squares);}
                if cache[v as usize]==89 {count+=1;}
            }count},
        _ => return extra::solve(n),
    })
}

#[cfg(test)]
mod tests {
    use super::*;
    #[test] fn prime_edges(){assert!(!prime(0));assert!(!prime(1));assert!(prime(2));assert!(prime(97));assert!(!prime(49));}
    #[test] fn numeral_edges(){assert_eq!(letters(342),23);assert_eq!(letters(115),20);assert_eq!(letters(1000),11);}
    #[test] fn divisors(){assert_eq!(divisor_count(28),6);assert_eq!(divisor_sum(220),284);assert_eq!(divisor_sum(284),220);}
    #[test] fn small_problems(){assert_eq!(solve(1),Some(233168));assert_eq!(solve(2),Some(4613732));assert_eq!(solve(3),Some(6857));}
}
