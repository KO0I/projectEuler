//! Prime solutions are consecutive-cube differences 3n^2+3n+1.
use project_euler::extra_helpers::prime64;
fn solve() -> usize {
    (1u64..)
        .map(|n| 3 * n * n + 3 * n + 1)
        .take_while(|&p| p < 1_000_000)
        .filter(|&p| prime64(p))
        .count()
}
fn main() {
    println!("{}", solve());
}
