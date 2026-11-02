// Project Euler 184: lattice triangles containing the origin.
//
// Sort all points by exact polar angle.  A triangle misses the origin iff its
// vertices fit in one open semicircle; a two-pointer window counts these.
// Triples containing an antipodal pair put the origin on an edge and are
// removed separately by grouping points by primitive unoriented direction.
use std::cmp::Ordering;

#[derive(Clone, Copy)]
struct Point {
    x: i32,
    y: i32,
}
fn half(p: Point) -> bool {
    p.y < 0 || (p.y == 0 && p.x < 0)
}
fn gcd(mut a: i32, mut b: i32) -> i32 {
    while b != 0 {
        (a, b) = (b, a % b);
    }
    a
}
fn c2(n: i64) -> i64 {
    n * (n - 1) / 2
}
fn c3(n: i64) -> i64 {
    n * (n - 1) * (n - 2) / 6
}
fn solve() -> i64 {
    const R: i32 = 105;
    let mut points = Vec::new();
    for x in -R + 1..R {
        for y in -R + 1..R {
            if (x != 0 || y != 0) && x * x + y * y < R * R {
                points.push(Point { x, y });
            }
        }
    }
    points.sort_unstable_by(|&a, &b| {
        let ha = half(a);
        let hb = half(b);
        if ha != hb {
            return ha.cmp(&hb);
        }
        let cross = a.x as i64 * b.y as i64 - a.y as i64 * b.x as i64;
        if cross != 0 {
            return if cross > 0 {
                Ordering::Less
            } else {
                Ordering::Greater
            };
        }
        (a.x * a.x + a.y * a.y).cmp(&(b.x * b.x + b.y * b.y))
    });
    let count = points.len();
    let mut outside = 0i64;
    let mut end = 1usize;
    for start in 0..count {
        end = end.max(start + 1);
        while end < start + count {
            let a = points[start];
            let b = points[end % count];
            let cross = a.x as i64 * b.y as i64 - a.y as i64 * b.x as i64;
            let dot = a.x as i64 * b.x as i64 + a.y as i64 * b.y as i64;
            if cross > 0 || (cross == 0 && dot > 0 && end < count) {
                end += 1;
            } else {
                break;
            }
        }
        outside += c2((end - start - 1) as i64);
    }
    let side = 2 * R + 1;
    let cap = (side * side) as usize;
    let (mut positive, mut negative) = (vec![0i64; cap], vec![0i64; cap]);
    for p in &points {
        let g = gcd(p.x.abs(), p.y.abs());
        let (mut x, mut y) = (p.x / g, p.y / g);
        let mut pos = true;
        if y < 0 || (y == 0 && x < 0) {
            x = -x;
            y = -y;
            pos = false;
        }
        let i = ((y + R) * side + (x + R)) as usize;
        if pos {
            positive[i] += 1;
        } else {
            negative[i] += 1;
        }
    }
    let mut boundary = 0i64;
    for (&a, &b) in positive.iter().zip(&negative) {
        if a != 0 && b != 0 {
            boundary += a * b * (count as i64 - a - b) + c2(a) * b + a * c2(b);
        }
    }
    c3(count as i64) - outside - boundary
}
fn main() {
    println!("{}", solve());
}
