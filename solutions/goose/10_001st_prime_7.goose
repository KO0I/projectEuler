// Project Euler 7: 10 001st Prime.
import euler from "../../support/goose";

fn solve() -> i64 {
    var count = 0;
    var n = 1;
    while count < 10001 {
        n++;
        if prime(n) {
            count++;
        }
    }
    return n;
}

fn main() {
    print(solve());
}
