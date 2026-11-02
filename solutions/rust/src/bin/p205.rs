// Project Euler 205: dice game.
// Repeated convolution gives exact sum frequencies; only final formatting
// converts the favorable/total ratio to floating point.
fn distribution(dice: usize, sides: usize) -> [u64; 64] {
    let mut cur = [0u64; 64];
    cur[0] = 1;
    for _ in 0..dice {
        let mut next = [0; 64];
        for s in 0..64 - sides {
            for f in 1..=sides {
                next[s + f] += cur[s];
            }
        }
        cur = next;
    }
    cur
}
fn solve() -> f64 {
    let p = distribution(9, 4);
    let c = distribution(6, 6);
    let (mut favorable, mut lower) = (0u64, 0u64);
    for s in 0..64 {
        favorable += p[s] * lower;
        lower += c[s];
    }
    favorable as f64 / (4u64.pow(9) * 6u64.pow(6)) as f64
}
fn main() {
    println!("{:.7}", solve());
}
