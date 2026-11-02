// Project Euler 33: Digit Cancelling Fractions.
import euler from "../../support/goose";

fn solve() -> i64 {
    var num = 1;
    var den = 1;
    for a in 10..100 {
        for b in a + 1..100 {
            if a % 10 == 0 || b % 10 == 0 {
                continue;
            }
            if (a % 10 == b / 10 && a / 10 * b == a * (b % 10)) || (a / 10 == b % 10 && a % 10 * b == a * (b / 10)) {
                num *= a;
                den *= b;
            }
        }
    }
    return den / gcd(num, den);
}

fn main() {
    print(solve());
}
