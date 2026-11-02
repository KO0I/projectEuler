//! Compute A(n) by propagating a repunit remainder.
fn a(n: u64) -> u64 {
    let (mut r, mut k) = (1 % n, 1);
    while r != 0 {
        r = (10 * r + 1) % n;
        k += 1
    }
    k
}
fn solve() -> u64 {
    (1_000_001u64..)
        .step_by(2)
        .find(|&n| n % 5 != 0 && a(n) > 1_000_000)
        .unwrap()
}
fn main() {
    println!("{}", solve());
}
