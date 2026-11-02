// Project Euler 35: Circular Primes.
import euler from "../../support/goose";

fn solve() -> i64 {
    let p = sieve(999999);
    var count = 0;
    for n in 2..1000000 {
        if !p[n] {
            continue;
        }
        var r = n;
        let k = ipow(10, digits(n) - 1);
        var ok = true;
        for _ in digits(n) - 1 {
            r = r / 10 + r % 10 * k;
            if !p[r] {
                ok = false;
                break;
            }
        }
        if ok {
            count++;
        }
    }
    return count;
}

fn main() {
    print(solve());
}
