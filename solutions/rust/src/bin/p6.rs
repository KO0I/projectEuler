//! Project Euler 6: Sum Square Difference.

fn solve() -> i64 {
    {
        let sum = 100i64 * 101 / 2;
        sum * sum - 100 * 101 * 201 / 6
    }
}

fn main() {
    println!("{}", solve());
}
