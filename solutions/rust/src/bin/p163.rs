//! Closed form after classifying triangle orientations in an order-36 grid.
fn solve() -> i64 {
    let n = 36;
    let mut x = 1678 * n * n * n + 3117 * n * n + 88 * n;
    x -= 345 * (n % 2);
    x -= 320 * (n % 3);
    x -= 90 * (n % 4);
    x -= 288 * (n % 5);
    x / 240
}
fn main() {
    println!("{}", solve());
}
