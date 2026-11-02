//! Descending pandigital results make the first valid factorization maximal.
use project_euler::extra_helpers::gcd;
fn append(mut n: u64, mask: &mut u16, len: &mut usize) -> bool {
    let mut d = Vec::new();
    loop {
        d.push((n % 10) as u8);
        n /= 10;
        if n == 0 {
            break;
        }
    }
    for x in d.into_iter().rev() {
        let b = 1 << x;
        if *mask & b != 0 {
            return false;
        }
        *mask |= b;
        *len += 1
    }
    true
}
fn works(f: u64, a: u64, b: u64) -> bool {
    if f <= 1 || a % f != 0 || b % f != 0 {
        return false;
    }
    let (mut m, mut l) = (0, 0);
    append(f, &mut m, &mut l)
        && append(a / f, &mut m, &mut l)
        && append(b / f, &mut m, &mut l)
        && l == 10
        && m == 1023
}
fn inspect(d: &[u8; 10]) -> bool {
    let whole = d.iter().fold(0u64, |v, &x| 10 * v + x as u64);
    let mut left = 0;
    for cut in 1..10 {
        left = 10 * left + d[cut - 1] as u64;
        if d[cut] == 0 {
            continue;
        }
        let right = whole % 10u64.pow((10 - cut) as u32);
        let g = gcd(left, right);
        let mut q = 2;
        while q * q <= g {
            if g % q == 0 && (works(q, left, right) || works(g / q, left, right)) {
                return true;
            }
            q += 1
        }
        if works(g, left, right) {
            return true;
        }
    }
    false
}
fn search(pos: usize, used: u16, d: &mut [u8; 10]) -> Option<u64> {
    if pos == 10 {
        return inspect(d).then(|| d.iter().fold(0, |v, &x| 10 * v + x as u64));
    }
    for x in (0..=9).rev() {
        if used & (1 << x) != 0 || (pos == 0 && x == 0) {
            continue;
        }
        d[pos] = x;
        if let Some(v) = search(pos + 1, used | (1 << x), d) {
            return Some(v);
        }
    }
    None
}
fn solve() -> u64 {
    search(0, 0, &mut [0; 10]).unwrap()
}
fn main() {
    println!("{}", solve());
}
