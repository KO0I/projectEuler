// Project Euler 183: maximum product of integer parts.
// The continuous maximum of (N/k)^k lies at k=N/e, so only the two adjacent
// integers need comparison.  A reduced fraction terminates in base ten iff
// its denominator has no prime factors other than 2 and 5.
fn gcd(mut a: u32, mut b: u32) -> u32 {
    while b != 0 {
        (a, b) = (b, a % b);
    }
    a
}
fn solve() -> i64 {
    let mut answer = 0i64;
    for n in 5u32..=10_000 {
        let low = (n as f64 / std::f64::consts::E).floor() as u32;
        let high = low + 1;
        let score = |k: u32| k as f64 * (n as f64 / k as f64).ln();
        let k = if score(low) > score(high) { low } else { high };
        let mut denominator = k / gcd(n, k);
        while denominator % 2 == 0 {
            denominator /= 2;
        }
        while denominator % 5 == 0 {
            denominator /= 5;
        }
        answer += if denominator == 1 {
            -(n as i64)
        } else {
            n as i64
        };
    }
    answer
}
fn main() {
    println!("{}", solve());
}
