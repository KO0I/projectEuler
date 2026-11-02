//! Generate the grid, then apply Kadane's algorithm to every line direction.
const SIDE: i32 = 2000;
fn line(g: &[i32], mut r: i32, mut c: i32, dr: i32, dc: i32) -> i64 {
    let (mut best, mut cur) = (i64::MIN, 0);
    while (0..SIDE).contains(&r) && (0..SIDE).contains(&c) {
        cur += g[(r * SIDE + c) as usize] as i64;
        best = best.max(cur);
        cur = cur.max(0);
        r += dr;
        c += dc
    }
    best
}
fn solve() -> i64 {
    let size = (SIDE * SIDE) as usize;
    let mut g = vec![0i32; size];
    for k in 1..=size {
        let mut v = if k <= 55 {
            (100003 - 200003 * k as i64 + 300007 * (k as i64).pow(3)) % 1_000_000
        } else {
            (g[k - 25] as i64 + g[k - 56] as i64 + 1_000_000) % 1_000_000
        };
        if v < 0 {
            v += 1_000_000
        }
        g[k - 1] = (v - 500_000) as i32
    }
    let mut best = i64::MIN;
    for i in 0..SIDE {
        for v in [
            line(&g, i, 0, 0, 1),
            line(&g, 0, i, 1, 0),
            line(&g, i, 0, 1, 1),
            line(&g, 0, i, 1, 1),
            line(&g, i, 0, -1, 1),
            line(&g, SIDE - 1, i, -1, 1),
        ] {
            best = best.max(v)
        }
    }
    best
}
fn main() {
    println!("{}", solve());
}
