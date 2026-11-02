//! Project Euler 21: Amicable Numbers.
use project_euler::extra_helpers::divisor_sum;

fn solve() -> i64 {
    (2u32..10000)
        .filter(|&a| {
            let b = divisor_sum(a);
            b != a && b > 0 && divisor_sum(b) == a
        })
        .map(i64::from)
        .sum()
}

fn main() {
    println!("{}", solve());
}
