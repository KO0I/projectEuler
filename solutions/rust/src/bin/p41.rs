//! Project Euler 41: Pandigital Prime.
use project_euler::extra_helpers::{next_permutation, prime};

fn solve() -> i64 {
    {
        let mut best = 0u64;
        for len in [7usize, 4] {
            let mut p: Vec<u8> = (1..=len as u8).collect();
            loop {
                let n = p.iter().fold(0u64, |v, &d| v * 10 + d as u64);
                if prime(n) {
                    best = best.max(n);
                }
                if !next_permutation(&mut p) {
                    break;
                }
            }
            if best > 0 {
                break;
            }
        }
        best as i64
    }
}

fn main() {
    println!("{}", solve());
}
