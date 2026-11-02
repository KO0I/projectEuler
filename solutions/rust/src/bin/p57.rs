//! Project Euler 57: Square Root Convergents.
use num_bigint::BigUint;

fn solve() -> i64 {
    {
        let (mut top, mut bottom) = (BigUint::from(3u32), BigUint::from(2u32));
        let mut count = 0;
        for _ in 0..1000 {
            if top.to_string().len() > bottom.to_string().len() {
                count += 1;
            }
            let next = &top + &bottom * 2u32;
            bottom += &top;
            top = next;
        }
        count
    }
}

fn main() {
    println!("{}", solve());
}
