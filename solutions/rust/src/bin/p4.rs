//! Project Euler 4: Largest Palindrome Product.
use project_euler::extra_helpers::is_palindrome;

fn solve() -> i64 {
    {
        let mut best = 0u32;
        for a in 100..=999 {
            for b in a..=999 {
                let prod = a * b;
                if prod > best && is_palindrome(prod) {
                    best = prod;
                }
            }
        }
        i64::from(best)
    }
}

fn main() {
    println!("{}", solve());
}
