//! Project Euler 75: Singular Integer Right Triangles.
use project_euler::extra_helpers::gcd;

fn solve() -> i64 {
    {
        let mut ways = vec![0u8; 1_500_001];
        for m in 2u64..=866 {
            for n in 1..m {
                if (m - n) % 2 == 0 || gcd(m, n) != 1 {
                    continue;
                }
                let perimeter = 2 * m * (m + n);
                if perimeter > 1_500_000 {
                    break;
                }
                for p in (perimeter as usize..=1_500_000).step_by(perimeter as usize) {
                    ways[p] = ways[p].saturating_add(1);
                }
            }
        }
        ways.iter().filter(|&&n| n == 1).count() as i64
    }
}

fn main() {
    println!("{}", solve());
}
