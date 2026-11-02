// Project Euler 241: perfection quotients.
// Search a reduced residual ratio (A/2)n/sigma(n).  Its denominator forces
// the next prime; deterministic Miller-Rabin and Pollard rho factor it.
fn gcd(mut a: u64, mut b: u64) -> u64 {
    while b != 0 {
        (a, b) = (b, a % b);
    }
    a
}
fn gcd128(mut a: u128, mut b: u128) -> u128 {
    while b != 0 {
        (a, b) = (b, a % b);
    }
    a
}
fn mul(a: u64, b: u64, m: u64) -> u64 {
    (a as u128 * b as u128 % m as u128) as u64
}
fn pow(mut a: u64, mut e: u64, m: u64) -> u64 {
    let mut r = 1;
    while e > 0 {
        if e & 1 == 1 {
            r = mul(r, a, m);
        }
        a = mul(a, a, m);
        e >>= 1;
    }
    r
}
fn prime(n: u64) -> bool {
    if n < 2 {
        return false;
    }
    for p in [2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37] {
        if n % p == 0 {
            return n == p;
        }
    }
    let (mut d, mut s) = (n - 1, 0);
    while d & 1 == 0 {
        d >>= 1;
        s += 1;
    }
    'w: for a in [2, 325, 9375, 28178, 450775, 9780504, 1795265022] {
        let a = a % n;
        if a == 0 {
            continue;
        }
        let mut x = pow(a, d, n);
        if x == 1 || x == n - 1 {
            continue;
        }
        for _ in 1..s {
            x = mul(x, x, n);
            if x == n - 1 {
                continue 'w;
            }
        }
        return false;
    }
    true
}
fn rho(n: u64) -> u64 {
    if n % 2 == 0 {
        return 2;
    }
    if n % 3 == 0 {
        return 3;
    }
    for c in 1.. {
        let (mut x, mut y, mut d) = (2, 2, 1);
        while d == 1 {
            x = (mul(x, x, n) + c) % n;
            y = (mul(y, y, n) + c) % n;
            y = (mul(y, y, n) + c) % n;
            d = gcd(x.abs_diff(y), n);
        }
        if d != n {
            return d;
        }
    }
    unreachable!()
}
fn smallest(n: u64) -> u64 {
    if prime(n) {
        n
    } else {
        let f = rho(n);
        smallest(f).min(smallest(n / f))
    }
}
fn search(n: u64, u: u64, v: u64, used: &mut Vec<u64>) -> u128 {
    if u == v {
        return n as u128;
    }
    if u < v || v == 1 || n as u128 * v as u128 > 1_000_000_000_000_000_000u128 {
        return 0;
    }
    let p = smallest(v);
    if used.contains(&p) {
        return 0;
    }
    let mut copy = v;
    let mut min_e = 0;
    while copy % p == 0 {
        copy /= p;
        min_e += 1;
    }
    used.push(p);
    let (mut total, mut power, mut sigma) = (0, 1u64, 1u64);
    for e in 1.. {
        if power > 1_000_000_000_000_000_000u64 / p / n {
            break;
        }
        power *= p;
        let Some(s) = sigma.checked_add(power) else {
            break;
        };
        sigma = s;
        if e < min_e {
            continue;
        }
        let (nu, nv) = (u as u128 * power as u128, v as u128 * sigma as u128);
        let g = gcd128(nu, nv);
        let (nu, nv) = (nu / g, nv / g);
        if nu > u64::MAX as u128 || nv > u64::MAX as u128 {
            continue;
        }
        if nu < nv {
            break;
        }
        total += search(n * power, nu as u64, nv as u64, used);
    }
    used.pop();
    total
}
fn solve() -> u128 {
    (3..=13)
        .step_by(2)
        .map(|a| search(1, a, 2, &mut Vec::new()))
        .sum()
}
fn main() {
    println!("{}", solve());
}
