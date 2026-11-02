//! Canonical reduced fractions represent capacitances for each exact component count.
use project_euler::extra_helpers::gcd;
use std::collections::HashSet;
#[derive(Clone, Copy, Hash, Eq, PartialEq)]
struct F(u64, u64);
fn f(n: u64, d: u64) -> F {
    let g = gcd(n, d);
    F(n / g, d / g)
}
fn solve() -> usize {
    let mut sets: Vec<HashSet<F>> = (0..=18).map(|_| HashSet::new()).collect();
    sets[1].insert(F(1, 1));
    for size in 2..=18 {
        let mut out = HashSet::new();
        for l in 1..=size / 2 {
            for &F(an, ad) in &sets[l] {
                for &F(bn, bd) in &sets[size - l] {
                    let sn = an * bd + bn * ad;
                    out.insert(f(sn, ad * bd));
                    out.insert(f(an * bn, sn));
                }
            }
        }
        sets[size] = out
    }
    let mut all: HashSet<F> = HashSet::new();
    for s in sets.iter().skip(1) {
        all.extend(s.iter().copied())
    }
    all.len()
}
fn main() {
    println!("{}", solve());
}
