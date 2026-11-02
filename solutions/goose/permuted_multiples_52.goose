// Project Euler 52: Permuted Multiples.
import euler from "../../support/goose";

fn solve() -> i64 {
    var n = 1;
    loop {
        var ok = true;
        for k in 2..7 {
            if signature(n) != signature(k * n) {
                ok = false;
                break;
            }
        }
        if ok {
            return n;
        }
        n++;
    }
}

fn main() {
    print(solve());
}
