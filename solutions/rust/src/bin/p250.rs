// Project Euler 250: 250250.
// Residue DP includes or omits each i^i mod 250.  A fresh array prevents
// reuse, and all counts are reduced modulo the requested 10^16 suffix.
fn pow250(mut b: u32, mut e: u32) -> usize {
    let mut r = 1;
    b %= 250;
    while e > 0 {
        if e & 1 == 1 {
            r = r * b % 250;
        }
        b = b * b % 250;
        e >>= 1;
    }
    r as usize
}
fn solve() -> u64 {
    const M: u64 = 10_000_000_000_000_000;
    let mut dp = [0u64; 250];
    dp[0] = 1;
    for i in 1..=250_250 {
        let v = pow250(i, i);
        let mut next = dp;
        for r in 0..250 {
            next[(r + v) % 250] = (next[(r + v) % 250] + dp[r]) % M;
        }
        dp = next;
    }
    (dp[0] + M - 1) % M
}
fn main() {
    println!("{:016}", solve());
}
