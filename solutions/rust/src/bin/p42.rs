//! Project Euler 42: Coded Triangle Numbers.
use project_euler::extra_helpers::is_square;

fn solve() -> i64 {
    {
        let words = include_str!("../../../../tests/data/p042_words.txt");
        words
            .split(',')
            .filter(|w| {
                let w = w.trim().trim_matches('"');
                let sum: u64 = w.bytes().map(|b| (b - b'A' + 1) as u64).sum();
                is_square(8 * sum + 1)
            })
            .count() as i64
    }
}

fn main() {
    println!("{}", solve());
}
