// Project Euler 203: squarefree binomial coefficients.
// Build Pascal coefficients multiplicatively, deduplicate, then reject any
// value divisible by a prime square (testing all integers is still tiny here).
fn squarefree(v: u64) -> bool {
    let mut p = 2;
    while p <= v / p {
        if v % (p * p) == 0 {
            return false;
        }
        p += 1;
    }
    true
}
fn solve() -> u64 {
    let mut values = Vec::new();
    for n in 0..=50u64 {
        let mut c = 1;
        for k in 0..=n {
            values.push(c);
            if k < n {
                c = c * (n - k) / (k + 1);
            }
        }
    }
    values.sort_unstable();
    values.dedup();
    values.into_iter().filter(|&v| squarefree(v)).sum()
}
fn main() {
    println!("{}", solve());
}
