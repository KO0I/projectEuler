// Project Euler 235: arithmetic-geometric sequence.
// Bisection on the unique decreasing root; repeated multiplication evaluates
// powers without thousands of separate pow calls.
fn sum(r: f64) -> f64 {
    let (mut power, mut total) = (1.0, 0.0);
    for k in 1..=5000 {
        total += (900.0 - 3.0 * k as f64) * power;
        power *= r;
    }
    total
}
fn solve() -> f64 {
    let (mut low, mut high) = (1.0, 1.01);
    for _ in 0..200 {
        let mid = (low + high) / 2.0;
        if sum(mid) > -600_000_000_000.0 {
            low = mid
        } else {
            high = mid
        }
    }
    (low + high) / 2.0
}
fn main() {
    println!("{:.12}", solve());
}
