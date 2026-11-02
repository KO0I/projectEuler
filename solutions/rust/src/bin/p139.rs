//! Primitive Euclid triples whose hypotenuse is divisible by the leg difference.
use project_euler::extra_helpers::gcd;
fn solve() -> u64 {
    const L: u64 = 100_000_000;
    let mut count = 0;
    for m in 2u64.. {
        if 2 * m * (m + 1) >= L {
            break;
        }
        for n in 1..m {
            if (m - n) % 2 == 0 || gcd(m, n) != 1 {
                continue;
            }
            let (a, b, c) = (m * m - n * n, 2 * m * n, m * m + n * n);
            if c % a.abs_diff(b) == 0 {
                count += (L - 1) / (a + b + c)
            }
        }
    }
    count
}
fn main() {
    println!("{}", solve());
}
