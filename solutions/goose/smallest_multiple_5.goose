// Project Euler 5: Smallest Multiple.
import euler from "../../support/goose";

fn solve() -> i64 {
    var n = 1;
    for d in 1..21 {
        n = n / gcd(n, d) * d;
    }
    return n;
}

fn main() {
    print(solve());
}
