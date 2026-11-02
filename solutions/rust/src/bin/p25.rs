//! Project Euler 25: 1000-digit Fibonacci Number.
use num_bigint::BigUint;
use num_traits::One;

fn solve() -> i64 {
    {
        let mut a = BigUint::one();
        let mut b = BigUint::one();
        let limit = BigUint::from(10u8).pow(999);
        let mut index = 2;
        while b < limit {
            let next = &a + &b;
            a = b;
            b = next;
            index += 1;
        }
        index
    }
}

fn main() {
    println!("{}", solve());
}
