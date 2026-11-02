//! Minimize a nonincreasing prime-exponent vector with the required divisor product.
fn go(pos: usize, maxe: u32, remain: u64, value: u64, best: &mut u64) {
    const P: [u64; 10] = [2, 3, 5, 7, 11, 13, 17, 19, 23, 29];
    if remain == 1 {
        *best = (*best).min(value);
        return;
    }
    if pos == P.len() {
        return;
    }
    let mut power = 1u64;
    for e in 1..=maxe {
        let Some(v) = power.checked_mul(P[pos]) else {
            break;
        };
        power = v;
        if value > *best / power {
            break;
        }
        let f = 2 * e as u64 + 1;
        if remain % f == 0 {
            go(pos + 1, e, remain / f, value * power, best)
        }
    }
}
fn solve() -> u64 {
    let mut b = u64::MAX;
    go(0, 31, 2 * 47547 + 1, 1, &mut b);
    2 * b
}
fn main() {
    println!("{}", solve());
}
