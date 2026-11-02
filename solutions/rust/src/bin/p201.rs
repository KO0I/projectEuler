// Project Euler 201: subsets with a unique sum.
// ways[k,sum] is saturated at two because only impossible, unique, and
// non-unique matter.  Descending loops prevent reusing the current square.
fn solve() -> u64 {
    const CHOOSE: usize = 50;
    const MAX: usize = 338350;
    let width = MAX + 1;
    let mut ways = vec![0u8; (CHOOSE + 1) * width];
    ways[0] = 1;
    let mut reachable = 0;
    for n in 1..=100usize {
        let sq = n * n;
        reachable += sq;
        for k in (1..=n.min(CHOOSE)).rev() {
            for s in (sq..=reachable).rev() {
                let i = k * width + s;
                ways[i] = (ways[i] + ways[(k - 1) * width + s - sq]).min(2);
            }
        }
    }
    (0..=MAX)
        .filter(|&s| ways[CHOOSE * width + s] == 1)
        .map(|s| s as u64)
        .sum()
}
fn main() {
    println!("{}", solve());
}
