// Project Euler 226: a scoop of blancmange.
// Bisection locates the nontrivial curve intersection; a million-panel
// Simpson rule resolves the Takagi function's dyadic corners.
fn blanc(mut x: f64) -> f64 {
    let (mut sum, mut weight) = (0.0, 1.0);
    for _ in 0..53 {
        sum += (x - x.round()).abs() * weight;
        x *= 2.0;
        weight *= 0.5;
    }
    sum
}
fn diff(x: f64) -> f64 {
    blanc(x) - (0.5 - (0.0625 - (x - 0.25).powi(2)).sqrt())
}
fn solve() -> f64 {
    let (mut low, mut high) = (0.0, 0.25);
    for _ in 0..100 {
        let mid = (low + high) / 2.0;
        if diff(mid) > 0.0 {
            high = mid
        } else {
            low = mid
        }
    }
    let from = (low + high) / 2.0;
    let step = (0.5 - from) / 1_000_000.0;
    let mut sum = diff(from) + diff(0.5);
    for i in 1..1_000_000 {
        sum += if i & 1 == 1 { 4.0 } else { 2.0 } * diff(from + i as f64 * step);
    }
    sum * step / 3.0
}
fn main() {
    println!("{:.8}", solve());
}
