// Project Euler 210: obtuse-angled triangles.
// Two obtuse regions sum by diagonals.  Completing squares turns the third
// into an integer-circle count, evaluated with a monotone y pointer in O(k).
fn isqrt(n: u64) -> u64 {
    let mut r = (n as f64).sqrt() as u64;
    while (r + 1) <= n / (r + 1) {
        r += 1;
    }
    while r > n / r {
        r -= 1;
    }
    r
}
fn circle(k: u64) -> u64 {
    let limit = (k * k - 1) / 2;
    let radius = isqrt(limit);
    let diagonal = isqrt(limit / 2);
    let mut y = radius;
    let mut half = 0;
    for x in 1..=diagonal {
        while x * x + y * y > limit {
            y -= 1;
        }
        half += y - x + 1;
    }
    1 + 4 * radius + 4 * (2 * half - diagonal)
}
fn solve() -> u64 {
    let r = 1_000_000_000u64;
    let k = r / 4;
    let linear = r * (2 * r + 1) / 2 + r * (2 * r + 1) / 4;
    linear + circle(k) - (4 * k - 1)
}
fn main() {
    println!("{}", solve());
}
