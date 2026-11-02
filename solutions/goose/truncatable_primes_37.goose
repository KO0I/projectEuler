// Project Euler 37: Truncatable Primes.
import euler from "../../support/goose";

fn solve() -> i64 {
    var count = 0;
    var sum = 0;
    var n = 11;
    while count < 11 {
        if prime(n) {
            var r = n / 10;
            var ok = true;
            while r > 0 {
                if !prime(r) {
                    ok = false;
                }
                r /= 10;
            }
            var p = 10;
            while p < n {
                if !prime(n % p) {
                    ok = false;
                }
                p *= 10;
            }
            if ok {
                count++;
                sum += n;
            }
        }
        n += 2;
    }
    return sum;
}

fn main() {
    print(solve());
}
