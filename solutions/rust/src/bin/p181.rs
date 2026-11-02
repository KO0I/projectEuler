// Project Euler 181: two-colour groupings.
//
// A group type is the pair (black, white).  This is a two-dimensional
// unbounded coin-change calculation: processing types on the outside makes
// the resulting collection of groups unordered, so every partition is
// counted exactly once.
fn solve() -> u64 {
    let mut ways = [[0u64; 41]; 61];
    ways[0][0] = 1;
    for black in 0..=60 {
        for white in 0..=40 {
            if black == 0 && white == 0 {
                continue;
            }
            for b in black..=60 {
                for w in white..=40 {
                    ways[b][w] += ways[b - black][w - white];
                }
            }
        }
    }
    ways[60][40]
}
fn main() {
    println!("{}", solve());
}
