//! Closed forms from paired-digit carry analysis by decimal length.
fn solve() -> u64 {
    let (mut total, mut even) = (0, 20);
    for _ in (2..=8).step_by(2) {
        total += even;
        even *= 30
    }
    let mut odd = 100;
    for _ in (3..=7).step_by(4) {
        total += odd;
        odd *= 500
    }
    total
}
fn main() {
    println!("{}", solve());
}
