//! Project Euler 104: Pandigital Fibonacci Ends.
use project_euler::extra_helpers::{fibonacci_pair, pandigital_1_to_9};

fn solve() -> i64 {
    {
        let mut prev = 1u64;
        let mut curr = 1u64;
        let mut index = 2u64;
        loop {
            index += 1;
            let next = (prev + curr) % 1_000_000_000;
            prev = curr;
            curr = next;
            if pandigital_1_to_9(curr) {
                let text = fibonacci_pair(index).0.to_string();
                if text.len() >= 9 && pandigital_1_to_9(text[..9].parse().unwrap()) {
                    break index as i64;
                }
            }
        }
    }
}

fn main() {
    println!("{}", solve());
}
