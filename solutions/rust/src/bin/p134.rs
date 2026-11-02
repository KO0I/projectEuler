//! Solve scale*k+p1 == 0 (mod p2) for consecutive primes.
use project_euler::extra_helpers::sieve;
fn inv(mut a: i64, mut m: i64) -> u64 {
    let modulus = m;
    let (mut x0, mut x1) = (1i64, 0i64);
    while m != 0 {
        let q = a / m;
        (a, m) = (m, a - q * m);
        (x0, x1) = (x1, x0 - q * x1);
    }
    x0.rem_euclid(modulus) as u64
}
fn solve() -> u64 {
    let ps: Vec<_> = sieve(1_100_000)
        .iter()
        .enumerate()
        .filter_map(|(n, &v)| (v && n >= 5).then_some(n as u64))
        .collect();
    let mut total = 0;
    for w in ps.windows(2) {
        let (p1, p2) = (w[0], w[1]);
        if p1 > 1_000_000 {
            break;
        }
        let mut scale = 10;
        while scale <= p1 {
            scale *= 10
        }
        let k = (p2 - p1 % p2) * inv((scale % p2) as i64, p2 as i64) % p2;
        total += k * scale + p1;
    }
    total
}
fn main() {
    println!("{}", solve());
}
