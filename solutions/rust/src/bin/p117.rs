//! Project Euler 117: Red, Green, and Blue Tiles.

fn solve() -> i64 {
    {
        let mut ways = [0u64; 51];
        ways[0] = 1;
        for n in 1..=50 {
            for size in 1..=4 {
                if size <= n {
                    ways[n] += ways[n - size];
                }
            }
        }
        ways[50] as i64
    }
}

fn main() {
    println!("{}", solve());
}
