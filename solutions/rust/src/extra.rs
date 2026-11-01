//! Additional native Rust solutions, split by algorithm for reuse and review.
mod easy;
mod middle;
mod hard;
pub fn solve(n: u32) -> Option<i64> {
    easy::solve(n).or_else(|| middle::solve(n)).or_else(|| hard::solve(n))
}
