// Project Euler 255: rounded square roots.
// Partition input ranges by constant ceil(n/x), hence constant next iterate.
// Recursing on aggregate intervals counts 90 trillion inputs exactly.
fn total(lo: u64, hi: u64, x: u64, done: u64) -> u64 {
    let first = lo.div_ceil(x);
    let last = hi.div_ceil(x);
    let (first_y, last_y) = ((x + first) / 2, (x + last) / 2);
    let mut answer = 0;
    for y in first_y..=last_y {
        let mut qlo = 2 * y - x;
        let mut qhi = qlo + 1;
        qlo = qlo.max(first);
        qhi = qhi.min(last);
        let child_lo = ((qlo - 1) * x + 1).max(lo);
        let child_hi = (qhi * x).min(hi);
        if y == x {
            answer += (child_hi - child_lo + 1) * (done + 1)
        } else {
            answer += total(child_lo, child_hi, y, done + 1);
        }
    }
    answer
}
fn initial(digits: u32) -> u64 {
    if digits % 2 == 1 {
        2 * 10u64.pow((digits - 1) / 2)
    } else {
        7 * 10u64.pow((digits - 2) / 2)
    }
}
fn solve() -> f64 {
    assert_eq!(total(10_000, 99_999, initial(5), 0), 288_926);
    let (lo, hi) = (10_000_000_000_000, 99_999_999_999_999);
    total(lo, hi, initial(14), 0) as f64 / (hi - lo + 1) as f64
}
fn main() {
    println!("{:.10}", solve());
}
