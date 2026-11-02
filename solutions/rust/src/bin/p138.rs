//! Pell recurrence for the equal-side lengths.
fn solve() -> u64 {
    let (mut a, mut b, mut sum) = (1u64, 17u64, 17u64);
    for _ in 1..12 {
        (a, b) = (b, 18 * b - a);
        sum += b
    }
    sum
}
fn main() {
    println!("{}", solve());
}
