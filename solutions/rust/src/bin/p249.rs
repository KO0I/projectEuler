// Project Euler 249: prime subset sums.
// Downward 0/1 subset DP counts sums modulo 10^16; a second sieve selects
// prime totals.  The empty subset is naturally excluded because zero is not prime.
fn solve() -> u64 {
    const MOD: u64 = 10_000_000_000_000_000;
    let mut comp = [false; 5000];
    let mut ps = Vec::new();
    let mut total = 0;
    for p in 2..5000 {
        if !comp[p] {
            ps.push(p);
            total += p;
            if p < 5000 / p {
                for m in (p * p..5000).step_by(p) {
                    comp[m] = true;
                }
            }
        }
    }
    let mut dp = vec![0u64; total + 1];
    dp[0] = 1;
    let mut current = 0;
    for p in ps {
        for s in (0..=current).rev() {
            dp[s + p] = (dp[s + p] + dp[s]) % MOD;
        }
        current += p;
    }
    let mut sum_comp = vec![false; total + 1];
    sum_comp[0] = true;
    sum_comp[1] = true;
    for p in 2..=((total as f64).sqrt() as usize) {
        if !sum_comp[p] {
            for m in (p * p..=total).step_by(p) {
                sum_comp[m] = true;
            }
        }
    }
    (2..=total)
        .filter(|&s| !sum_comp[s])
        .fold(0, |a, s| (a + dp[s]) % MOD)
}
fn main() {
    println!("{:016}", solve());
}
