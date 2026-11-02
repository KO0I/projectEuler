//! Enumerate only the congruent v values and saturate counts at two.
fn solve() -> usize {
    const L: usize = 50_000_000;
    let mut count = vec![0u8; L];
    let mut u = 1;
    while u * (u / 3 + 1) < L {
        let mut v = u / 3 + 1;
        v += (4 - (u + v) % 4) % 4;
        while u * v < L {
            let n = u * v;
            count[n] = (count[n] + 1).min(2);
            v += 4
        }
        u += 1
    }
    count.into_iter().filter(|&x| x == 1).count()
}
fn main() {
    println!("{}", solve());
}
