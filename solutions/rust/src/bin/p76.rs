//! Project Euler 76: Counting Summations.

fn solve() -> i64 {
    {
        let mut ways = [0u64; 101];
        ways[0] = 1;
        for part in 1..100 {
            for n in part..=100 {
                ways[n] += ways[n - part];
            }
        }
        ways[100] as i64
    }
}

fn main() {
    println!("{}", solve());
}
