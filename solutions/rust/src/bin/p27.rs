//! Project Euler 27: Quadratic Primes.
use project_euler::extra_helpers::prime;

fn solve() -> i64 {
    {
        let mut longest = 0;
        let mut product = 0;
        for a in -999i64..1000 {
            for b in -999i64..1000 {
                let mut len = 0;
                loop {
                    let candidate = len * len + a * len + b;
                    if candidate < 2 || !prime(candidate as u64) {
                        break;
                    }
                    len += 1;
                }
                if len > longest {
                    longest = len;
                    product = a * b;
                }
            }
        }
        product
    }
}

fn main() {
    println!("{}", solve());
}
