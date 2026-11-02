// Project Euler 63: Powerful Digit Counts.
import euler from "../../support/goose";

fn solve() -> i64 {
    var count = 0;
    for a in 1..10 {
        var b = big(1);
        for n in 1..30 {
            multiply(b, a);
            if b.len == n {
                count++;
            }
        }
    }
    return count;
}

fn main() {
    print(solve());
}
