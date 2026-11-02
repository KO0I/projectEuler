// Project Euler 248: inverse totient of 13!.
// Every prime divisor has p-1 dividing 13!.  Enumerate those primes and
// recursively compose all prime powers whose totient contributions divide it.
const TARGET: u64 = 6_227_020_800;
fn prime(n: u64, ps: &[u64]) -> bool {
    n >= 2 && !ps.iter().take_while(|&&p| p <= n / p).any(|p| n % p == 0)
}
fn divisors(at: usize, value: u64, out: &mut Vec<u64>) {
    let ps = [2, 3, 5, 7, 11, 13];
    let es = [10, 5, 2, 1, 1, 1];
    if at == 6 {
        out.push(value);
        return;
    }
    let mut power = 1;
    for _ in 0..=es[at] {
        divisors(at + 1, value * power, out);
        power *= ps[at];
    }
}
fn generate(start: usize, remaining: u64, n: u64, candidates: &[u64], out: &mut Vec<u64>) {
    if remaining == 1 {
        out.push(n);
        return;
    }
    for i in start..candidates.len() {
        let p = candidates[i];
        let (mut contribution, mut power) = (p - 1, p);
        if remaining % contribution != 0 {
            continue;
        }
        while remaining % contribution == 0 {
            if let Some(next) = n.checked_mul(power) {
                generate(i + 1, remaining / contribution, next, candidates, out);
            }
            if contribution > remaining / p || power > u64::MAX / p {
                break;
            }
            contribution *= p;
            power *= p;
        }
    }
}
fn solve() -> u64 {
    let mut composite = vec![false; 80_001];
    let mut ps = Vec::new();
    for p in 2..=80_000 {
        if !composite[p] {
            ps.push(p as u64);
            if p <= 80_000 / p {
                for m in (p * p..=80_000).step_by(p) {
                    composite[m] = true;
                }
            }
        }
    }
    let mut ds = Vec::new();
    divisors(0, 1, &mut ds);
    let mut candidates: Vec<_> = ds
        .into_iter()
        .map(|d| d + 1)
        .filter(|&p| prime(p, &ps))
        .collect();
    candidates.sort_unstable();
    let mut out = Vec::new();
    generate(0, TARGET, 1, &candidates, &mut out);
    out.sort_unstable();
    out[149_999]
}
fn main() {
    println!("{}", solve());
}
