// Project Euler 28: Number Spiral Diagonals.
import euler from "../../support/goose";

fn solve() -> i64 {
    var sum = 1;
    for k in 1..501 {
        sum += 4 * (2 * k + 1) * (2 * k + 1) - 12 * k;
    }
    return sum;
}

fn main() {
    print(solve());
}
