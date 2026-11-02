//! Project Euler 18: Maximum Path Sum I.
use project_euler::extra_helpers::{matrix, triangle_max};

fn solve() -> i64 {
    triangle_max(matrix(include_str!(
        "../../../../tests/data/p018_triangle.txt"
    ))) as i64
}

fn main() {
    println!("{}", solve());
}
