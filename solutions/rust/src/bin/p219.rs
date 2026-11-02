// Project Euler 219: skew-cost coding.
// Expanding the cheapest leaf is optimal.  Node multiplicities by cost obey
// f(c)=f(c-1)+f(c-4), avoiding a billion-element priority queue.
fn solve() -> u64 {
    let needed = 999_999_999u64;
    let mut f = [0u64; 128];
    f[0] = 1;
    let (mut used, mut cost) = (0, 0);
    for c in 0..128 {
        if c > 0 {
            f[c] = f[c - 1] + if c >= 4 { f[c - 4] } else { 0 };
        }
        let take = f[c].min(needed - used);
        used += take;
        cost += c as u64 * take;
        if used == needed {
            break;
        }
    }
    cost + 5 * needed
}
fn main() {
    println!("{}", solve());
}
