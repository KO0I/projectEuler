//! Project Euler 1: Multiples of 3 or 5.

fn solve() -> i64 {
    (1i64..1000).filter(|v| v % 3 == 0 || v % 5 == 0).sum()
}

fn main() {
    println!("{}", solve());
}
