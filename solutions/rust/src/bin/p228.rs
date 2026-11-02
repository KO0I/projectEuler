// Project Euler 228: Minkowski sums.
// Edge directions reduce to k/d; denominator d appears iff it divides some
// n in 1864..1909, contributing phi(d) distinct reduced directions.
fn solve() -> u32 {
    let mut phi: Vec<u32> = (0..=1909).collect();
    for p in 2..=1909 {
        if phi[p] == p as u32 {
            for m in (p..=1909).step_by(p) {
                phi[m] -= phi[m] / p as u32;
            }
        }
    }
    let mut present = [false; 1910];
    present[1] = true;
    for n in 1864..=1909 {
        let mut d = 1;
        while d * d <= n {
            if n % d == 0 {
                present[d] = true;
                present[n / d] = true;
            }
            d += 1;
        }
    }
    (1..=1909).filter(|&d| present[d]).map(|d| phi[d]).sum()
}
fn main() {
    println!("{}", solve());
}
