//! Project Euler 69: Totient Maximum.
use project_euler::extra_helpers::prime;

fn solve() -> i64 {
    {
        let mut product = 1i64;
        for p in 2i64.. {
            if prime(p as u64) {
                if product > 1_000_000 / p {
                    break;
                }
                product *= p;
            }
        }
        product
    }
}

fn main() {
    println!("{}", solve());
}
