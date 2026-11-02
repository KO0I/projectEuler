//! Project Euler 10: Summation of Primes.
use project_euler::extra_helpers::sieve;

fn solve() -> i64 {
    {
        let p = sieve(2_000_000);
        (2..2_000_000).filter(|&v| p[v]).map(|v| v as i64).sum()
    }
}

fn main() {
    println!("{}", solve());
}
