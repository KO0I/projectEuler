//! Project Euler 119: Digit Power Sum.
use project_euler::extra_helpers::digits;

fn solve() -> i64 {
    for size in 2..=19u64 {
        let cutoff = 10u64.pow(size as u32);
        let mut values = Vec::new();
        for base in 2..=9 * size {
            let mut v = base * base;
            while v < cutoff {
                if v >= 10 && digits(v).iter().map(|&v| v as u64).sum::<u64>() == base {
                    values.push(v);
                }
                if v > (cutoff - 1) / base {
                    break;
                }
                v *= base;
            }
        }
        values.sort_unstable();
        values.dedup();
        if values.len() >= 30 {
            return values[29] as i64;
        }
    }
    panic!("30th digit-power term not found")
}

fn main() {
    println!("{}", solve());
}
