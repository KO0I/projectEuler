//! Project Euler 77: Prime Summations.
use project_euler::extra_helpers::primes;

fn solve() -> i64 {
    {
        let mut n = 2usize;
        loop {
            let mut ways = vec![0u64; n + 1];
            ways[0] = 1;
            for p in primes(n) {
                let p = p as usize;
                for x in p..=n {
                    ways[x] += ways[x - p];
                }
            }
            if ways[n] > 5000 {
                break n as i64;
            }
            n += 1;
        }
    }
}

fn main() {
    println!("{}", solve());
}
