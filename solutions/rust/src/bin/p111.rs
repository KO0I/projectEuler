//! Project Euler 111: Primes with Runs.
use project_euler::extra_helpers::prime;

fn solve() -> i64 {
    fn recurse(
        position: u32,
        length: u32,
        digit: u64,
        changes: u32,
        value: u64,
        count: &mut u64,
        sum: &mut u64,
    ) {
        if changes > length - position {
            return;
        }
        if position == length {
            if changes == 0 && prime(value) {
                *count += 1;
                *sum += value;
            }
            return;
        }
        if digit != 0 || position != 0 {
            recurse(
                position + 1,
                length,
                digit,
                changes,
                value * 10 + digit,
                count,
                sum,
            );
        }
        if changes > 0 {
            for d in if position == 0 { 1 } else { 0 }..=9 {
                if d as u64 != digit {
                    recurse(
                        position + 1,
                        length,
                        digit,
                        changes - 1,
                        value * 10 + d as u64,
                        count,
                        sum,
                    );
                }
            }
        }
    }
    let mut total = 0;
    for digit in 0..=9 {
        for changes in 0..=10 {
            let (mut count, mut sum) = (0, 0);
            recurse(0, 10, digit, changes, 0, &mut count, &mut sum);
            if count > 0 {
                total += sum;
                break;
            }
        }
    }
    total as i64
}

fn main() {
    println!("{}", solve());
}
