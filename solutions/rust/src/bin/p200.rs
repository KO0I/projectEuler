// Project Euler 200: prime-proof squbes containing "200".
// Generate p^2q^3 below a proven-sufficient ceiling, sort, and test decimal
// replacements using deterministic 64-bit Miller-Rabin.
fn powmod(mut b: u64, mut e: u64, m: u64) -> u64 {
    let mut r = 1;
    while e > 0 {
        if e & 1 == 1 {
            r = (r as u128 * b as u128 % m as u128) as u64;
        }
        b = (b as u128 * b as u128 % m as u128) as u64;
        e >>= 1;
    }
    r
}
fn prime(n: u64) -> bool {
    for p in [2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37] {
        if n % p == 0 {
            return n == p;
        }
    }
    if n < 2 {
        return false;
    }
    let (mut d, mut s) = (n - 1, 0);
    while d & 1 == 0 {
        d >>= 1;
        s += 1;
    }
    'outer: for a in [2, 325, 9375, 28178, 450775, 9780504, 1795265022] {
        let a = a % n;
        if a == 0 {
            continue;
        }
        let mut x = powmod(a, d, n);
        if x == 1 || x == n - 1 {
            continue;
        }
        for _ in 1..s {
            x = (x as u128 * x as u128 % n as u128) as u64;
            if x == n - 1 {
                continue 'outer;
            }
        }
        return false;
    }
    true
}
fn prime_proof(n: u64) -> bool {
    let mut ds = n.to_string().into_bytes();
    for pos in 0..ds.len() {
        let old = ds[pos];
        for d in b'0'..=b'9' {
            if d == old {
                continue;
            }
            ds[pos] = d;
            let v = std::str::from_utf8(&ds).unwrap().parse().unwrap();
            if prime(v) {
                ds[pos] = old;
                return false;
            }
        }
        ds[pos] = old;
    }
    true
}
fn solve() -> u64 {
    const LIMIT: u64 = 1_000_000_000_000;
    let lim = 1_000_000;
    let mut comp = vec![false; lim + 1];
    let mut ps = Vec::new();
    for n in 2..=lim {
        if !comp[n] {
            ps.push(n as u64);
            if n <= lim / n {
                for m in (n * n..=lim).step_by(n) {
                    comp[m] = true;
                }
            }
        }
    }
    let mut values = Vec::new();
    for &q in &ps {
        let q3 = q * q * q;
        if q3 > LIMIT {
            break;
        }
        for &p in &ps {
            if p * p > LIMIT / q3 {
                break;
            }
            if p != q {
                values.push(p * p * q3);
            }
        }
    }
    values.sort_unstable();
    let mut found = 0;
    for v in values {
        if v.to_string().contains("200") && prime_proof(v) {
            found += 1;
            if found == 200 {
                return v;
            }
        }
    }
    panic!("search ceiling insufficient")
}
fn main() {
    println!("{}", solve());
}
