//! Project Euler 34: Digit Factorials.
use project_euler::extra_helpers::{digit_power_sum, factorial};

fn solve() -> i64 {
    {
        let table: [u32; 10] = std::array::from_fn(|v| factorial(v as u32));
        (3u32..=2_540_160)
            .filter(|&n| digit_power_sum(n, &table) == n)
            .map(i64::from)
            .sum()
    }
}

fn main() {
    println!("{}", solve());
}
