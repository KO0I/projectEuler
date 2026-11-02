//! Radical-ordered abc-hit search with the radical product as a stopping bound.
use project_euler::extra_helpers::gcd;
fn solve() -> u64 {
    const L: usize = 120_000;
    let mut rad = vec![1usize; L];
    for p in 2..L {
        if rad[p] == 1 {
            for n in (p..L).step_by(p) {
                rad[n] *= p;
            }
        }
    }
    let mut ordered: Vec<_> = (1..L).map(|n| (rad[n], n)).collect();
    ordered.sort_unstable();
    let mut total = 0;
    for c in 3..L {
        for &(ra, a) in &ordered {
            if ra * rad[c] >= c {
                break;
            }
            if a >= (c + 1) / 2 {
                continue;
            }
            let b = c - a;
            if gcd(a as u64, c as u64) == 1
                && rad[a] as u64 * rad[b] as u64 * (rad[c] as u64) < c as u64
            {
                total += c as u64;
            }
        }
    }
    total
}
fn main() {
    println!("{}", solve());
}
