//! Project Euler 36: Double-base Palindromes.
use project_euler::extra_helpers::palindrome;

fn solve() -> i64 {
    (1u64..1_000_000)
        .step_by(2)
        .filter(|&v| palindrome(v, 10) && palindrome(v, 2))
        .sum::<u64>() as i64
}

fn main() {
    println!("{}", solve());
}
