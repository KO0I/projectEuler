// Project Euler 211: divisor-square sums.
// Segment the factor sieve to avoid a 64-million-entry SPF table.  Each prime
// contributes its geometric sigma_2 factor; a leftover residual is prime.
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
fn solve() -> u64 {
    const LIMIT: usize = 64_000_000;
    const BLOCK: usize = 1_000_000;
    let mut comp = [false; 8001];
    let mut ps = Vec::new();
    for n in 2..=8000 {
        if !comp[n] {
            ps.push(n);
            if n <= 8000 / n {
                for m in (n * n..=8000).step_by(n) {
                    comp[m] = true;
                }
            }
        }
    }
    let (mut residual, mut sigma) = (vec![0u64; BLOCK], vec![1u64; BLOCK]);
    let mut answer = 0;
    for low in (1..LIMIT).step_by(BLOCK) {
        let high = (low + BLOCK).min(LIMIT);
        let len = high - low;
        for i in 0..len {
            residual[i] = (low + i) as u64;
            sigma[i] = 1;
        }
        for &p0 in &ps {
            let p = p0 as u64;
            if p * p >= high as u64 {
                break;
            }
            let first = low.div_ceil(p0) * p0;
            for v in (first..high).step_by(p0) {
                let i = v - low;
                let (mut term, mut power) = (1, 1);
                while residual[i] % p == 0 {
                    residual[i] /= p;
                    power *= p * p;
                    term += power;
                }
                sigma[i] *= term;
            }
        }
        for i in 0..len {
            if residual[i] > 1 {
                sigma[i] *= 1 + residual[i] * residual[i];
            }
            let r = isqrt(sigma[i]);
            if r * r == sigma[i] {
                answer += (low + i) as u64;
            }
        }
    }
    answer
}
fn main() {
    println!("{}", solve());
}
