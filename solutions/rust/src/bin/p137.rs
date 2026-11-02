//! Pell-derived recurrence for Fibonacci golden nuggets.
fn solve() -> u64 {
    let (mut a, mut b) = (2u64, 15u64);
    for _ in 2..15 {
        (a, b) = (b, 7 * b - a + 1)
    }
    b
}
fn main() {
    println!("{}", solve());
}
