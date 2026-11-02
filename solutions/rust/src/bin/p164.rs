//! Rolling DP on the previous two digits.
fn solve() -> u64 {
    let mut dp = [[0u64; 10]; 10];
    for d in 1..=9 {
        dp[0][d] = 1
    }
    for _ in 1..20 {
        let mut next = [[0u64; 10]; 10];
        for a in 0..10 {
            for b in 0..10 {
                if a + b <= 9 {
                    for d in 0..=9 - a - b {
                        next[b][d] += dp[a][b]
                    }
                }
            }
        }
        dp = next
    }
    dp.iter().flatten().sum()
}
fn main() {
    println!("{}", solve());
}
