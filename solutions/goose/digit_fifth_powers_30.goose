// Project Euler 30: Digit Fifth Powers.
import euler from "../../support/goose";

fn solve() -> i64 {
    var sum = 0;
    for n in 2..354295 {
        var v = n;
        var s = 0;
        while v > 0 {
            s += ipow(v % 10, 5);
            v /= 10;
        }
        if s == n {
            sum += n;
        }
    }
    return sum;
}

fn main() {
    print(solve());
}
