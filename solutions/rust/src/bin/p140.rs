//! Generate the six Pell orbits of x^2-5y^2=44 and take 30 unique nuggets.
fn solve() -> u64 {
    let seeds = [(7i128, 1i128), (7, -1), (8, 2), (8, -2), (13, 5), (13, -5)];
    let mut v = Vec::new();
    for (mut x, mut y) in seeds {
        for _ in 0..35 {
            if x > 7 && (x - 7) % 5 == 0 {
                v.push(((x - 7) / 5) as u64)
            }
            (x, y) = (9 * x + 20 * y, 4 * x + 9 * y);
        }
    }
    v.sort_unstable();
    v.dedup();
    v.into_iter().take(30).sum()
}
fn main() {
    println!("{}", solve());
}
