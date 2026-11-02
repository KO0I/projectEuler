// Project Euler 69: Totient Maximum.
import euler from "../../support/goose";

fn solve() -> i64 {
    var n = 1;
    var p = 2;
    loop {
        if prime(p) {
            if n * p > 1000000 {
                return n;
            }
            n *= p;
        }
        p++;
    }
}

fn main() {
    print(solve());
}
