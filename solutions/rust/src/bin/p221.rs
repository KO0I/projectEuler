// Project Euler 221: Alexandrian integers.
// (q-p)(r-p)=p^2+1, so divisor pairs d*e generate p(p+d)(p+e).  We factor
// every p^2+1 together by sieving its modular roots, then sort and deduplicate.
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
fn root_minus_one(p: u64) -> u64 {
    let mut g = 2;
    while powmod(g, (p - 1) / 2, p) != p - 1 {
        g += 1;
    }
    powmod(g, (p - 1) / 4, p)
}
fn solve() -> u64 {
    const P: usize = 500_000;
    let mut comp = vec![false; P + 1];
    for p in 2..=((P as f64).sqrt() as usize) {
        if !comp[p] {
            for m in (p * p..=P).step_by(p) {
                comp[m] = true;
            }
        }
    }
    let mut residual: Vec<u64> = (0..=P).map(|p| p as u64 * p as u64 + 1).collect();
    let mut factors = vec![Vec::<(u64, u8)>::new(); P + 1];
    for prime in 2..=P {
        if comp[prime] || (prime != 2 && prime % 4 != 1) {
            continue;
        }
        let root = if prime == 2 {
            1
        } else {
            root_minus_one(prime as u64) as usize
        };
        let roots = if prime == 2 {
            vec![root]
        } else {
            vec![root, prime - root]
        };
        for start in roots {
            for p in (start..=P).step_by(prime) {
                if residual[p] % prime as u64 != 0 {
                    continue;
                }
                let mut e = 0;
                while residual[p] % prime as u64 == 0 {
                    residual[p] /= prime as u64;
                    e += 1;
                }
                factors[p].push((prime as u64, e));
            }
        }
    }
    let mut candidates = Vec::with_capacity(2_000_000);
    for p in 1..=P {
        if residual[p] > 1 {
            factors[p].push((residual[p], 1));
        }
        let mut ds = vec![1u64];
        for &(prime, e) in &factors[p] {
            let old = ds.clone();
            let mut power = 1;
            for _ in 0..e {
                power *= prime;
                for &d in &old {
                    ds.push(d * power);
                }
            }
        }
        let n = p as u64 * p as u64 + 1;
        for d in ds {
            if d > n / d {
                continue;
            }
            let e = n / d;
            let wide = p as u128 * (p as u128 + d as u128) * (p as u128 + e as u128);
            if wide <= u64::MAX as u128 {
                candidates.push(wide as u64);
            }
        }
    }
    candidates.sort_unstable();
    candidates.dedup();
    let answer = candidates[149_999];
    assert!((P as u128 + 1) * (P as u128 + 2).pow(2) > answer as u128);
    answer
}
fn main() {
    println!("{}", solve());
}
