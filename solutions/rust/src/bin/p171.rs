//! DP tracks both count and numeric sum for each digit-square total.
fn solve() -> u64 {
    const M: u64 = 1_000_000_000;
    let mut count = vec![0u64; 1621];
    let mut sum = vec![0u64; 1621];
    count[0] = 1;
    for pos in 0..20 {
        let (mut nc, mut ns) = (vec![0u64; 1621], vec![0u64; 1621]);
        for s in 0..=pos * 81 {
            for d in 0..10 {
                let t = s + d * d;
                nc[t] = (nc[t] + count[s]) % M;
                ns[t] = (ns[t] + sum[s] * 10 + count[s] * d as u64) % M
            }
        }
        count = nc;
        sum = ns
    }
    let mut answer = 0;
    for r in 1..=40 {
        answer = (answer + sum[r * r]) % M
    }
    answer
}
fn main() {
    println!("{}", solve());
}
