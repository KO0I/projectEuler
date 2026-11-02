// Project Euler 218: perfect right triangles.
// This computes a modular proof rather than embedding zero: every admissible
// residue pair (u,v) mod 84 makes the Euclidean area divisible by 84.
fn solve() -> u32 {
    let mut bad = 0;
    for u in 0i64..84 {
        for v in 0i64..84 {
            if (u ^ v) & 1 == 0 || (u % 3 == 0 && v % 3 == 0) || (u % 7 == 0 && v % 7 == 0) {
                continue;
            }
            let (m, n) = (u * u - v * v, 2 * u * v);
            if (m * n % 84 * (m * m - n * n) % 84).rem_euclid(84) != 0 {
                bad += 1;
            }
        }
    }
    bad
}
fn main() {
    println!("{}", solve());
}
