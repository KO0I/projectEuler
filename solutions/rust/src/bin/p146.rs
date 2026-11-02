//! Small-prime filters precede deterministic u64 primality tests.
use project_euler::extra_helpers::prime64;
fn solve() -> u64 {
    let offsets = [1, 3, 7, 9, 13, 27];
    let filters = [
        3, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47, 53, 59, 61, 67, 71, 73, 79, 83, 89, 97,
    ];
    let mut sum = 0;
    for n in (10u64..150_000_000).step_by(10) {
        let s = n * n;
        if filters
            .iter()
            .any(|&p| offsets.iter().any(|&o| (s + o) % p == 0))
        {
            continue;
        }
        if offsets.iter().all(|&o| prime64(s + o)) && !prime64(s + 19) && !prime64(s + 21) {
            sum += n
        }
    }
    sum
}
fn main() {
    println!("{}", solve());
}
