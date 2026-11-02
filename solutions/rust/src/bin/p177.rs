//! Match sine-law ratios and canonicalize rotations and reflections.
use std::collections::HashSet;
fn canon(x: [u8; 8]) -> [u8; 8] {
    let mut best = x;
    let mut rev = x;
    rev.reverse();
    for src in [x, rev] {
        for r in (0..8).step_by(2) {
            let mut c = [0; 8];
            for i in 0..8 {
                c[i] = src[(i + r) & 7]
            }
            best = best.min(c)
        }
    }
    best
}
fn solve() -> usize {
    let mut log = [0.0f64; 180];
    for a in 1..180 {
        log[a] = ((a as f64).to_radians().sin()).ln()
    }
    let mut shapes = HashSet::new();
    for u in 2..=178 {
        let v = 180 - u;
        let mut table = Vec::new();
        for a in 1..v {
            for c in 1..u {
                table.push((log[u - c] + log[a] - log[c] - log[v - a], a, c))
            }
        }
        table.sort_by(|a, b| a.0.total_cmp(&b.0));
        for p in 1..u {
            let q = u - p;
            for r in 1..v {
                let s = v - r;
                let target = log[p] + log[r] - log[q] - log[s];
                let mut i = table.partition_point(|x| x.0 < target - 1e-11);
                while i < table.len() && table[i].0 <= target + 1e-11 {
                    let (t, a, c) = table[i];
                    if (t - target).abs() <= 1e-12 {
                        shapes.insert(canon([
                            a as u8,
                            p as u8,
                            q as u8,
                            r as u8,
                            s as u8,
                            c as u8,
                            (u - c) as u8,
                            (v - a) as u8,
                        ]));
                    }
                    i += 1
                }
            }
        }
    }
    shapes.len()
}
fn main() {
    println!("{}", solve());
}
