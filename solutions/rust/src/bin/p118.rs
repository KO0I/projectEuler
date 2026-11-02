//! Project Euler 118: Pandigital Prime Sets.
use project_euler::extra_helpers::prime;

fn solve() -> i64 {
    fn enumerate(mask: usize, value: u64, digit_sum: u32, counts: &mut [u64; 512]) {
        if mask != 0
            && (matches!(value, 2 | 3 | 5 | 7)
                || (digit_sum % 3 != 0 && value % 2 != 0 && value % 5 != 0 && prime(value)))
        {
            counts[mask] += 1;
        }
        for digit in 1..=9u64 {
            let bit = 1usize << (digit - 1);
            if mask & bit == 0 {
                enumerate(
                    mask | bit,
                    value * 10 + digit,
                    digit_sum + digit as u32,
                    counts,
                );
            }
        }
    }
    fn partitions(mask: usize, counts: &[u64; 512], cache: &mut [Option<u64>; 512]) -> u64 {
        if mask == 0 {
            return 1;
        }
        if let Some(v) = cache[mask] {
            return v;
        }
        let lowest = mask & (!mask + 1);
        let mut subtotal = 0;
        let mut subset = mask;
        while subset > 0 {
            if subset & lowest != 0 && counts[subset] > 0 {
                subtotal += counts[subset] * partitions(mask ^ subset, counts, cache);
            }
            subset = (subset - 1) & mask;
        }
        cache[mask] = Some(subtotal);
        subtotal
    }
    let mut counts = [0u64; 512];
    enumerate(0, 0, 0, &mut counts);
    partitions(511, &counts, &mut [None; 512]) as i64
}

fn main() {
    println!("{}", solve());
}
