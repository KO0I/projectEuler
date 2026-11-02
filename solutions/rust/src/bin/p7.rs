//! Project Euler 7: 10 001st Prime.
use project_euler::extra_helpers::prime;

fn solve() -> i64 {
    {
        let mut found = 0;
        let mut candidate = 1u64;
        while found < 10001 {
            candidate += 1;
            if prime(candidate) {
                found += 1;
            }
        }
        candidate as i64
    }
}

fn main() {
    println!("{}", solve());
}
