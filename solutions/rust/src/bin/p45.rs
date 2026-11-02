//! Project Euler 45: Triangular, Pentagonal, and Hexagonal.
use project_euler::extra_helpers::isqrt;

fn solve() -> i64 {
    {
        let mut n = 144u64;
        loop {
            let hex = n * (2 * n - 1);
            let discr = 24 * hex + 1;
            let root = isqrt(discr);
            if root * root == discr && (root + 1) % 6 == 0 {
                break hex as i64;
            }
            n += 1;
        }
    }
}

fn main() {
    println!("{}", solve());
}
