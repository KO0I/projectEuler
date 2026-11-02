// Project Euler 251: Cardano triplets.
// Parametrize a=3pq-1,b=pr,c=sq² with 8pq=sr²+3.  For fixed coprime q,r,
// modular inversion gives the first p and all later solutions form a linear
// arithmetic progression, counted at once under the sum bound.
fn gcd(mut a: i64, mut b: i64) -> i64 {
    while b != 0 {
        (a, b) = (b, a % b);
    }
    a
}
fn inverse(a: i64, m: i64) -> i64 {
    let (mut r0, mut r, mut s0, mut s) = (a, m, 1, 0);
    while r != 0 {
        let q = r0 / r;
        (r0, r) = (r, r0 - q * r);
        (s0, s) = (s, s0 - q * s);
    }
    s0.rem_euclid(m)
}
fn solve() -> u64 {
    let limit = 110_000_000i64;
    let l = limit + 1;
    let qmax = (limit as f64).sqrt() as i64;
    let mut total = 0;
    for q in 1..=qmax {
        let q2 = q * q;
        total += ((l + 3 * q2) / (8 * q * q2 + 3 * q + 1)) as u64;
        let mut rmax = (8.0 * q as f64 * l as f64).cbrt() as i64 + 3;
        if rmax & 1 == 0 {
            rmax += 1;
        }
        for r in (3..=rmax).step_by(2) {
            if gcd(q, r) != 1 {
                continue;
            }
            let r2 = r * r;
            let mut p0 = 3 * inverse(8 * q % r2, r2) % r2;
            if p0 == 0 {
                p0 = r2;
            }
            let s0 = (8 * p0 * q - 3) / r2;
            let base = p0 * (3 * q + r) + s0 * q2;
            if base > l {
                continue;
            }
            let step = r2 * (3 * q + r) + 8 * q * q2;
            total += ((l - base) / step + 1) as u64;
        }
    }
    total
}
fn main() {
    println!("{}", solve());
}
