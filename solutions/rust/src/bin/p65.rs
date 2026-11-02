//! Project Euler 65: Convergents of e.
use num_bigint::BigUint;
use num_traits::{One, Zero};
use project_euler::extra_helpers::digits_sum_big;

fn solve() -> i64 {
    {
        let (mut numerator, mut previous) = (BigUint::one(), BigUint::zero());
        for k in (0..100).rev() {
            let a = if k == 0 {
                2
            } else if k % 3 == 2 {
                2 * (k / 3 + 1)
            } else {
                1
            };
            let next = &numerator * (a as u32) + &previous;
            previous = numerator;
            numerator = next;
        }
        digits_sum_big(&numerator) as i64
    }
}

fn main() {
    println!("{}", solve());
}
