// Project Euler 239: twenty-two foolish primes.
// Inclusion-exclusion terms are advanced as normalized factorial ratios,
// avoiding construction of 100! while retaining ample f64 precision.
fn solve() -> f64 {
    let mut term = 1.0 / (100.0 * 99.0 * 98.0);
    let mut sum = term;
    for j in 0..22 {
        term *= -(22 - j) as f64 / ((j + 1) as f64 * (97 - j) as f64);
        sum += term;
    }
    25.0 * 24.0 * 23.0 / 6.0 * sum
}
fn main() {
    println!("{:.12}", solve());
}
