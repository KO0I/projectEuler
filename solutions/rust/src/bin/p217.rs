// Project Euler 217: balanced numbers.
// For each half length and digit sum, track both string count and aggregate
// numeric value.  Matching left/right buckets combine algebraically.
const MAX: usize = 207;
const MOD: i64 = 14_348_907;
fn append(count: &[i64], sum: &[i64], old_max: usize) -> (Vec<i64>, Vec<i64>) {
    let (mut nc, mut ns) = (vec![0; MAX + 1], vec![0; MAX + 1]);
    for s in 0..=old_max {
        for d in 0..=9 {
            nc[s + d] = (nc[s + d] + count[s]) % MOD;
            ns[s + d] = (ns[s + d] + 10 * sum[s] + d as i64 * count[s]) % MOD;
        }
    }
    (nc, ns)
}
fn solve() -> i64 {
    let (mut rc, mut rs, mut lc, mut ls) = (
        vec![0; MAX + 1],
        vec![0; MAX + 1],
        vec![0; MAX + 1],
        vec![0; MAX + 1],
    );
    rc[0] = 1;
    let mut p10 = [0i64; 48];
    p10[0] = 1;
    for i in 1..48 {
        p10[i] = p10[i - 1] * 10 % MOD;
    }
    let mut answer = 45;
    for h in 1..=23 {
        (rc, rs) = append(&rc, &rs, 9 * (h - 1));
        if h == 1 {
            for d in 1..=9 {
                lc[d] = 1;
                ls[d] = d as i64;
            }
        } else {
            (lc, ls) = append(&lc, &ls, 9 * (h - 1));
        }
        let (mut even, mut odd) = (0, 0);
        for s in 0..=9 * h {
            let pairs = lc[s] * rc[s] % MOD;
            even = (even + ls[s] * rc[s] % MOD * p10[h] + rs[s] * lc[s]) % MOD;
            let outer =
                (10 * ls[s] % MOD * rc[s] % MOD * p10[h + 1] + 10 * rs[s] % MOD * lc[s]) % MOD;
            let middle = 45 * p10[h] % MOD * pairs % MOD;
            odd = (odd + outer + middle) % MOD;
        }
        answer = (answer + even + odd) % MOD;
    }
    answer
}
fn main() {
    println!("{}", solve());
}
