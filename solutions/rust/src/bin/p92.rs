//! Project Euler 92: Square Digit Chains.
use project_euler::extra_helpers::digit_power_sum;

fn solve() -> i64 {
    {
        let mut cache = [0u8; 568];
        cache[1] = 1;
        cache[89] = 89;
        let squares = [0u32, 1, 4, 9, 16, 25, 36, 49, 64, 81];
        let mut count = 0;
        for n in 1u32..10_000_000 {
            let mut v = digit_power_sum(n, &squares);
            while cache[v as usize] == 0 {
                v = digit_power_sum(v, &squares);
            }
            if cache[v as usize] == 89 {
                count += 1;
            }
        }
        count
    }
}

fn main() {
    println!("{}", solve());
}
