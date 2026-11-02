//! Monotone interval pruning finds every fixed point of the digit-count function.
fn occ(n: u64, d: u64) -> u64 {
    let (mut count, mut p) = (0, 1);
    while p <= n {
        let (lower, current, higher) = (n % p, (n / p) % 10, n / p / 10);
        count += higher * p;
        if current > d {
            count += p
        } else if current == d {
            count += lower + 1
        }
        if p > u64::MAX / 10 {
            break;
        }
        p *= 10
    }
    count
}
fn search(lo: u64, hi: u64, d: u64) -> u64 {
    let (a, b) = (occ(lo, d), occ(hi, d));
    if a > hi || b < lo {
        return 0;
    }
    if lo == hi {
        return if a == lo { lo } else { 0 };
    }
    let m = lo + (hi - lo) / 2;
    search(lo, m, d) + search(m + 1, hi, d)
}
fn solve() -> u64 {
    (1..=9).map(|d| search(0, 1_000_000_000_000, d)).sum()
}
fn main() {
    println!("{}", solve());
}
