// Project Euler 252: convex holes.
// For each canonical lowest anchor, polar-sort later vertices.  Bitsets test
// empty fan triangles, and a convex-chain DP maximizes doubled polygon area.
#[derive(Clone, Copy)]
struct P {
    x: i32,
    y: i32,
}
const N: usize = 500;
const W: usize = 8;
fn cross(ps: &[P], a: usize, b: usize, c: usize) -> i64 {
    (ps[b].x - ps[a].x) as i64 * (ps[c].y - ps[a].y) as i64
        - (ps[b].y - ps[a].y) as i64 * (ps[c].x - ps[a].x) as i64
}
fn solve() -> String {
    let mut state = 290797u64;
    let mut ps = Vec::new();
    for _ in 0..N {
        state = state * state % 50_515_093;
        let x = (state % 2000) as i32 - 1000;
        state = state * state % 50_515_093;
        let y = (state % 2000) as i32 - 1000;
        ps.push(P { x, y });
    }
    let mut left = vec![0u64; N * N * W];
    let mut inner = vec![false; N * N];
    for a in 0..N {
        for b in 0..N {
            if a == b {
                continue;
            }
            for p in 0..N {
                if p == a || p == b {
                    continue;
                }
                let c = cross(&ps, a, b, p);
                if c > 0 {
                    left[(a * N + b) * W + (p >> 6)] |= 1u64 << (p & 63);
                } else if c == 0 {
                    let dot = (ps[p].x - ps[a].x) as i64 * (ps[p].x - ps[b].x) as i64
                        + (ps[p].y - ps[a].y) as i64 * (ps[p].y - ps[b].y) as i64;
                    if dot < 0 {
                        inner[a * N + b] = true;
                    }
                }
            }
        }
    }
    let empty = |a: usize, b: usize, c: usize| -> bool {
        (0..W).all(|w| {
            left[(a * N + b) * W + w] & left[(b * N + c) * W + w] & left[(c * N + a) * W + w] == 0
        })
    };
    let mut global = 0;
    for anchor in 0..N {
        let mut vs: Vec<_> = (0..N)
            .filter(|&v| {
                ps[v].y > ps[anchor].y || (ps[v].y == ps[anchor].y && ps[v].x > ps[anchor].x)
            })
            .collect();
        if vs.len() < 2 {
            continue;
        }
        vs.sort_unstable_by(|&a, &b| {
            let c = cross(&ps, anchor, a, b);
            if c != 0 {
                return c.cmp(&0).reverse();
            }
            let dist = |v: usize| {
                (ps[v].x - ps[anchor].x) as i64 * (ps[v].x - ps[anchor].x) as i64
                    + (ps[v].y - ps[anchor].y) as i64 * (ps[v].y - ps[anchor].y) as i64
            };
            dist(a).cmp(&dist(b))
        });
        let m = vs.len();
        let mut dp = vec![0i64; m * m];
        for k in 1..m {
            for j in 0..k {
                let area = cross(&ps, anchor, vs[j], vs[k]);
                if area <= 0 || !empty(anchor, vs[j], vs[k]) {
                    continue;
                }
                let mut best = area;
                if !inner[anchor * N + vs[j]] {
                    for h in 0..j {
                        let previous = dp[h * m + j];
                        if previous > 0 && cross(&ps, vs[h], vs[j], vs[k]) > 0 {
                            best = best.max(previous + area);
                        }
                    }
                }
                dp[j * m + k] = best;
                global = global.max(best);
            }
        }
    }
    format!("{}.{}", global / 2, if global & 1 == 1 { 5 } else { 0 })
}
fn main() {
    println!("{}", solve());
}
