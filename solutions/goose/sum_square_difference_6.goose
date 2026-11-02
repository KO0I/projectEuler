// Project Euler 6: Sum Square Difference.
import euler from "../../support/goose";

fn solve() -> i64 {
    var a = 0;
    var b = 0;
    for n in 1..101 {
        a += n;
        b += n * n;
    }
    return a * a - b;
}

fn main() {
    print(solve());
}
