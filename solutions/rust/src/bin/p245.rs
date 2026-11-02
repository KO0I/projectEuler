// Project Euler 245: coresilience.
// Squarefree two-prime solutions follow from (p-k)(q-k)=k²-k+1 and a
// polynomial factor sieve.  For >=3 primes, the final prime is algebraically
// forced from partial n and phi(n).
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
fn modsqrt(mut n: u64, p: u64) -> u64 {
    n %= p;
    if n == 0 {
        return 0;
    }
    if p % 4 == 3 {
        return pow(n, (p + 1) / 4, p);
    }
    let (mut q, mut s) = (p - 1, 0);
    while q & 1 == 0 {
        q >>= 1;
        s += 1;
    }
    let mut z = 2;
    while pow(z, (p - 1) / 2, p) != p - 1 {
        z += 1;
    }
    let (mut c, mut x, mut t, mut m) = (pow(z, q, p), pow(n, (q + 1) / 2, p), pow(n, q, p), s);
    while t != 1 {
        let mut i = 1;
        let mut v = mul(t, t, p);
        while v != 1 {
            v = mul(v, v, p);
            i += 1;
        }
        let b = pow(c, 1u64 << (m - i - 1), p);
        x = mul(x, b, p);
        c = mul(b, b, p);
        t = mul(t, c, p);
        m = i;
    }
    x
}
fn strip(p: usize, root: usize, remaining: &mut [u64], factors: &mut [Vec<(u64, u8)>]) {
    let mut start = root;
    while start < 2 {
        start += p;
    }
    for k in (start..remaining.len()).step_by(p) {
        if remaining[k] % p as u64 != 0 {
            continue;
        }
        let mut e = 0;
        while remaining[k] % p as u64 == 0 {
            remaining[k] /= p as u64;
            e += 1;
        }
        factors[k].push((p as u64, e));
    }
}
fn divisors(fs: &[(u64, u8)], i: usize, v: u64, out: &mut Vec<u64>) {
    if i == fs.len() {
        out.push(v);
        return;
    }
    let mut power = 1;
    for _ in 0..=fs[i].1 {
        divisors(fs, i + 1, v * power, out);
        power *= fs[i].0;
    }
}
fn last(a: u64, b: u64, last: u64) -> u64 {
    const L: u64 = 200_000_000_000;
    let d = a - b;
    let rmax = L / a;
    if d == 0 || rmax <= last {
        return 0;
    }
    let mut umin = (b * (last - 1) - 1) / (b + last * d) + 1;
    umin = umin.max(1);
    let mut umax = (b * (rmax - 1) - 1) / (b + rmax * d);
    umax = umax.min((b - 1) / d);
    let mut answer = 0;
    for u in umin..=umax {
        let den = b - u * d;
        if den == 0 {
            break;
        }
        let num = b * (u + 1) + 1;
        if num % den == 0 {
            let r = num / den;
            if r > last && a <= L / r && prime(r) {
                answer += a * r;
            }
        }
    }
    answer
}
fn dfs(ps: &[u64], index: usize, remaining: usize, a: u64, b: u64, last_p: u64) -> u64 {
    if remaining == 0 {
        return last(a, b, last_p);
    }
    let bound = 200_000_000_000u64 / a;
    let mut answer = 0;
    for i in index..ps.len() {
        let p = ps[i];
        let mut power = 1;
        let mut fits = true;
        for _ in 0..=remaining {
            if power > bound / p {
                fits = false;
                break;
            }
            power *= p;
        }
        if !fits {
            break;
        }
        answer += dfs(ps, i + 1, remaining - 1, a * p, b * (p - 1), p);
    }
    answer
}
fn solve() -> u64 {
    const K: usize = 447_213;
    let mut comp = vec![false; K + 1];
    let mut ps = Vec::new();
    for p in 2..=K {
        if !comp[p] {
            ps.push(p as u64);
            if p <= K / p {
                for m in (p * p..=K).step_by(p) {
                    comp[m] = true;
                }
            }
        }
    }
    let mut remaining: Vec<u64> = (0..=K)
        .map(|k| k as u64 * k as u64 - k as u64 + 1)
        .collect();
    let mut factors = vec![Vec::new(); K + 1];
    strip(3, 2, &mut remaining, &mut factors);
    for &p0 in &ps {
        let p = p0 as usize;
        if p == 3 || p % 3 != 1 {
            continue;
        }
        let root = modsqrt(p0 - 3, p0);
        let inv = (p0 + 1) / 2;
        let r1 = mul(1 + root, inv, p0) as usize;
        let r2 = mul(1 + p0 - root, inv, p0) as usize;
        strip(p, r1, &mut remaining, &mut factors);
        if r2 != r1 {
            strip(p, r2, &mut remaining, &mut factors);
        }
    }
    let mut answer = 0;
    for k in 2..=K {
        if remaining[k] > 1 {
            factors[k].push((remaining[k], 1));
        }
        let mut ds = Vec::new();
        divisors(&factors[k], 0, 1, &mut ds);
        let poly = k as u64 * k as u64 - k as u64 + 1;
        for v in ds {
            let other = poly / v;
            if v > other {
                continue;
            }
            let (p, q) = (k as u64 + v, k as u64 + other);
            if p != q && prime(p) && prime(q) && p <= 200_000_000_000 / q {
                answer += p * q;
            }
        }
    }
    let mut product = 1;
    let mut max = 0;
    for &p in ps.iter().skip(1) {
        if product > 200_000_000_000 / p {
            break;
        }
        product *= p;
        max += 1;
    }
    for count in 3..=max {
        answer += dfs(&ps, 1, count - 1, 1, 1, 2);
    }
    answer
}
fn main() {
    println!("{}", solve());
}
