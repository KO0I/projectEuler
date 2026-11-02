//! Project Euler 86: Cuboid Route.
use project_euler::extra_helpers::is_square;

fn solve() -> i64 {
    {
        let mut total = 0u64;
        for maximum in 1u64.. {
            for sum in 2..=2 * maximum {
                if is_square(maximum * maximum + sum * sum) {
                    let low = if sum > maximum { sum - maximum } else { 1 };
                    let high = (sum / 2).min(maximum);
                    if high >= low {
                        total += high - low + 1;
                    }
                }
            }
            if total > 1_000_000 {
                return maximum as i64;
            }
        }
        unreachable!("unbounded cuboid search")
    }
}

fn main() {
    println!("{}", solve());
}
