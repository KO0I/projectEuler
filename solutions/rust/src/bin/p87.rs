//! Project Euler 87: Prime Power Triples.
use project_euler::extra_helpers::{isqrt, primes};

fn solve() -> i64 {
    {
        let limit = 50_000_000u64;
        let roots = primes(isqrt(limit) as usize);
        let mut seen = vec![false; limit as usize];
        for &p in &roots {
            let a = p * p;
            if a >= limit {
                break;
            }
            for &q in &roots {
                let b = q * q * q;
                if a + b >= limit {
                    break;
                }
                for &r in &roots {
                    let c = r * r * r * r;
                    if a + b + c >= limit {
                        break;
                    }
                    seen[(a + b + c) as usize] = true;
                }
            }
        }
        seen.into_iter().filter(|&b| b).count() as i64
    }
}

fn main() {
    println!("{}", solve());
}
