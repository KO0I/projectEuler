// Project Euler 206: concealed square.
// The suffix forces roots ending 30 or 70.  Check the prescribed alternate
// decimal digits from right to left for only those two residue classes.
fn pattern(mut s: u64) -> bool {
    for expected in [0, 9, 8, 7, 6, 5, 4, 3, 2, 1] {
        if s % 10 != expected {
            return false;
        }
        s /= 100;
    }
    true
}
fn solve() -> u64 {
    let mut r = 1_010_101_030;
    while r <= 1_389_026_630 {
        if pattern(r * r) {
            return r;
        }
        r += if r % 100 == 30 { 40 } else { 60 };
    }
    panic!("no root")
}
fn main() {
    println!("{}", solve());
}
