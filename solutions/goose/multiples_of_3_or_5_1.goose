// Project Euler 1: Multiples of 3 or 5.
import euler from "../../support/goose";

fn solve() -> i64 {
    var s = 0;
    for n in 1..1000 {
        if n % 3 == 0 || n % 5 == 0 {
            s += n;
        }
    }
    return s;
}

fn main() {
    print(solve());
}
