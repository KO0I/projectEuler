// Project Euler 56: Powerful Digit Sum.
import euler from "../../support/goose";

fn solve() -> i64 {
    var best = 0;
    for a in 1..100 {
        var b = big(1);
        for _ in 1..100 {
            multiply(b, a);
            best = max(best, big_sum(b));
        }
    }
    return best;
}

fn main() {
    print(solve());
}
