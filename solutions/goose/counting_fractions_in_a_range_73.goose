// Project Euler 73: Counting Fractions in a Range.
import euler from "../../support/goose";

fn solve() -> i64 {
    var count = 0;
    for d in 2..12001 {
        for n in d / 3 + 1..(d + 1) / 2 {
            if gcd(n, d) == 1 {
                count++;
            }
        }
    }
    return count;
}

fn main() {
    print(solve());
}
