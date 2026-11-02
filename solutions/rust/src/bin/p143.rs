//! Eisenstein triples form a compatibility graph; valid triples are its 3-cycles.
use project_euler::extra_helpers::gcd;
fn solve() -> u64 {
    const L: usize = 120_000;
    let mut g = vec![Vec::<usize>::new(); L + 1];
    for m in 2usize.. {
        if m * m + 2 * m >= L {
            break;
        }
        for n in 1..m {
            if gcd(m as u64, n as u64) != 1 {
                continue;
            }
            let (mut p, mut q) = (m * m - n * n, 2 * m * n + n * n);
            if (m - n) % 3 == 0 {
                p /= 3;
                q /= 3
            }
            for k in 1.. {
                if k * (p + q) > L {
                    break;
                }
                g[k * p].push(k * q);
                g[k * q].push(k * p);
            }
        }
    }
    for v in &mut g {
        v.sort_unstable();
        v.dedup()
    }
    let mut sums = vec![false; L + 1];
    for p in 1..=L {
        let v = &g[p];
        for i in 0..v.len() {
            let q = v[i];
            if q <= p {
                continue;
            }
            for &r in &v[i + 1..] {
                if r <= q {
                    continue;
                }
                let s = p + q + r;
                if s > L {
                    break;
                }
                if g[q].binary_search(&r).is_ok() {
                    sums[s] = true
                }
            }
        }
    }
    sums.iter()
        .enumerate()
        .filter_map(|(n, &x)| x.then_some(n as u64))
        .sum()
}
fn main() {
    println!("{}", solve());
}
