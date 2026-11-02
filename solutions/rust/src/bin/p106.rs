//! Project Euler 106: Special Subset Sums: Meta-testing.
use project_euler::extra_helpers::choose;

fn solve() -> i64 {
    {
        let n = 12u64;
        (2..=6)
            .map(|k| {
                let pairs = choose(n, 2 * k) * choose(2 * k, k) / 2;
                let comparable = choose(2 * k, k) / u128::from(k + 1);
                (pairs - choose(n, 2 * k) * comparable) as i64
            })
            .sum()
    }
}

fn main() {
    println!("{}", solve());
}
