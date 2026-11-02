//! Project Euler 48: Self Powers.
use num_bigint::BigUint;
use num_traits::Zero;

fn solve() -> i64 {
    {
        let modulus = BigUint::from(10u64).pow(10);
        let mut total = BigUint::zero();
        for n in 1u32..=1000 {
            let b = BigUint::from(n);
            total = (total + b.modpow(&b, &modulus)) % &modulus;
        }
        total.to_string().parse::<i64>().unwrap()
    }
}

fn main() {
    println!("{}", solve());
}
