//! Project Euler 55: Lychrel Numbers.
use num_bigint::BigUint;

fn solve() -> i64 {
    {
        let mut count = 0;
        for start in 1u32..10000 {
            let mut n = BigUint::from(start);
            let mut palindrome = false;
            for _ in 0..50 {
                let reverse: String = n.to_string().chars().rev().collect();
                n += reverse.parse::<BigUint>().unwrap();
                let s = n.to_string();
                if s.bytes().eq(s.bytes().rev()) {
                    palindrome = true;
                    break;
                }
            }
            if !palindrome {
                count += 1;
            }
        }
        count
    }
}

fn main() {
    println!("{}", solve());
}
