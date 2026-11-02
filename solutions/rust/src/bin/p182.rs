// Project Euler 182: RSA unconcealed messages.
// A valid exponent e fixes (1+gcd(e-1,p-1))(1+gcd(e-1,q-1)) messages by CRT.
fn gcd(mut a: u64, mut b: u64) -> u64 {
    while b != 0 {
        (a, b) = (b, a % b);
    }
    a
}
fn solve() -> u64 {
    let (p, q) = (1009u64, 3643u64);
    let phi = (p - 1) * (q - 1);
    let (mut minimum, mut answer) = (u64::MAX, 0);
    for e in 2..phi {
        if gcd(e, phi) != 1 {
            continue;
        }
        let fixed = (gcd(e - 1, p - 1) + 1) * (gcd(e - 1, q - 1) + 1);
        if fixed < minimum {
            minimum = fixed;
            answer = e;
        } else if fixed == minimum {
            answer += e;
        }
    }
    answer
}
fn main() {
    println!("{}", solve());
}
