//! Project Euler 72: Counting Fractions.
use project_euler::extra_helpers::totients;

fn solve() -> i64 {
    {
        let p = totients(1_000_000);
        p[2..].iter().map(|&v| v as i64).sum()
    }
}

fn main() {
    println!("{}", solve());
}
