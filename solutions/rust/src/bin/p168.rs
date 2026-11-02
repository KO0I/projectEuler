//! Solve the rotation equation exactly with arbitrary-precision integers.
use num_bigint::BigUint;
use num_traits::{ToPrimitive, Zero};
fn solve() -> u64 {
    let mut power = BigUint::from(10u32);
    let mut answer = 0;
    for _digits in 2..=100 {
        let lower = &power / 10u32;
        for m in 1u32..=9 {
            let den = 10 * m - 1;
            for last in 1u32..=9 {
                let num = &power * last - m * last;
                let q = &num / den;
                if &num % den != BigUint::zero() || q < lower || q >= power {
                    continue;
                }
                let n = q * 10u32 + last;
                answer = (answer + (&n % 100_000u32).to_u64().unwrap()) % 100_000
            }
        }
        power *= 10u32
    }
    answer
}
fn main() {
    println!("{:05}", solve());
}
