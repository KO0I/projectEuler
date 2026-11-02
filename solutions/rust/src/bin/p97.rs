//! Project Euler 97: Large Non-Mersenne Prime.
use project_euler::extra_helpers::mod_pow;

fn solve() -> i64 {
    ((28433u64 * mod_pow(2, 7_830_457, 10_000_000_000) + 1) % 10_000_000_000) as i64
}

fn main() {
    println!("{}", solve());
}
