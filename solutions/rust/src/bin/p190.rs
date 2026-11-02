// Project Euler 190: maximising a weighted product.
// Lagrange multipliers give x_i=2i/(m+1) for the constrained maximum.
fn solve() -> u64 {
    (2..=15)
        .map(|m| {
            let p = (1..=m).fold(1f64, |p, i| p * (2.0 * i as f64 / (m + 1) as f64).powi(i));
            p.floor() as u64
        })
        .sum()
}
fn main() {
    println!("{}", solve());
}
