// Project Euler 38: Pandigital Multiples.
import euler from "../../support/goose";

fn solve() -> i64 {
    var best = 0;
    for n in 1..10000 {
        var c = 0;
        var k = 1;
        while digits(c) < 9 {
            let v = n * k;
            c = c * ipow(10, digits(v)) + v;
            k++;
        }
        if k > 2 && pandigital(c) {
            best = max(best, c);
        }
    }
    return best;
}

fn main() {
    print(solve());
}
