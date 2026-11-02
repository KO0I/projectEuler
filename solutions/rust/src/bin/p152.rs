//! Exact meet-in-the-middle subset sums over a common arbitrary-precision denominator.
use num_bigint::BigUint;
use num_traits::{One, Zero};
fn gcd(mut a: BigUint, mut b: BigUint) -> BigUint {
    while !b.is_zero() {
        let r = &a % &b;
        a = b;
        b = r
    }
    a
}
fn solve() -> u64 {
    let c = [
        2u32, 3, 4, 5, 6, 7, 8, 9, 10, 12, 13, 14, 15, 18, 20, 21, 24, 28, 30, 35, 36, 39, 40, 42,
        45, 52, 56, 60, 63, 70, 72,
    ];
    let mut common = BigUint::one();
    for &n in &c {
        let q = BigUint::from(n * n);
        common = common.clone() / gcd(common.clone(), q.clone()) * q
    }
    let terms: Vec<_> = c.iter().map(|&n| &common / (n * n)).collect();
    let half = &common / 2u32;
    fn sums(terms: &[BigUint]) -> Vec<BigUint> {
        let mut v = vec![BigUint::zero(); 1 << terms.len()];
        for m in 1usize..v.len() {
            let bit = m.trailing_zeros() as usize;
            v[m] = &v[m & (m - 1)] + &terms[bit]
        }
        v
    }
    let left = sums(&terms[..15]);
    let mut right = sums(&terms[15..]);
    right.sort();
    let mut answer = 0;
    for x in left {
        if x > half {
            continue;
        }
        let need = &half - x;
        let lo = right.partition_point(|v| v < &need);
        let hi = right.partition_point(|v| v <= &need);
        answer += (hi - lo) as u64
    }
    answer
}
fn main() {
    println!("{}", solve());
}
