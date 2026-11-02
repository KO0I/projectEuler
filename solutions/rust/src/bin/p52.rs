//! Project Euler 52: Permuted Multiples.
use project_euler::extra_helpers::decimal_signature;

fn solve() -> i64 {
    {
        let mut lower = 1u64;
        let mut upper = 9u64;
        loop {
            let answer = (lower..=upper / 6).find(|&n| {
                let signature = decimal_signature(n);
                (2..=6).all(|k| decimal_signature(k * n) == signature)
            });
            if let Some(v) = answer {
                break v as i64;
            }
            lower = upper + 1;
            upper = upper * 10 + 9;
        }
    }
}

fn main() {
    println!("{}", solve());
}
