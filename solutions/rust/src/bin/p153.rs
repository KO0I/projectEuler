//! Sum real divisors and primitive Gaussian directions with all scales.
use project_euler::extra_helpers::gcd;
fn solve() -> u64 {
    const L: u64 = 100_000_000;
    let mut answer = (1..=L).map(|d| d * (L / d)).sum();
    for a in 1u64.. {
        if a * a >= L {
            break;
        }
        for b in 1u64.. {
            let norm = a * a + b * b;
            if norm > L {
                break;
            }
            if gcd(a, b) != 1 {
                continue;
            }
            for k in 1..=L / norm {
                answer += 2 * a * k * (L / (norm * k))
            }
        }
    }
    answer
}
fn main() {
    println!("{}", solve());
}
