//! Row prefixes make every widening layer of a downward triangle O(1).
fn solve() -> i64 {
    const R: usize = 1000;
    let mut rows = Vec::with_capacity(R);
    let mut t = 0i64;
    for r in 0..R {
        let mut p = vec![0i64; r + 2];
        for c in 0..=r {
            t = (615949 * t + 797807) % (1 << 20);
            p[c + 1] = p[c] + t - (1 << 19)
        }
        rows.push(p)
    }
    let mut best = i64::MAX;
    for top in 0..R {
        for col in 0..=top {
            let mut sum = 0;
            for bottom in top..R {
                sum += rows[bottom][col + bottom - top + 1] - rows[bottom][col];
                best = best.min(sum)
            }
        }
    }
    best
}
fn main() {
    println!("{}", solve());
}
