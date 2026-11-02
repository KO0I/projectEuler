//! Lucas' theorem reduces the row count to base-seven digit products.
fn solve() -> u64 {
    let mut value = 1_000_000_000u64;
    let mut digits = Vec::new();
    while value > 0 {
        digits.push(value % 7);
        value /= 7
    }
    let mut powers = vec![1u64; digits.len() + 1];
    for i in 1..powers.len() {
        powers[i] = powers[i - 1] * 28
    }
    let (mut total, mut prefix) = (0, 1);
    for pos in (0..digits.len()).rev() {
        let d = digits[pos];
        total += prefix * d * (d + 1) / 2 * powers[pos];
        prefix *= d + 1
    }
    total
}
fn main() {
    println!("{}", solve());
}
