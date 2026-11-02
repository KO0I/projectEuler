//! Sieve radicals, then order by (rad(n),n).
fn solve() -> usize {
    const N: usize = 100_000;
    let mut rad = vec![1usize; N + 1];
    for p in 2..=N {
        if rad[p] == 1 {
            for n in (p..=N).step_by(p) {
                rad[n] *= p;
            }
        }
    }
    let mut v: Vec<(usize, usize)> = (1..=N).map(|n| (rad[n], n)).collect();
    v.sort_unstable();
    v[9999].1
}
fn main() {
    println!("{}", solve());
}
