//! Enumerate the four rational equations and sum unique x+y+z values exactly.
use num_bigint::BigUint;
use num_traits::ToPrimitive;
use project_euler::extra_helpers::{gcd, is_square};
use std::collections::HashSet;
#[derive(Clone, Copy, Hash, Eq, PartialEq)]
struct F(u64, u64);
fn f(n: u64, d: u64) -> F {
    let g = gcd(n, d);
    F(n / g, d / g)
}
fn valid(x: F) -> bool {
    x.0 > 0 && x.0 < x.1 && x.1 <= 35
}
fn add(x: F, y: F, z: F) -> F {
    f(
        x.0 * y.1 * z.1 + y.0 * x.1 * z.1 + z.0 * x.1 * y.1,
        x.1 * y.1 * z.1,
    )
}
fn bgcd(mut a: BigUint, mut b: BigUint) -> BigUint {
    while b != BigUint::from(0u32) {
        let r = &a % &b;
        a = b;
        b = r
    }
    a
}
fn solve() -> u64 {
    let mut v = Vec::new();
    for d in 2..=35 {
        for n in 1..d {
            if gcd(n, d) == 1 {
                v.push(F(n, d))
            }
        }
    }
    let mut sums = HashSet::new();
    for i in 0..v.len() {
        for j in i..v.len() {
            let (x, y) = (v[i], v[j]);
            for z in [
                f(x.0 * y.1 + y.0 * x.1, x.1 * y.1),
                f(x.0 * y.0, x.0 * y.1 + y.0 * x.1),
            ] {
                if valid(z) {
                    sums.insert(add(x, y, z));
                }
            }
            let sq = x.0 * x.0 * y.1 * y.1 + y.0 * y.0 * x.1 * x.1;
            if is_square(sq) {
                let r = (sq as f64).sqrt() as u64;
                for z in [f(r, x.1 * y.1), f(x.0 * y.0, r)] {
                    if valid(z) {
                        sums.insert(add(x, y, z));
                    }
                }
            }
        }
    }
    let (mut n, mut d) = (BigUint::from(0u32), BigUint::from(1u32));
    for F(a, b) in sums {
        n = n * b + a * &d;
        d *= b;
        let g = bgcd(n.clone(), d.clone());
        n /= &g;
        d /= g
    }
    (&n + &d).to_u64().unwrap()
}
fn main() {
    println!("{}", solve());
}
