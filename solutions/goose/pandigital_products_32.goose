// Project Euler 32: Pandigital Products.
import euler from "../../support/goose";

fn solve() -> i64 {
    var seen = [false; 10000];
    var s = 0;
    for a in 2..100 {
        for b in 100..10000 {
            let c = a * b;
            if c > 9999 {
                break;
            }
            let combined = (a * ipow(10, digits(b)) + b) * 10000 + c;
            if pandigital(combined) && !seen[c] {
                seen[c] = true;
                s += c;
            }
        }
    }
    return s;
}

fn main() {
    print(solve());
}
