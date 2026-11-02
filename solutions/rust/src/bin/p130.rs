//! Composite n qualify exactly when A(n) divides n-1.
use project_euler::extra_helpers::prime64;
fn a(n: u64) -> u64 {
    let (mut r, mut k) = (1 % n, 1);
    while r != 0 {
        r = (10 * r + 1) % n;
        k += 1
    }
    k
}
fn solve() -> u64 {
    let (mut found, mut sum) = (0, 0);
    for n in (3u64..).step_by(2) {
        if n % 5 != 0 && !prime64(n) && (n - 1) % a(n) == 0 {
            sum += n;
            found += 1;
            if found == 25 {
                break;
            }
        }
    }
    sum
}
fn main() {
    println!("{}", solve());
}
