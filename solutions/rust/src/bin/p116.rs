//! Project Euler 116: Red, Green or Blue Tiles.

fn solve() -> i64 {
    {
        let mut sum = 0u64;
        for size in [2usize, 3, 4] {
            let mut ways = [0u64; 51];
            ways[0] = 1;
            for n in 1..=50 {
                ways[n] = ways[n - 1] + if n >= size { ways[n - size] } else { 0 };
            }
            sum += ways[50] - 1;
        }
        sum as i64
    }
}

fn main() {
    println!("{}", solve());
}
