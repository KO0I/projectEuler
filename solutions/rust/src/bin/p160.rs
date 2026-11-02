//! Remove factors of five, cancel matching twos, and retain guard digits.
fn solve() -> u64 {
    let mut limit = 1_000_000_000_000u64;
    while limit % 100_000 == 0 {
        limit /= 5
    }
    let mut r = 1;
    for v in 1..=limit {
        let mut x = v;
        while x % 5 == 0 {
            x /= 5;
            r /= 2
        }
        r = r * (x % 100_000) % 1_000_000_000
    }
    r % 100_000
}
fn main() {
    println!("{:05}", solve());
}
