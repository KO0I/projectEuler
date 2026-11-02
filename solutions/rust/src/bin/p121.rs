//! Integer-weight dynamic program for the disc game.
fn solve() -> u64 {
    let mut ways = [0u64; 16];
    ways[0] = 1;
    let mut denominator = 1u64;
    for turn in 1..=15 {
        for r in (1..=turn).rev() {
            ways[r] += ways[r - 1] * turn as u64;
        }
        denominator *= (turn + 1) as u64;
    }
    denominator / ways[..8].iter().sum::<u64>()
}
fn main() {
    println!("{}", solve());
}
