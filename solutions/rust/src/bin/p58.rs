//! Project Euler 58: Spiral Primes.
use project_euler::extra_helpers::prime;

fn solve() -> i64 {
    {
        let mut diagonal_primes = 0;
        for side in (3u64..).step_by(2) {
            let square = side * side;
            for corner in 1..=3 {
                if prime(square - corner * (side - 1)) {
                    diagonal_primes += 1;
                }
            }
            if 10 * diagonal_primes < 2 * side - 1 {
                return side as i64;
            }
        }
        unreachable!("unbounded spiral search")
    }
}

fn main() {
    println!("{}", solve());
}
