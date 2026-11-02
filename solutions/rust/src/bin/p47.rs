//! Project Euler 47: Distinct Primes Factors.
use project_euler::extra_helpers::distinct_factors;

fn solve() -> i64 {
    {
        let mut consecutive = 0;
        let mut answer = 0;
        for n in 2u32..1_000_000 {
            if distinct_factors(n) == 4 {
                consecutive += 1;
                if consecutive == 4 {
                    answer = n - 3;
                    break;
                }
            } else {
                consecutive = 0;
            }
        }
        i64::from(answer)
    }
}

fn main() {
    println!("{}", solve());
}
