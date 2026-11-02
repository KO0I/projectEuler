// Project Euler 2: Even Fibonacci Numbers.
import euler from "../../support/goose";

fn solve() -> i64 {
    var a = 1;
    var b = 2;
    var s = 0;
    while a < 4000000 {
        if a % 2 == 0 {
            s += a;
        }
        let t = a + b;
        a = b;
        b = t;
    }
    return s;
}

fn main() {
    print(solve());
}
