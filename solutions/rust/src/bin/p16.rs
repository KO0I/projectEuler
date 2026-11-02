//! Project Euler 16: Power Digit Sum.
use num_bigint::BigUint;
use project_euler::extra_helpers::digit_sum;

fn solve() -> i64 {
    digit_sum(&BigUint::from(2u8).pow(1000))
}

fn main() {
    println!("{}", solve());
}
