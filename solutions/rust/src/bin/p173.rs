//! Enumerate outer and same-parity inner square side lengths.
fn solve() -> u64 {
    let mut z = 0;
    for outer in 3u64.. {
        if 4 * (outer - 1) > 1_000_000 {
            break;
        }
        let mut inner = outer - 2;
        loop {
            if outer * outer - inner * inner > 1_000_000 {
                break;
            }
            z += 1;
            if inner <= 2 {
                break;
            }
            inner -= 2
        }
    }
    z
}
fn main() {
    println!("{}", solve());
}
