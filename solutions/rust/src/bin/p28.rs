//! Project Euler 28: Number Spiral Diagonals.

fn solve() -> i64 {
    {
        let mut sum = 1i64;
        for side in (3i64..=1001).step_by(2) {
            sum += 4 * side * side - 6 * (side - 1);
        }
        sum
    }
}

fn main() {
    println!("{}", solve());
}
