// Project Euler 48: Self Powers.
import euler from "../../support/goose";

fn solve() -> i64 {
    var sum = 0;
    for n in 1..1001 {
        var v = 1;
        for _ in n {
            v = v * n % 10000000000;
        }
        sum = (sum + v) % 10000000000;
    }
    return sum;
}

fn main() {
    print(solve());
}
