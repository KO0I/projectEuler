//! Project Euler 64: Odd Period Square Roots.
use project_euler::extra_helpers::isqrt;

fn solve() -> i64 {
    {
        let mut count = 0;
        for n in 2u64..=10000 {
            let a0 = isqrt(n);
            if a0 * a0 == n {
                continue;
            }
            let (mut m, mut d, mut a) = (0u64, 1u64, a0);
            let mut period = 0;
            loop {
                m = d * a - m;
                d = (n - m * m) / d;
                a = (a0 + m) / d;
                period += 1;
                if a == 2 * a0 {
                    break;
                }
            }
            if period % 2 == 1 {
                count += 1;
            }
        }
        count
    }
}

fn main() {
    println!("{}", solve());
}
