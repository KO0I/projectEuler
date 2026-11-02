// Project Euler 202: laserbeam paths.
// Unfolding reduces the geometry to 1<=s<n, 2s=n (mod 3), gcd(s,n)=1.
// Inclusion-exclusion over distinct prime factors counts this in O(2^omega).
fn residue_count(max: u64, r: u64) -> u64 {
    if r == 0 {
        max / 3
    } else if max < r {
        0
    } else {
        (max - r) / 3 + 1
    }
}
fn multiples(n: u64, d: u64, r: u64) -> u64 {
    let max = (n - 1) / d;
    match d % 3 {
        0 => {
            if r == 0 {
                max
            } else {
                0
            }
        }
        1 => residue_count(max, r),
        _ => residue_count(max, 2 * r % 3),
    }
}
fn solve() -> u64 {
    let n = (12_017_639_147u64 + 3) / 2;
    let mut rem = n;
    let mut fs = Vec::new();
    let mut p = 2;
    while p <= rem / p {
        if rem % p == 0 {
            fs.push(p);
            while rem % p == 0 {
                rem /= p;
            }
        }
        p += if p == 2 { 1 } else { 2 };
    }
    if rem > 1 {
        fs.push(rem);
    }
    let target = 2 * (n % 3) % 3;
    let mut total = 0i64;
    for mask in 0..1usize << fs.len() {
        let mut d = 1;
        let mut odd = false;
        for (i, &p) in fs.iter().enumerate() {
            if mask >> i & 1 == 1 {
                d *= p;
                odd = !odd;
            }
        }
        let c = multiples(n, d, target) as i64;
        total += if odd { -c } else { c };
    }
    total as u64
}
fn main() {
    println!("{}", solve());
}
