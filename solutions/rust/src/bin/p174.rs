//! Count lamina representations per tile total, saturating above ten.
fn solve() -> usize {
    let mut c = vec![0u8; 1_000_001];
    for o in 3usize.. {
        if 4 * (o - 1) > 1_000_000 {
            break;
        }
        let mut i = o - 2;
        loop {
            let t = o * o - i * i;
            if t > 1_000_000 {
                break;
            }
            c[t] = (c[t] + 1).min(11);
            if i <= 2 {
                break;
            }
            i -= 2
        }
    }
    c.into_iter().filter(|&x| (1..=10).contains(&x)).count()
}
fn main() {
    println!("{}", solve());
}
