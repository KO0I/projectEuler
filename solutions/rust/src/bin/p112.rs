//! Project Euler 112: Bouncy Numbers.
use project_euler::extra_helpers::digits;

fn solve() -> i64 {
    {
        fn bouncy(n: u64) -> bool {
            let d = digits(n);
            !(d.windows(2).all(|v| v[0] <= v[1]) || d.windows(2).all(|v| v[0] >= v[1]))
        }
        let mut count = 0;
        let mut n = 99u64;
        loop {
            n += 1;
            if bouncy(n) {
                count += 1;
            }
            if 100 * count == 99 * n {
                break n as i64;
            }
        }
    }
}

fn main() {
    println!("{}", solve());
}
