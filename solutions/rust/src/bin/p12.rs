//! Project Euler 12: Highly Divisible Triangular Number.
use project_euler::extra_helpers::divisor_count;

fn solve() -> i64 {
    {
        let mut n = 1u64;
        loop {
            let tri = n * (n + 1) / 2;
            if divisor_count(tri) > 500 {
                break tri as i64;
            }
            n += 1;
        }
    }
}

fn main() {
    println!("{}", solve());
}
