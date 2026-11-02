//! Coprime parameters generate all progressive numbers below 10^12.
use project_euler::extra_helpers::{gcd, is_square};
fn solve() -> u64 {
    let mut found = Vec::new();
    for b in 2u64.. {
        let b3 = b * b * b;
        if b3 >= 1_000_000_000_000 {
            break;
        }
        for c in 1..b {
            if gcd(b, c) != 1 {
                continue;
            }
            for a in 1u64.. {
                let Some(n) = a.checked_mul(c).and_then(|x| x.checked_mul(a * b3 + c)) else {
                    break;
                };
                if n >= 1_000_000_000_000 {
                    break;
                }
                if is_square(n) {
                    found.push(n)
                }
            }
        }
    }
    found.sort_unstable();
    found.dedup();
    found.into_iter().sum()
}
fn main() {
    println!("{}", solve());
}
