// Project Euler 229: four representations using squares.
// Admissible squarefree kernels contain only primes 1,25,121 mod 168.  A
// bit-packed odd sieve and product DFS count q*m^2; then perfect-square cases
// admitted through zero coefficients are corrected by exact factor pairs.
const LIMIT: u64 = 2_000_000_000;
const ROOT: usize = 44_721;
fn isqrt(n: u64) -> u64 {
    let mut r = (n as f64).sqrt() as u64;
    while (r + 1) <= n / (r + 1) {
        r += 1;
    }
    while r > n / r {
        r -= 1;
    }
    r
}
fn bit_get(bits: &[u8], v: u64) -> bool {
    let i = v >> 1;
    bits[(i >> 3) as usize] >> (i & 7) & 1 == 1
}
fn bit_set(bits: &mut [u8], v: u64) {
    let i = v >> 1;
    bits[(i >> 3) as usize] |= 1 << (i & 7);
}
fn products(ps: &[u32], start: usize, product: u64, depth: u8) -> u64 {
    let mut sum = 0;
    for i in start..ps.len() {
        let p = ps[i] as u64;
        if p > LIMIT / product {
            break;
        }
        let next = product * p;
        if depth + 1 >= 2 {
            sum += isqrt(LIMIT / next);
        }
        sum += products(ps, i + 1, next, depth + 1);
    }
    sum
}
fn factor(mut v: usize, spf: &[u32], fs: &mut Vec<(u64, u32)>, mult: u32) {
    while v > 1 {
        let p = if spf[v] == 0 { v } else { spf[v] as usize };
        let mut e = 0;
        while v % p == 0 {
            v /= p;
            e += 1;
        }
        if let Some(x) = fs.iter_mut().find(|x| x.0 == p as u64) {
            x.1 += mult * e
        } else {
            fs.push((p as u64, mult * e));
        }
    }
}
fn solve() -> u64 {
    let odd = LIMIT / 2 + 1;
    let mut bits = vec![0u8; odd.div_ceil(8) as usize];
    let mut p = 3;
    while p * p <= LIMIT {
        if !bit_get(&bits, p) {
            let mut m = p * p;
            while m <= LIMIT {
                bit_set(&mut bits, m);
                m += 2 * p;
            }
        }
        p += 2;
    }
    let mut relaxed = isqrt(LIMIT);
    let mut good = Vec::new();
    for residue in [1u64, 25, 121] {
        let mut v = residue;
        if v < 193 {
            v += (193 - v).div_ceil(168) * 168;
        }
        while v <= LIMIT {
            if !bit_get(&bits, v) {
                relaxed += isqrt(LIMIT / v);
                if v <= LIMIT / 193 {
                    good.push(v as u32);
                }
            }
            v += 168;
        }
    }
    good.sort_unstable();
    relaxed += products(&good, 0, 1, 0);
    drop(bits);
    let mut spf = vec![0u32; ROOT + 1];
    for p in 2..=((ROOT as f64).sqrt() as usize) {
        if spf[p] == 0 {
            for m in (p * p..=ROOT).step_by(p) {
                if spf[m] == 0 {
                    spf[m] = p as u32;
                }
            }
        }
    }
    let mut forms = vec![vec![false; ROOT + 1]; 4];
    for (di, d) in [1usize, 2, 3, 7].into_iter().enumerate() {
        let max = isqrt(LIMIT / d as u64) as usize;
        for b in 1..=max {
            let mut fs = Vec::new();
            factor(d, &spf, &mut fs, 1);
            factor(b, &spf, &mut fs, 2);
            let mut ds = vec![1u64];
            for &(p, e) in &fs {
                let old = ds.clone();
                let mut power = 1;
                for _ in 0..e {
                    power *= p;
                    for &x in &old {
                        ds.push(x * power);
                    }
                }
            }
            let product = d as u64 * b as u64 * b as u64;
            for u in ds {
                let v = product / u;
                if u < v && (u ^ v) & 1 == 0 {
                    let m = ((u + v) / 2) as usize;
                    if m <= ROOT {
                        forms[di][m] = true;
                    }
                }
            }
        }
    }
    let valid = (1..=ROOT).filter(|&m| (0..4).all(|d| forms[d][m])).count() as u64;
    relaxed - ROOT as u64 + valid
}
fn main() {
    println!("{}", solve());
}
