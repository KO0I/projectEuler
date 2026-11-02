//! Project Euler 53: Combinatoric Selections.

fn solve() -> i64 {
    {
        let mut row = [0u64; 101];
        row[0] = 1;
        let mut count = 0;
        for n in 1..=100 {
            for j in (1..=n).rev() {
                row[j] = (row[j] + row[j - 1]).min(1_000_001);
            }
            count += (0..=n).filter(|&j| row[j] > 1_000_000).count();
        }
        count as i64
    }
}

fn main() {
    println!("{}", solve());
}
