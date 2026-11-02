//! Project Euler 70: Totient Permutation.
use project_euler::extra_helpers::{decimal_signature, totients};

fn solve() -> i64 {
    {
        let phi = totients(10_000_000);
        let mut best_n = 0u64;
        let mut best_phi = 1u64;
        for n in 2usize..10_000_000 {
            let p = phi[n] as u64;
            if decimal_signature(n as u64) != decimal_signature(p) {
                continue;
            }
            if best_n == 0 || (n as u64) * best_phi < best_n * p {
                best_n = n as u64;
                best_phi = p;
            }
        }
        best_n as i64
    }
}

fn main() {
    println!("{}", solve());
}
