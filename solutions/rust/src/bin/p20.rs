//! Project Euler 20: Factorial Digit Sum.
use num_bigint::BigUint;
use num_traits::One;
use project_euler::extra_helpers::digit_sum;

fn solve() -> i64 {
    {
        let mut fact = BigUint::one();
        for n in 2u32..=100 {
            fact *= n;
        }
        digit_sum(&fact)
    }
}

fn main() {
    println!("{}", solve());
}
