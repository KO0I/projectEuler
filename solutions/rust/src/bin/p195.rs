// Project Euler 195: 60-degree triangles with bounded inradius.
// Coprime Eisenstein parameters m>2n identify primitive triangles.  Integer
// correction loops around the floating estimate make each scale count exact.
fn gcd(mut a: i64, mut b: i64) -> i64 {
    while b != 0 {
        (a, b) = (b, a % b);
    }
    a
}
fn scale_count(c: i64, q: i64) -> i64 {
    let mut k = (c as f64 / (q as f64 * 3f64.sqrt())) as i64;
    let c2 = c as i128 * c as i128;
    while 3i128 * q as i128 * q as i128 * (k + 1) as i128 * (k + 1) as i128 <= c2 {
        k += 1;
    }
    while 3i128 * q as i128 * q as i128 * k as i128 * k as i128 > c2 {
        k -= 1;
    }
    k
}
fn solve() -> u64 {
    let radius = 1_053_779i64;
    let max_q = (2.0 * 3f64.sqrt() * radius as f64) as i64;
    let mut total = 0u64;
    let mut n = 1;
    while n * n < max_q {
        for m in 2 * n + 1..=n + max_q / n {
            if gcd(m, n) != 1 {
                continue;
            }
            let q = n * (m - n);
            let c = if (m + n) % 3 == 0 {
                6 * radius
            } else {
                2 * radius
            };
            total += scale_count(c, q) as u64;
        }
        n += 1;
    }
    total
}
fn main() {
    println!("{}", solve());
}
