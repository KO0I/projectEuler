// Project Euler 225: Tribonacci non-divisors.
// The modular transition is invertible, so the sequence is purely periodic:
// either zero occurs or state (1,1,1) returns first.
fn never(m: u32) -> bool {
    let (mut a, mut b, mut c) = (1, 1, 1);
    loop {
        let n = (a + b + c) % m;
        (a, b, c) = (b, c, n);
        if c == 0 {
            return false;
        }
        if (a, b, c) == (1, 1, 1) {
            return true;
        }
    }
}
fn solve() -> u32 {
    let mut found = 0;
    for m in (1..).step_by(2) {
        if never(m) {
            found += 1;
            if found == 124 {
                return m;
            }
        }
    }
    unreachable!()
}
fn main() {
    println!("{}", solve());
}
