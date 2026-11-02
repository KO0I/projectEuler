//! Project Euler 56: Powerful Digit Sum.
use num_bigint::BigUint;
use num_traits::One;
use project_euler::extra_helpers::digits_sum_big;

fn solve() -> i64 {
    {
        let mut best = 0;
        for a in 1u32..100 {
            let mut power = BigUint::one();
            for _ in 1..100 {
                power *= a;
                best = best.max(digits_sum_big(&power));
            }
        }
        best as i64
    }
}

fn main() {
    println!("{}", solve());
}
