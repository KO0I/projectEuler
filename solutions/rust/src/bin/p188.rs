// Project Euler 188: modular hyperexponentiation.
// Since 1777 is coprime to every modulus in the totient chain, Euler's
// theorem reduces each exponent before the next modular power.
fn phi(mut n: u64) -> u64 {
    let mut result = n;
    let mut p = 2;
    while p * p <= n {
        if n % p == 0 {
            while n % p == 0 {
                n /= p;
            }
            result -= result / p;
        }
        p += 1;
    }
    if n > 1 {
        result -= result / n;
    }
    result
}
fn powmod(mut b: u64, mut e: u64, m: u64) -> u64 {
    let mut r = 1;
    while e > 0 {
        if e & 1 == 1 {
            r = r * b % m;
        }
        b = b * b % m;
        e >>= 1;
    }
    r
}
fn tower(height: u32, modulus: u64) -> u64 {
    if modulus == 1 {
        0
    } else if height == 1 {
        1777 % modulus
    } else {
        powmod(1777, tower(height - 1, phi(modulus)), modulus)
    }
}
fn solve() -> u64 {
    tower(1855, 100_000_000)
}
fn main() {
    println!("{:08}", solve());
}
