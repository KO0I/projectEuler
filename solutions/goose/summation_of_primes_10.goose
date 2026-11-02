// Project Euler 10: Summation of Primes.
import euler from "../../support/goose";

fn solve() -> i64 {
    let a = sieve(1999999);
    var s = 0;
    for i in 2..a.len {
        if a[i] {
            s += i;
        }
    }
    return s;
}

fn main() {
    print(solve());
}
