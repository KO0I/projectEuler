// Project Euler 243: resilience.
// Build the first primorial whose phi(n)/n crosses the target, then inspect
// its increasing multiples with exact cross-products.
fn phi(n: u64) -> u64 {
    let (mut result, mut rem) = (n, n);
    let mut p = 2;
    while p <= rem / p {
        if rem % p == 0 {
            result -= result / p;
            while rem % p == 0 {
                rem /= p;
            }
        }
        p += 1;
    }
    if rem > 1 {
        result -= result / rem;
    }
    result
}
fn solve() -> u64 {
    let (mut base, mut ph) = (1, 1);
    for p in [2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37] {
        base *= p;
        ph *= p - 1;
        if ph * 94744 < base * 15499 {
            break;
        }
    }
    for m in 1.. {
        let d = base * m;
        if phi(d) * 94744 < (d - 1) * 15499 {
            return d;
        }
    }
    unreachable!()
}
fn main() {
    println!("{}", solve());
}
