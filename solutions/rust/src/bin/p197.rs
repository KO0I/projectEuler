// Project Euler 197: a recursively defined sequence converging to a 2-cycle.
fn next(u: f64) -> f64 {
    (2f64.powf(30.403243784 - u * u)).floor() * 1e-9
}
fn solve() -> f64 {
    let mut u = -1.0;
    for _ in 0..2000 {
        u = next(u);
    }
    u + next(u)
}
fn main() {
    println!("{:.9}", solve());
}
