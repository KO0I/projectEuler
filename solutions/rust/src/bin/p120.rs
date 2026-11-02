//! Project Euler 120: Square Remainders.

fn solve() -> i64 {
    (3u64..=1000)
        .map(|a| if a % 2 == 0 { a * (a - 2) } else { a * (a - 1) })
        .sum::<u64>() as i64
}

fn main() {
    println!("{}", solve());
}
