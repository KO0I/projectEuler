//! Project Euler 30: Digit Fifth Powers.
use project_euler::extra_helpers::digit_power_sum;

fn solve() -> i64 {
    {
        let p5: [u32; 10] = std::array::from_fn(|v| (v as u32).pow(5));
        (2u32..=354294)
            .filter(|&n| digit_power_sum(n, &p5) == n)
            .map(i64::from)
            .sum()
    }
}

fn main() {
    println!("{}", solve());
}
