//! Repeated tenth powers test whether a prime can divide R(10^n).
use project_euler::extra_helpers::{mod_pow, prime64};
fn solve() -> u64 {
    (2u64..100_000)
        .filter(|&p| prime64(p))
        .filter(|&p| {
            let m = 9 * p;
            let mut r = 10 % m;
            for _ in 0..20 {
                r = mod_pow(r, 10, m)
            }
            r != 1
        })
        .sum()
}
fn main() {
    println!("{}", solve());
}
