// Project Euler 97: Large Non-Mersenne Prime.
import euler from "../../support/goose";

fn solve() -> i64 {
    var n = 1;
    for _ in 7830457 {
        n = n * 2 % 10000000000;
    }
    return (28433 * n + 1) % 10000000000;
}

fn main() {
    print(solve());
}
