// Project Euler 237: tours on a 4xn board.
// Frontier connectivity yields a fourth-order recurrence, exponentiated as a
// 4x4 companion matrix modulo 10^8 for n=10^12.
const M: u64 = 100_000_000;
type Mat = [[u64; 4]; 4];
fn mul(a: Mat, b: Mat) -> Mat {
    let mut c = [[0; 4]; 4];
    for i in 0..4 {
        for j in 0..4 {
            for k in 0..4 {
                c[i][j] = (c[i][j] + a[i][k] * b[k][j]) % M;
            }
        }
    }
    c
}
fn power(mut a: Mat, mut e: u64) -> Mat {
    let mut r = [[0; 4]; 4];
    for i in 0..4 {
        r[i][i] = 1;
    }
    while e > 0 {
        if e & 1 == 1 {
            r = mul(r, a);
        }
        a = mul(a, a);
        e >>= 1;
    }
    r
}
fn solve() -> u64 {
    let mut t = [[0; 4]; 4];
    t[0][1] = 1;
    t[1][2] = 1;
    t[2][3] = 1;
    t[3] = [1, M - 2, 2, 2];
    let j = power(t, 1_000_000_000_000 - 4);
    [1, 1, 4, 8]
        .iter()
        .enumerate()
        .map(|(i, &v)| j[3][i] * v)
        .sum::<u64>()
        % M
}
fn main() {
    println!("{}", solve());
}
