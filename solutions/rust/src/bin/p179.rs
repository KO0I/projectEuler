//! Divisor sieve followed by adjacent equality comparisons.
fn solve() -> usize {
    const L: usize = 10_000_000;
    let mut d = vec![0u16; L + 1];
    for x in 1..=L {
        for n in (x..=L).step_by(x) {
            d[n] += 1
        }
    }
    (2..L).filter(|&n| d[n] == d[n + 1]).count()
}
fn main() {
    println!("{}", solve());
}
