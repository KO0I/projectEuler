//! Project Euler 50: Consecutive Prime Sum.
use project_euler::extra_helpers::sieve;

fn solve() -> i64 {
    {
        let p = sieve(1_000_000);
        let mut prefix = vec![0u64];
        for (n, &is_prime) in p.iter().enumerate().skip(2).take(999998) {
            if is_prime {
                prefix.push(*prefix.last().unwrap() + n as u64);
            }
        }
        let (mut length, mut best) = (0usize, 0i64);
        for i in 0..prefix.len() - 1 {
            for j in (i + length + 1)..prefix.len() {
                let sum = prefix[j] - prefix[i];
                if sum >= 1_000_000 {
                    break;
                }
                if p[sum as usize] {
                    length = j - i;
                    best = sum as i64;
                }
            }
        }
        best
    }
}

fn main() {
    println!("{}", solve());
}
