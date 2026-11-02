//! Quotient-compressed subtractive Euclid gives the reversed run lengths.
fn solve() -> String {
    let (mut a, mut b) = (123456789u64, 987654321u64);
    let mut runs = Vec::new();
    while a != b {
        if a < b {
            let q = (b - 1) / a;
            runs.push(q);
            b -= q * a
        } else {
            let q = (a - 1) / b;
            runs.push(q);
            a -= q * b
        }
    }
    runs.push(1);
    runs.into_iter()
        .rev()
        .map(|x| x.to_string())
        .collect::<Vec<_>>()
        .join(",")
}
fn main() {
    println!("{}", solve());
}
