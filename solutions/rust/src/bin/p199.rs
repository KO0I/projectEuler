// Project Euler 199: iterative circle packing.
// Signed curvatures and Descartes' theorem give each inserted circle.  Its
// insertion splits a gap into three independent child gaps.
fn fill(a: f64, b: f64, c: f64, depth: u8) -> f64 {
    if depth == 0 {
        return 0.0;
    }
    let d = a + b + c + 2.0 * (a * b + b * c + c * a).sqrt();
    1.0 / (d * d) + fill(a, b, d, depth - 1) + fill(b, c, d, depth - 1) + fill(c, a, d, depth - 1)
}
fn solve() -> f64 {
    let r = 2.0 * 3f64.sqrt() - 3.0;
    let k = 1.0 / r;
    1.0 - (3.0 * r * r + fill(k, k, k, 10) + 3.0 * fill(-1.0, k, k, 10))
}
fn main() {
    println!("{:.8}", solve());
}
