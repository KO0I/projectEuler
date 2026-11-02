//! Project Euler 102: Triangle Containment.

fn solve() -> i64 {
    include_str!("../../../../tests/data/p102_triangles.txt")
        .lines()
        .filter(|line| {
            let p: Vec<i64> = line.split(',').map(|x| x.parse().expect("coord")).collect();
            assert_eq!(p.len(), 6);
            let a = p[0] * p[3] - p[1] * p[2];
            let b = p[2] * p[5] - p[3] * p[4];
            let c = p[4] * p[1] - p[5] * p[0];
            (a > 0 && b > 0 && c > 0) || (a < 0 && b < 0 && c < 0)
        })
        .count() as i64
}

fn main() {
    println!("{}", solve());
}
