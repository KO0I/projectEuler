//! Choose letters and one nontrivial rise among otherwise ordered positions.
fn solve() -> u64 {
    let (mut choose, mut power, mut best) = (1u64, 1u64, 0u64);
    for n in 1..=26 {
        choose = choose * (27 - n) / n;
        power *= 2;
        best = best.max(choose * (power - n - 1))
    }
    best
}
fn main() {
    println!("{}", solve());
}
