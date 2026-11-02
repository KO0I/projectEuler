//! Project Euler 114: Counting Block Combinations I.

fn solve() -> i64 {
    {
        let mut ways = [0u64; 51];
        ways[0] = 1;
        for n in 1..=50 {
            ways[n] = ways[n - 1];
            for red in 3..=n {
                ways[n] += if red == n { 1 } else { ways[n - red - 1] };
            }
        }
        ways[50] as i64
    }
}

fn main() {
    println!("{}", solve());
}
