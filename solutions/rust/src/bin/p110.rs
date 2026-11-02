//! Project Euler 110: Diophantine Reciprocals II.
use project_euler::extra_helpers::least_reciprocal_denominator;

fn solve() -> i64 {
    least_reciprocal_denominator(4_000_000)
}

fn main() {
    println!("{}", solve());
}
