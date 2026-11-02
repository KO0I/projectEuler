// Project Euler 34: Digit Factorials.
import euler from "../../support/goose";

fn solve() -> i64 {
    let f = [1, 1, 2, 6, 24, 120, 720, 5040, 40320, 362880];
    var total = 0;
    for n in 3..2540161 {
        var t = n;
        var s = 0;
        while t > 0 {
            s += f[t % 10];
            t /= 10;
        }
        if s == n {
            total += n;
        }
    }
    return total;
}

fn main() {
    print(solve());
}
