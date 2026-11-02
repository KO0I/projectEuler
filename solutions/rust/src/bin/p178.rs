//! Last digit plus used-digit mask is the complete step-number state.
fn solve() -> u64 {
    let mut dp = vec![vec![0u64; 1024]; 10];
    for d in 1..=9 {
        dp[d][1 << d] = 1
    }
    let mut answer = 0;
    for len in 1..=40 {
        if len >= 10 {
            answer += (0..10).map(|d| dp[d][1023]).sum::<u64>()
        }
        let mut n = vec![vec![0u64; 1024]; 10];
        for d in 0..10 {
            for m in 0..1024 {
                if d > 0 {
                    n[d - 1][m | 1 << (d - 1)] += dp[d][m]
                }
                if d < 9 {
                    n[d + 1][m | 1 << (d + 1)] += dp[d][m]
                }
            }
        }
        dp = n
    }
    answer
}
fn main() {
    println!("{}", solve());
}
