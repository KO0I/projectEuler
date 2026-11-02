//! Project Euler 23: Non-Abundant Sums.
use project_euler::extra_helpers::proper_divisor_sums;

fn solve() -> i64 {
    {
        let divs = proper_divisor_sums(28123);
        let abundant: Vec<usize> = (1..=28123).filter(|&x| divs[x] > x).collect();
        let mut represented = vec![false; 28124];
        for (i, &a) in abundant.iter().enumerate() {
            for &b in &abundant[i..] {
                if a + b > 28123 {
                    break;
                }
                represented[a + b] = true;
            }
        }
        (1..=28123).filter(|&x| !represented[x]).sum::<usize>() as i64
    }
}

fn main() {
    println!("{}", solve());
}
