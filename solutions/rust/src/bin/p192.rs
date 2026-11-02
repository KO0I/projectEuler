// Project Euler 192: best rational approximations.
//
// Continued fractions leave only the final convergent and the furthest legal
// semiconvergent as candidates.  To compare them without floating error,
// order the fractions and compare sqrt(n) with their arithmetic midpoint;
// squaring that positive comparison is exact with BigUint.
use num_bigint::BigUint;
fn closer(n: u64, p1: u64, q1: u64, p2: u64, q2: u64) -> bool {
    if p1 * q2 == p2 * q1 {
        return false;
    }
    let first_lower = p1 * q2 < p2 * q1;
    let midpoint_num = BigUint::from(p1) * q2 + BigUint::from(p2) * q1;
    let midpoint_den = BigUint::from(2u8) * q1 * q2;
    let root_below_mid =
        BigUint::from(n) * &midpoint_den * &midpoint_den < &midpoint_num * &midpoint_num;
    // The lower fraction is nearer exactly when the root is below midpoint.
    if first_lower {
        root_below_mid
    } else {
        !root_below_mid
    }
}
fn best_denominator(n: u64) -> u64 {
    const Q: u64 = 1_000_000_000_000;
    let a0 = (n as f64).sqrt() as u64;
    let (mut m, mut d, mut a) = (0u64, 1u64, a0);
    let (mut p0, mut q0, mut p1, mut q1) = (0u64, 1u64, 1u64, 0u64);
    loop {
        let p2 = a * p1 + p0;
        let q2 = a * q1 + q0;
        if q2 > Q {
            let k = (Q - q0) / q1;
            let (sp, sq) = (p0 + k * p1, q0 + k * q1);
            return if closer(n, p1, q1, sp, sq) { q1 } else { sq };
        }
        (p0, q0, p1, q1) = (p1, q1, p2, q2);
        m = d * a - m;
        d = (n - m * m) / d;
        a = (a0 + m) / d;
    }
}
fn solve() -> u64 {
    (2..=100_000u64)
        .filter(|&n| {
            let r = (n as f64).sqrt() as u64;
            r * r != n
        })
        .map(best_denominator)
        .sum()
}
fn main() {
    println!("{}", solve());
}
