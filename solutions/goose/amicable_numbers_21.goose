// Project Euler 21: Amicable Numbers.
import euler from "../../support/goose";

fn solve() -> i64 {
    var s = 0;
    for n in 2..10000 {
        let b = divisor_sum(n);
        if b != n && divisor_sum(b) == n {
            s += n;
        }
    }
    return s;
}

fn main() {
    print(solve());
}
