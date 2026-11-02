//! For odd n the binomial remainder is 2*n*p_n modulo p_n^2.
use project_euler::extra_helpers::sieve;
fn solve() -> usize {
    sieve(300_000)
        .iter()
        .enumerate()
        .filter(|(_, p)| **p)
        .enumerate()
        .find(|(i, (p, _))| {
            let n = i + 1;
            n % 2 == 1 && 2u64 * n as u64 * (*p as u64) > 10_000_000_000
        })
        .unwrap()
        .0
        + 1
}
fn main() {
    println!("{}", solve());
}
