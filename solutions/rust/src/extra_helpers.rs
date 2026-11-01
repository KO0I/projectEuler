//! Reusable exact arithmetic, combinatorics, and input primitives for Euler 1–120.
//! The solver modules share these algorithms instead of copying a prime test or
//! dataset parser into every executable.
use num_bigint::BigUint;

pub fn gcd(mut a: u64, mut b: u64) -> u64 {
    while b != 0 { (a, b) = (b, a % b); } a
}
pub fn isqrt(n: u64) -> u64 {
    let mut lo = 0u64;
    let mut hi = n.min(u32::MAX as u64) + 1;
    while hi - lo > 1 {
        let mid = lo + (hi - lo) / 2;
        if mid <= n / mid { lo = mid; } else { hi = mid; }
    }
    lo
}
pub fn prime(n: u64) -> bool {
    if n < 2 { return false; }
    if n % 2 == 0 { return n == 2; }
    let mut d = 3;
    while d <= n / d {
        if n % d == 0 { return false; }
        d += 2;
    }
    true
}
pub fn sieve(limit: usize) -> Vec<bool> {
    let mut out = vec![true; limit + 1];
    out[0] = false;
    if limit >= 1 { out[1] = false; }
    for p in 2..=limit {
        if p > limit / p { break; }
        if out[p] {
            for j in (p * p..=limit).step_by(p) { out[j] = false; }
        }
    }
    out
}
pub fn primes(limit: usize) -> Vec<u64> {
    sieve(limit).into_iter().enumerate().filter_map(|(i, x)| x.then_some(i as u64)).collect()
}
pub fn decimal_signature(mut n: u64) -> u64 {
    let mut bits = 0;
    loop { bits += 1u64 << (5 * (n % 10)); n /= 10; if n == 0 { break; } }
    bits
}
pub fn palindrome(mut n: u64, base: u64) -> bool {
    let original = n;
    let mut reversed = 0;
    loop { reversed = base * reversed + n % base; n /= base; if n == 0 { break; } }
    reversed == original
}
pub fn digits(mut n: u64) -> Vec<u8> {
    let mut out = Vec::new();
    loop { out.push((n % 10) as u8); n /= 10; if n == 0 { break; } }
    out.reverse(); out
}
pub fn digits_sum_big(n: &BigUint) -> usize {
    n.to_string().bytes().map(|c| (c - b'0') as usize).sum()
}
pub fn parse_numbers(data: &str) -> Vec<u64> {
    data.split(|c: char| !c.is_ascii_digit())
        .filter(|s| !s.is_empty()).map(|s| s.parse().expect("integer input")).collect()
}
pub fn matrix(data: &str) -> Vec<Vec<u64>> {
    data.lines().filter(|s| !s.trim().is_empty()).map(parse_numbers).collect()
}
pub fn triangle_max(mut rows: Vec<Vec<u64>>) -> u64 {
    for row in (0..rows.len() - 1).rev() {
        for col in 0..rows[row].len() {
            rows[row][col] += rows[row+1][col].max(rows[row+1][col+1]);
        }
    }
    rows[0][0]
}
pub fn proper_divisor_sums(limit: usize) -> Vec<usize> {
    let mut result = vec![0; limit + 1];
    for d in 1..=limit / 2 {
        for n in (2*d..=limit).step_by(d) { result[n] += d; }
    }
    result
}
pub fn totients(limit: usize) -> Vec<u32> {
    let mut phi: Vec<u32> = (0..=limit).map(|n| n as u32).collect();
    for p in 2..=limit {
        if phi[p] == p as u32 {
            for n in (p..=limit).step_by(p) { phi[n] -= phi[n] / p as u32; }
        }
    }
    phi
}
pub fn is_square(n: u64) -> bool { let root = isqrt(n); root*root == n }
pub fn pandigital_1_to_9(mut n: u64) -> bool {
    let mut mask=0u16;
    for _ in 0..9 { let d=n%10; if d==0 || mask & (1<<d)!=0 { return false; } mask |= 1<<d; n/=10; }
    n==0 && mask==0x3fe
}
pub fn next_permutation<T: Ord>(items: &mut [T]) -> bool {
    if items.len()<2 {return false;}
    let Some(k)=(0..items.len()-1).rev().find(|&i|items[i]<items[i+1]) else {return false;};
    let j=(k+1..items.len()).rev().find(|&j|items[k]<items[j]).unwrap();
    items.swap(k,j); items[k+1..].reverse(); true
}
pub fn choose(n: u64, k: u64) -> u128 {
    let k = k.min(n-k);
    (1..=k).fold(1u128, |v,i| v * (n-k+i) as u128 / i as u128)
}
pub fn mod_pow(mut a: u64, mut e: u64, m: u64) -> u64 {
    let mut v=1u64;
    while e>0 {if e&1!=0 {v=((v as u128*a as u128)%m as u128) as u64;}
        a=((a as u128*a as u128)%m as u128) as u64; e>>=1; }
    v
}
#[cfg(test)] mod tests {
    use super::*;
    #[test] fn arithmetic() {
        assert_eq!(gcd(220,284),4); assert_eq!(isqrt(u64::MAX),u32::MAX as u64);
        assert!(prime(104729)); assert!(!prime(1));
        assert_eq!(proper_divisor_sums(300)[220],284);
        assert_eq!(totients(10)[9],6);
        assert_eq!(choose(40,20),137846528820);
        assert_eq!(mod_pow(2,10,1000),24);
        let mut values=[1,2,3]; assert!(next_permutation(&mut values)); assert_eq!(values,[1,3,2]);
    }
}

/// Floor of sqrt(n) with arbitrary precision, using descending Newton iterates.
pub fn big_isqrt(n: &BigUint) -> BigUint {
    if *n < BigUint::from(2u32) { return n.clone(); }
    let mut x = BigUint::from(1u32) << ((n.bits() as usize + 1) / 2);
    loop {
        let next = (&x + n / &x) >> 1usize;
        if next >= x { return x; }
        x = next;
    }
}
/// Fibonacci doubling identities avoid iterating over every intermediate
/// bignum. Returns (F(n), F(n+1)).
pub fn fibonacci_pair(n: u64) -> (BigUint, BigUint) {
    if n == 0 {return (BigUint::from(0u32),BigUint::from(1u32));}
    let (a,b) = fibonacci_pair(n/2);
    let c=&a*((&b<<1usize)-&a);
    let d=&a*&a+&b*&b;
    if n&1==0 {(c,d)} else {(d.clone(),c+d)}
}

/// Deterministic strong probable-prime test for the entire u64 domain.
/// The seven specified witnesses are sufficient for 0 <= n < 2^64.
pub fn prime64(n:u64)->bool {
    if n<2 {return false;}
    for p in [2,3,5,7,11,13,17,19,23,29,31,37] {if n%p==0 {return n==p;}}
    let s=(n-1).trailing_zeros();let d=(n-1)>>s;
    for a in [2,325,9375,28178,450775,9780504,1795265022] {
        let a=a%n;if a==0 {continue;}
        let mut v=mod_pow(a,d,n);
        if v==1||v==n-1 {continue;}
        let mut passed=false;
        for _ in 1..s {v=((v as u128*v as u128)%n as u128) as u64;
            if v==n-1 {passed=true;break;}
        }
        if !passed {return false;}
    }true
}
