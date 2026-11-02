//! Binary digit DP with coefficients 0,1,2 and a one-bit carry.
use num_bigint::BigUint;
use num_traits::{One, Zero};
fn solve() -> u64 {
    let mut n = BigUint::from(10u32).pow(25);
    let mut dp = [1u64, 0];
    while !n.is_zero() {
        let bit = (&n & BigUint::one()) == BigUint::one();
        let mut next = [0, 0];
        for carry in 0..2 {
            for digit in 0..=2 {
                if ((digit + carry) & 1 == 1) == bit {
                    next[(digit + carry) >> 1] += dp[carry]
                }
            }
        }
        dp = next;
        n >>= 1usize
    }
    dp[0]
}
fn main() {
    println!("{}", solve());
}
