// Project Euler 57: Square Root Convergents.
import euler from "../../support/goose";

fn solve() -> i64 {
    var n = big(3);
    var d = big(2);
    var count = 0;
    for _ in 1000 {
        if n.len > d.len {
            count++;
        }
        var nn = n;
        add(nn, d);
        add(nn, d);
        add(d, n);
        n = nn;
    }
    return count;
}

fn main() {
    print(solve());
}
