//! Project Euler 115: Counting Block Combinations II.

fn solve() -> i64 {
    {
        let mut ways = vec![1u64];
        for n in 1.. {
            let mut count = ways[n - 1];
            for block in 50..=n {
                count += if block == n { 1 } else { ways[n - block - 1] };
            }
            ways.push(count);
            if count > 1_000_000 {
                return n as i64;
            }
        }
        unreachable!("unbounded tiling search")
    }
}

fn main() {
    println!("{}", solve());
}
