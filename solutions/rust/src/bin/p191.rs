// Project Euler 191: prize strings.
// State records whether A has appeared and the current run (0..2) of Ls.
fn solve() -> u64 {
    let mut dp = [[0u64; 3]; 2];
    dp[0][0] = 1;
    for _ in 0..30 {
        let mut next = [[0; 3]; 2];
        for a in 0..2 {
            for l in 0..3 {
                next[a][0] += dp[a][l];
                if a == 0 {
                    next[1][0] += dp[a][l];
                }
                if l < 2 {
                    next[a][l + 1] += dp[a][l];
                }
            }
        }
        dp = next;
    }
    dp.iter().flatten().sum()
}
fn main() {
    println!("{}", solve());
}
