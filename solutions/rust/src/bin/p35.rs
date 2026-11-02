//! Project Euler 35: Circular Primes.
use project_euler::extra_helpers::sieve;

fn solve() -> i64 {
    {
        let p = sieve(1_000_000);
        let mut count = 0;
        for n in 2u32..1_000_000 {
            if !p[n as usize] {
                continue;
            }
            let mut magnitude = 1u32;
            while magnitude <= n / 10 {
                magnitude *= 10;
            }
            let mut rotated = n;
            let mut good = true;
            loop {
                rotated = rotated / 10 + (rotated % 10) * magnitude;
                if !p[rotated as usize] {
                    good = false;
                    break;
                }
                if rotated == n {
                    break;
                }
            }
            if good {
                count += 1;
            }
        }
        count
    }
}

fn main() {
    println!("{}", solve());
}
