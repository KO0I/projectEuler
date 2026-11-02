// Project Euler 234: semidivisible numbers.
// Between consecutive prime squares p²,q², inclusion-exclusion sums values
// divisible by exactly one of p and q as arithmetic progressions.
fn multiples(d: u64, low: u64, high: u64) -> u128 {
    let first = low.div_ceil(d);
    let last = high / d;
    if first > last {
        0
    } else {
        d as u128 * (first + last) as u128 * (last - first + 1) as u128 / 2
    }
}
fn solve() -> u64 {
    const LIM: u64 = 999_966_663_333;
    let n = 1_010_000;
    let mut comp = vec![false; n + 1];
    let mut ps = Vec::new();
    for p in 2..=n {
        if !comp[p] {
            ps.push(p as u64);
            if p <= n / p {
                for m in (p * p..=n).step_by(p) {
                    comp[m] = true;
                }
            }
        }
    }
    let mut total = 0u128;
    for w in ps.windows(2) {
        let (p, q) = (w[0], w[1]);
        let low = p * p + 1;
        if low > LIM {
            break;
        }
        let high = (q * q - 1).min(LIM);
        total +=
            multiples(p, low, high) + multiples(q, low, high) - 2 * multiples(p * q, low, high);
    }
    total as u64
}
fn main() {
    println!("{}", solve());
}
