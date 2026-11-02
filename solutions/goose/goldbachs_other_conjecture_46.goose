// Project Euler 46: Goldbach's Other Conjecture.
import euler from "../../support/goose";

fn solve() -> i64 {
    var n = 9;
    loop {
        if !prime(n) {
            var ok = false;
            var s = 1;
            while 2 * s * s < n {
                if prime(n - 2 * s * s) {
                    ok = true;
                    break;
                }
                s++;
            }
            if !ok {
                return n;
            }
        }
        n += 2;
    }
}

fn main() {
    print(solve());
}
