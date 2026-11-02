// Project Euler 71: Ordered Fractions.
import euler from "../../support/goose";

fn solve() -> i64 {
    var num = 0;
    var den = 1;
    for d in 2..1000001 {
        let n = (3 * d - 1) / 7;
        if n * den > num * d {
            num = n;
            den = d;
        }
    }
    return num / gcd(num, den);
}

fn main() {
    print(solve());
}
