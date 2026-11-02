// Project Euler 216: primality of 2n^2-1.
// Sieve the polynomial: for primes p=1 or 7 mod 8, Tonelli-Shanks gives the
// two residue classes whose values are divisible by p.
fn powmod(mut b: u64, mut e: u64, m: u64) -> u64 {
    let mut r = 1;
    while e > 0 {
        if e & 1 == 1 {
            r = r * b % m;
        }
        b = b * b % m;
        e >>= 1;
    }
    r
}
fn sqrt_mod(v: u64, p: u64) -> u64 {
    if p % 4 == 3 {
        return powmod(v, (p + 1) / 4, p);
    }
    let (mut q, mut s) = (p - 1, 0);
    while q & 1 == 0 {
        q >>= 1;
        s += 1;
    }
    let mut z = 2;
    while powmod(z, (p - 1) / 2, p) != p - 1 {
        z += 1;
    }
    let (mut c, mut x, mut t, mut m) = (
        powmod(z, q, p),
        powmod(v, (q + 1) / 2, p),
        powmod(v, q, p),
        s,
    );
    while t != 1 {
        let mut i = 1;
        let mut y = t * t % p;
        while y != 1 {
            y = y * y % p;
            i += 1;
        }
        let b = powmod(c, 1u64 << (m - i - 1), p);
        x = x * b % p;
        c = b * b % p;
        t = t * c % p;
        m = i;
    }
    x
}
fn solve() -> usize {
    const N: usize = 50_000_000;
    const P: usize = 70_710_678;
    let mut comp = vec![false; P + 1];
    for p in 2..=((P as f64).sqrt() as usize) {
        if !comp[p] {
            for m in (p * p..=P).step_by(p) {
                comp[m] = true;
            }
        }
    }
    let mut values = vec![false; N + 1];
    for p in (3..=P).step_by(2) {
        if comp[p] || (p % 8 != 1 && p % 8 != 7) {
            continue;
        }
        let r = sqrt_mod(((p + 1) / 2) as u64, p as u64) as usize;
        for n in (r..=N).step_by(p) {
            if n >= 2 {
                values[n] = true;
            }
        }
        let other = p - r;
        if other != r {
            for n in (other..=N).step_by(p) {
                if n >= 2 {
                    values[n] = true;
                }
            }
        }
    }
    let restore = ((P + 1) as f64 / 2.0).sqrt() as usize + 1;
    for n in 2..=restore {
        let v = 2 * n * n - 1;
        if v <= P && !comp[v] {
            values[n] = false;
        }
    }
    (2..=N).filter(|&n| !values[n]).count()
}
fn main() {
    println!("{}", solve());
}
