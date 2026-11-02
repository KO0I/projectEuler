// Project Euler 25: 1000-digit Fibonacci Number.
import euler from "../../support/goose";

fn solve() -> i64 {
    var a = big(1);
    var b = big(1);
    var k = 2;
    while b.len < 1000 {
        var c = a;
        add(c, b);
        a = b;
        b = c;
        k++;
    }
    return k;
}

fn main() {
    print(solve());
}
