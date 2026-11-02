//! Project Euler 108: Diophantine Reciprocals I.
use project_euler::extra_helpers::least_reciprocal_denominator;

fn solve() -> i64 {
    least_reciprocal_denominator(1000)
}

fn main() {
    println!("{}", solve());
}
