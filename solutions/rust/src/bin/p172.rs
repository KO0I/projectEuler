//! Enumerate digit multiplicities and count permutations, subtracting leading-zero cases.
fn fact(n: u64) -> u64 {
    (1..=n).product()
}
fn rec(d: usize, left: u8, c: &mut [u8; 10], ans: &mut u64) {
    if d == 10 {
        if left != 0 {
            return;
        }
        let den: u64 = c.iter().map(|&x| fact(x as u64)).product();
        let mut ways = fact(18) / den;
        if c[0] > 0 {
            let mut d0 = fact((c[0] - 1) as u64);
            for &x in &c[1..] {
                d0 *= fact(x as u64)
            }
            ways -= fact(17) / d0
        }
        *ans += ways;
        return;
    }
    for x in 0..=3.min(left) {
        c[d] = x;
        rec(d + 1, left - x, c, ans)
    }
}
fn solve() -> u64 {
    let mut a = 0;
    rec(0, 18, &mut [0; 10], &mut a);
    a
}
fn main() {
    println!("{}", solve());
}
