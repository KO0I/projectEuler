//! Project Euler 37: Truncatable Primes.
use project_euler::extra_helpers::prime;

fn solve() -> i64 {
    {
        fn traverse(n: u64, sum: &mut u64, count: &mut usize) {
            let mut power = 1;
            while power <= n / 10 {
                power *= 10;
            }
            if n >= 10 && {
                let mut p = power;
                let mut ok = true;
                while p > 1 {
                    if !prime(n % p) {
                        ok = false;
                        break;
                    }
                    p /= 10;
                }
                ok
            } {
                *sum += n;
                *count += 1;
            }
            for d in [1, 3, 7, 9] {
                let next = 10 * n + d;
                if prime(next) {
                    traverse(next, sum, count);
                }
            }
        }
        let mut sum = 0;
        let mut count = 0;
        for root in [2, 3, 5, 7] {
            traverse(root, &mut sum, &mut count);
        }
        assert_eq!(count, 11);
        sum as i64
    }
}

fn main() {
    println!("{}", solve());
}
