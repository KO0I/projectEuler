//! p divides R(k) iff 10^k is one modulo 9p.
use project_euler::extra_helpers::{mod_pow, prime64};
fn solve() -> u64 {
    (2u64..)
        .filter(|&p| p != 2 && p != 5 && prime64(p) && mod_pow(10, 1_000_000_000, 9 * p) == 1)
        .take(40)
        .sum()
}
fn main() {
    println!("{}", solve());
}
