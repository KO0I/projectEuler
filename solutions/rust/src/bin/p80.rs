//! Project Euler 80: Square Root Digital Expansion.
use num_bigint::BigUint;
use project_euler::extra_helpers::{big_isqrt, is_square};

fn solve() -> i64 {
    {
        let scale = BigUint::from(10u32).pow(198);
        let mut sum = 0usize;
        for n in 1..=100u64 {
            if is_square(n) {
                continue;
            }
            let root = big_isqrt(&(BigUint::from(n) * &scale));
            let text = root.to_string();
            assert_eq!(text.len(), 100);
            sum += text.bytes().map(|b| (b - b'0') as usize).sum::<usize>();
        }
        sum as i64
    }
}

fn main() {
    println!("{}", solve());
}
