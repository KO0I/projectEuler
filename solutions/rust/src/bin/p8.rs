//! Project Euler 8: Largest Product in a Series.

fn solve() -> i64 {
    {
        let digits: Vec<u64> = include_str!("../../../../tests/data/p008.txt")
            .bytes()
            .filter(|b| b.is_ascii_digit())
            .map(|b| u64::from(b - b'0'))
            .collect();
        assert_eq!(digits.len(), 1000);
        digits
            .windows(13)
            .map(|w| w.iter().copied().product::<u64>())
            .max()
            .unwrap() as i64
    }
}

fn main() {
    println!("{}", solve());
}
