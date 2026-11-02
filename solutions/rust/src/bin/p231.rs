// Project Euler 231: prime factors of a binomial coefficient.
// Legendre exponents in 20m! minus those in 15m! and 5m!, weighted by p.
fn exponent(mut n: usize, p: usize) -> usize {
    let mut e = 0;
    while n > 0 {
        n /= p;
        e += n;
    }
    e
}
fn solve() -> u64 {
    const N: usize = 20_000_000;
    let mut comp = vec![false; N + 1];
    for p in 2..=((N as f64).sqrt() as usize) {
        if !comp[p] {
            for m in (p * p..=N).step_by(p) {
                comp[m] = true;
            }
        }
    }
    (2..=N)
        .filter(|&p| !comp[p])
        .map(|p| {
            p as u64 * (exponent(N, p) - exponent(15_000_000, p) - exponent(5_000_000, p)) as u64
        })
        .sum()
}
fn main() {
    println!("{}", solve());
}
