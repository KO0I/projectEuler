//! Exact rational intersections are reduced and deduplicated in a hash set.
use std::collections::HashSet;
fn gcd(mut a: i64, mut b: i64) -> i64 {
    a = a.abs();
    while b != 0 {
        (a, b) = (b, a % b)
    }
    a
}
fn solve() -> usize {
    let mut seg = Vec::with_capacity(5000);
    let mut s = 290797u64;
    for _ in 0..5000 {
        let mut p = [0i64; 4];
        for x in &mut p {
            s = s * s % 50515093;
            *x = (s % 500) as i64
        }
        seg.push(p)
    }
    let mut points = HashSet::new();
    for i in 0..5000 {
        let a = seg[i];
        let (rx, ry) = (a[2] - a[0], a[3] - a[1]);
        for b in &seg[i + 1..] {
            let (sx, sy, qx, qy) = (b[2] - b[0], b[3] - b[1], b[0] - a[0], b[1] - a[1]);
            let (mut den, mut tn, mut un) =
                (rx * sy - ry * sx, qx * sy - qy * sx, qx * ry - qy * rx);
            if den < 0 {
                den = -den;
                tn = -tn;
                un = -un
            }
            if tn <= 0 || tn >= den || un <= 0 || un >= den {
                continue;
            }
            let (xn, yn) = (a[0] * den + rx * tn, a[1] * den + ry * tn);
            let (xg, yg) = (gcd(xn, den), gcd(yn, den));
            points.insert((xn / xg, den / xg, yn / yg, den / yg));
        }
    }
    points.len()
}
fn main() {
    println!("{}", solve());
}
