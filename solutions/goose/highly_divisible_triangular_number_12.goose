// Project Euler 12: Highly Divisible Triangular Number.
import euler from "../../support/goose";

fn solve() -> i64 {
    var n = 1;
    loop {
        let a = if n % 2 == 0 {
            n / 2
        } else {
            n
        };
        let b = if n % 2 == 0 {
            n + 1
        } else {
            (n + 1) / 2
        };
        if divisor_count(a) * divisor_count(b) > 500 {
            return n * (n + 1) / 2;
        }
        n++;
    }
}

fn main() {
    print(solve());
}
