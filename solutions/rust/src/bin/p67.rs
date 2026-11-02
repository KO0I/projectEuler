//! Project Euler 67: Maximum Path Sum II.
use project_euler::extra_helpers::{matrix, triangle_max};

fn solve() -> i64 {
    triangle_max(matrix(include_str!(
        "../../../../tests/data/p067_triangle.txt"
    ))) as i64
}

fn main() {
    println!("{}", solve());
}
