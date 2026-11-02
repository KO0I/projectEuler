//! Project Euler 38: Pandigital Multiples.
use project_euler::extra_helpers::pandigital_1_to_9;

fn solve() -> i64 {
    {
        let mut best = 0u64;
        for start in 1..10000 {
            let mut joined = String::new();
            let mut n = 1;
            while joined.len() < 9 {
                joined.push_str(&(start * n).to_string());
                n += 1;
            }
            if joined.len() == 9 && n > 2 {
                let value = joined.parse().unwrap();
                if pandigital_1_to_9(value) {
                    best = best.max(value);
                }
            }
        }
        best as i64
    }
}

fn main() {
    println!("{}", solve());
}
