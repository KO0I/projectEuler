//! Project Euler 29: Distinct Powers.
use num_bigint::BigUint;
use std::collections::HashSet;

fn solve() -> i64 {
    {
        let mut values = HashSet::new();
        for a in 2u32..=100 {
            for b in 2..=100 {
                values.insert(BigUint::from(a).pow(b));
            }
        }
        values.len() as i64
    }
}

fn main() {
    println!("{}", solve());
}
