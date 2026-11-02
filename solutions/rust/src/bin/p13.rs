//! Project Euler 13: Large Sum.
use num_bigint::BigUint;
use num_traits::Zero;

fn solve() -> i64 {
    {
        let mut total = BigUint::zero();
        let mut lines = 0;
        for line in include_str!("../../../../lsum").lines() {
            let number = line.trim();
            if number.is_empty() {
                continue;
            }
            total += number.parse::<BigUint>().unwrap();
            lines += 1;
        }
        assert_eq!(lines, 100);
        total.to_string()[..10].parse::<i64>().unwrap()
    }
}

fn main() {
    println!("{}", solve());
}
