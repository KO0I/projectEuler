// Project Euler 193: squarefree numbers below 2^50.
// Möbius inversion gives Q(N)=sum mu(d)*floor(N/d^2).  The sieve flips mu
// for every distinct prime divisor and zeros multiples of prime squares.
fn solve() -> i64 {
    let limit = (1u64 << 50) - 1;
    let root = 1usize << 25;
    let mut mu = vec![1i8; root + 1];
    let mut composite = vec![false; root + 1];
    for p in 2..=root {
        if composite[p] {
            continue;
        }
        for m in (p..=root).step_by(p) {
            mu[m] = -mu[m];
        }
        if p <= root / p {
            let square = p * p;
            for m in (square..=root).step_by(square) {
                mu[m] = 0;
            }
            for m in (square..=root).step_by(p) {
                composite[m] = true;
            }
        }
    }
    (1..=root)
        .map(|d| mu[d] as i64 * (limit / (d as u64 * d as u64)) as i64)
        .sum()
}
fn main() {
    println!("{}", solve());
}
