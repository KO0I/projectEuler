//! Project Euler 15: Lattice Paths.

fn solve() -> i64 {
    {
        let mut ways = [1u64; 21];
        for _ in 1..=20 {
            for j in 1..=20 {
                ways[j] += ways[j - 1];
            }
        }
        ways[20] as i64
    }
}

fn main() {
    println!("{}", solve());
}
