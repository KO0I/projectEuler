//! Project Euler 46: Goldbach's Other Conjecture.
use project_euler::extra_helpers::prime;

fn solve() -> i64 {
    {
        let mut n = 9;
        loop {
            if !prime(n) {
                let mut representation = false;
                for p in 2..n {
                    if !prime(p) {
                        continue;
                    }
                    if (n - p) % 2 != 0 {
                        continue;
                    }
                    let half = (n - p) / 2;
                    let r = (half as f64).sqrt() as u64;
                    if r * r == half {
                        representation = true;
                        break;
                    }
                }
                if !representation {
                    break n as i64;
                }
            }
            n += 2;
        }
    }
}

fn main() {
    println!("{}", solve());
}
