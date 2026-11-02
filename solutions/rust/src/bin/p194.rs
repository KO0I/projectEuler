// Project Euler 194: coloured configurations.
// Fixed-boundary chromatic polynomials count extensions of the two unit
// graphs.  Pascal recurrence supplies C(100,25) modulo the composite 10^8.
const M: u64 = 100_000_000;
fn powmod(mut b: u64, mut e: u32) -> u64 {
    let mut r = 1;
    while e > 0 {
        if e & 1 == 1 {
            r = r * b % M;
        }
        b = b * b % M;
        e >>= 1;
    }
    r
}
fn unit_a(c: i64) -> u64 {
    (((((c - 9) * c + 34) * c - 69) * c + 77) * c - 38) as u64
}
fn unit_b(c: i64) -> u64 {
    (((((c - 8) * c + 27) * c - 50) * c + 52) * c - 24) as u64
}
fn solve() -> u64 {
    let mut row = [0u64; 26];
    row[0] = 1;
    for i in 1..=100 {
        for j in (1..=i.min(25)).rev() {
            row[j] = (row[j] + row[j - 1]) % M;
        }
    }
    let c = 1984u64;
    row[25] * (c * (c - 1) % M) % M * powmod(unit_a(c as i64) % M, 25) % M
        * powmod(unit_b(c as i64) % M, 75)
        % M
}
fn main() {
    println!("{:08}", solve());
}
