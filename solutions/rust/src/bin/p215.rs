// Project Euler 215: crack-free walls.
// Row joints are bit masks.  Adjacent rows are compatible precisely when the
// masks are disjoint; a height DP composes ten such rows.
fn rows(pos: u32, mask: u32, out: &mut Vec<u32>) {
    if pos == 32 {
        out.push(mask);
        return;
    }
    for brick in [2, 3] {
        let end = pos + brick;
        if end <= 32 {
            rows(end, if end == 32 { mask } else { mask | 1 << end }, out);
        }
    }
}
fn solve() -> u64 {
    let mut masks = Vec::new();
    rows(0, 0, &mut masks);
    let mut ways = vec![1u64; masks.len()];
    for _ in 1..10 {
        ways = (0..masks.len())
            .map(|i| {
                (0..masks.len())
                    .filter(|&j| masks[i] & masks[j] == 0)
                    .map(|j| ways[j])
                    .sum()
            })
            .collect();
    }
    ways.iter().sum()
}
fn main() {
    println!("{}", solve());
}
