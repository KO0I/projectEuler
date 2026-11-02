// Project Euler 233: lattice points on a circle.
// The 1-mod-4 prime exponent patterns with product(2e+1)=105 generate a core.
// Prefix sums add cofactors containing no further 1-mod-4 prime factors.
const LIMIT: u64 = 100_000_000_000;
fn bounded(base: u64, e: u32, bound: u64) -> Option<u64> {
    let mut r = 1;
    for _ in 0..e {
        if r > bound / base {
            return None;
        }
        r *= base;
    }
    Some(r)
}
fn cores(
    exps: &[u32],
    depth: usize,
    core: u64,
    used: &mut Vec<usize>,
    primes: &[u64],
    prefix: &[u64],
) -> u64 {
    if depth == exps.len() {
        return core * prefix[(LIMIT / core) as usize];
    }
    let room = LIMIT / core;
    let mut answer = 0;
    for (i, &p) in primes.iter().enumerate() {
        if used.contains(&i) {
            continue;
        }
        let Some(power) = bounded(p, exps[depth], room) else {
            break;
        };
        used.push(i);
        answer += cores(exps, depth + 1, core * power, used, primes, prefix);
        used.pop();
    }
    answer
}
fn solve() -> u64 {
    const PL: usize = 4_800_000;
    const CL: usize = 278_454;
    let mut comp = vec![false; PL + 1];
    let mut primes = Vec::new();
    for p in 2..=PL {
        if !comp[p] {
            if p % 4 == 1 {
                primes.push(p as u64);
            }
            if p <= PL / p {
                for m in (p * p..=PL).step_by(p) {
                    comp[m] = true;
                }
            }
        }
    }
    let mut forbidden = vec![false; CL + 1];
    for &p in primes.iter().take_while(|&&p| p <= CL as u64) {
        for m in (p as usize..=CL).step_by(p as usize) {
            forbidden[m] = true;
        }
    }
    let mut prefix = vec![0u64; CL + 1];
    for m in 1..=CL {
        prefix[m] = prefix[m - 1] + if forbidden[m] { 0 } else { m as u64 };
    }
    [
        [52].as_slice(),
        [17, 1].as_slice(),
        [10, 2].as_slice(),
        [7, 3].as_slice(),
        [3, 2, 1].as_slice(),
    ]
    .iter()
    .map(|e| cores(e, 0, 1, &mut Vec::new(), &primes, &prefix))
    .sum()
}
fn main() {
    println!("{}", solve());
}
