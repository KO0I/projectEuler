// Project Euler 20: Factorial Digit Sum.
import euler from "../../support/goose";

fn solve() -> i64 {
    var b = big(1);
    for n in 2..101 {
        multiply(b, n);
    }
    return big_sum(b);
}

fn main() {
    print(solve());
}
