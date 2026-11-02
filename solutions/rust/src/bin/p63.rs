//! Project Euler 63: Powerful Digit Counts.
use num_bigint::BigUint;
use num_traits::One;

fn solve() -> i64 {
    {
        let mut count = 0;
        for base in 1u32..10 {
            let mut value = BigUint::one();
            for exponent in 1..=22 {
                value *= base;
                let length = value.to_string().len();
                if length == exponent {
                    count += 1;
                }
                if length < exponent {
                    break;
                }
            }
        }
        count
    }
}

fn main() {
    println!("{}", solve());
}
