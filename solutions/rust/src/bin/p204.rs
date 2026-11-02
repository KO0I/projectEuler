// Project Euler 204: generalized Hamming numbers.
// Choosing each prime's exponent before recursing gives every number whose
// prime factors are <=100 exactly once.
const PS: [u64; 25] = [
    2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47, 53, 59, 61, 67, 71, 73, 79, 83, 89, 97,
];
fn count(i: usize, mut value: u64) -> u64 {
    if i == PS.len() {
        return 1;
    }
    let mut result = 0;
    loop {
        result += count(i + 1, value);
        if value > 1_000_000_000 / PS[i] {
            break;
        }
        value *= PS[i];
    }
    result
}
fn solve() -> u64 {
    count(0, 1)
}
fn main() {
    println!("{}", solve());
}
