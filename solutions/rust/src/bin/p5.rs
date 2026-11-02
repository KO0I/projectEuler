//! Project Euler 5: Smallest Multiple.
use project_euler::extra_helpers::gcd;

fn solve() -> i64 {
    (2..=20u64).fold(1u64, |acc, v| acc / gcd(acc, v) * v) as i64
}

fn main() {
    println!("{}", solve());
}
