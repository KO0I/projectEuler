// Project Euler 242: odd triplets.
// Lucas' theorem reduces the count to sum 2^popcount(t); binary digit DP uses
// 3^i for i free lower bits whenever a bound bit is dropped to zero.
fn solve() -> u64 {
    let upper = (1_000_000_000_000u64 - 1) / 4;
    let mut p3 = [1u64; 40];
    for i in 1..40 {
        p3[i] = p3[i - 1] * 3;
    }
    let (mut answer, mut weight) = (0, 1);
    for bit in (0..40).rev() {
        if upper >> bit & 1 == 1 {
            answer += weight * p3[bit];
            weight *= 2;
        }
    }
    answer + weight
}
fn main() {
    println!("{}", solve());
}
