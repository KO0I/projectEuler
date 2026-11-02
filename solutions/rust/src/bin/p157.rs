//! Count divisor choices after separating powers of two and five.
use project_euler::extra_helpers::divisor_count;
fn solve() -> u64 {
    let (mut answer, mut ten) = (0, 1u64);
    for n in 1..=9 {
        ten *= 10;
        let mut two = 1;
        for a in 0..=n {
            let mut five = 1;
            for b in 0..=n {
                let y = two * five;
                answer += divisor_count((ten / y) * (y + 1)) as u64;
                if a > 0 && b > 0 {
                    answer += divisor_count((ten / y) * (two + five)) as u64
                }
                five *= 5
            }
            two *= 2
        }
    }
    answer
}
fn main() {
    println!("{}", solve());
}
