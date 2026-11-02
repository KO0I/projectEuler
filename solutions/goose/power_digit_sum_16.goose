// Project Euler 16: Power Digit Sum.
import euler from "../../support/goose";

fn solve() -> i64 {
    var b = big(1);
    for _ in 1000 {
        multiply(b, 2);
    }
    return big_sum(b);
}

fn main() {
    print(solve());
}
