//! Project Euler 71: Ordered Fractions.

fn solve() -> i64 {
    {
        let mut best_n = 0u64;
        let mut best_d = 1u64;
        for d in 2..=1_000_000u64 {
            let n = (3 * d - 1) / 7;
            if n * best_d > best_n * d {
                best_n = n;
                best_d = d;
            }
        }
        best_n as i64
    }
}

fn main() {
    println!("{}", solve());
}
